/*
 * ex7.c
 *
 *  Created on: 29/04/2021
 *      Author: raulcoelho
 */
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

typedef struct
{
    int array[10];
} shared_array;

int writer()
{
    int fd, data_size = sizeof(int) * 10, i;
    shared_array *array;

    shm_unlink("/ex07");

    fd = shm_open("/ex07", O_CREAT | O_EXCL | O_RDWR, S_IWUSR | S_IRUSR);
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

    array = (shared_array *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (array == NULL)
    {
        perror("Error mmap!\n");
        exit(EXIT_FAILURE);
    }

    srand(time(NULL) * getpid());
    for (i = 0; i < 10; i++)
    {
        array->array[i] = (rand() % 20 + 1);
        printf("%d\n", array->array[i]);
    }

    if (munmap(array, data_size) < 0)
    {
        perror("Error munmap!\n");
        exit(EXIT_FAILURE);
    }

    if (close(fd) < 0)
    {
        perror("Error close!\n");
        exit(EXIT_FAILURE);
    }
    return 0;
}

int reader()
{
    int data_size = 10 * sizeof(int);

    int fd = shm_open("/ex07", O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        printf("Failed shm reader\n");
        exit(EXIT_FAILURE);
    }

    int truncate = ftruncate(fd, data_size);
    if (truncate == -1)
    {
        printf("Error ftruncate!\n");
        exit(EXIT_FAILURE);
    }

    shared_array *shared_data = (shared_array *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (shared_data == MAP_FAILED)
    {
        printf("Error mmap!\n");
        exit(EXIT_FAILURE);
    }

    int i, total = 0, resultado = 0;
    for (i = 0; i < 10; i++)
    {
        total += shared_data->array[i];
    }
    resultado = total / 10;
    printf("Average: %d\n", resultado);

    if (munmap(shared_data, data_size) < 0)
    {
        perror("Error munmap!\n");
        exit(EXIT_FAILURE);
    }

    if (close(fd) < 0)
    {
        perror("Error close!\n");
        exit(EXIT_FAILURE);
    }

    if (shm_unlink("/ex07") < 0)
    {
        perror("erro a fechar memória");
        exit(EXIT_FAILURE);
    }
    return 0;
}

int main()
{
    writer();
    reader();
    return 0;
}

//Eu Miguel Silva, 1190778, revi o exercício do meu colega Raúl.