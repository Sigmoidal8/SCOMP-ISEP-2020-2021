/*
 * ex4.c
 *
 *  Created on: 28/04/2021
 *      Author: miguelsilva
 */

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#define MAX_SIZE 100

typedef struct
{
    char arr[MAX_SIZE];
} shared_array;

void getArray(char array[MAX_SIZE])
{
    int i;
    printf("Array escrito: \n");
    for (i = 0; i < MAX_SIZE; i++)
    {
        array[i] = (rand() % (91 - 65)) + 65;
        printf("%c ", array[i]);
    }
    printf("\n");
}

int writer()
{
    char array_escrito[MAX_SIZE];
    getArray(array_escrito);

    shared_array *shared_data;
    int i, fd, data_size = sizeof(shared_array);

    fd = shm_open("/ex4", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        perror("Failed shm writer!\n");
        exit(EXIT_FAILURE);
    }

    int truncate = ftruncate(fd, data_size);
    if (truncate == -1)
    {
        perror("Error ftruncate!\n");
        exit(EXIT_FAILURE);
    }

    shared_data = (shared_array *)mmap(NULL, strlen(array_escrito) + 1, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (shared_data == MAP_FAILED)
    {
        perror("Error mmap!\n");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < MAX_SIZE; i++)
    {
        shared_data->arr[i] = array_escrito[i];
    }

    if (close(fd) < 0)
    {
        perror("Error");
        exit(EXIT_FAILURE);
    }

    return 0;
}

int reader()
{
    shared_array *shared_data;
    int i, fd, data_size = sizeof(shared_array);
    float soma = 0.0;

    fd = shm_open("/ex4", O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        perror("Failed shm reader\n");
        exit(EXIT_FAILURE);
    }

    int truncate = ftruncate(fd, data_size);
    if (truncate == -1)
    {
        perror("Error ftruncate!\n");
        exit(EXIT_FAILURE);
    }

    shared_data = (shared_array *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (shared_data == MAP_FAILED)
    {
        perror("Error mmap!\n");
        exit(EXIT_FAILURE);
    }

    printf("Array Lido:\n");
    for (i = 0; i < MAX_SIZE; i++)
    {
        printf("%c ", shared_data->arr[i]);
    }
    printf("\n");
    printf("\n");

    for (i = 0; i < MAX_SIZE; i++)
    {
        soma += (int)(shared_data->arr[i]); //cast para int de forma a ter o valor ASCII dos caracteres
    }

    float media;
    media = soma / MAX_SIZE;
    printf("Média do ASCII: %.2f\n", media);

    if (close(fd) < 0)
    {
        perror("Error");
        exit(EXIT_FAILURE);
    }

    return 0;
}

int main(void)
{

    writer();
    reader();

    if (shm_unlink("/ex4") < 0)
    {
        perror("erro a fechar memória");
        exit(EXIT_FAILURE);
    }

    return 0;
}

//Eu Raúl Coelho, 1190986, revi o exercício do meu colega Miguel.