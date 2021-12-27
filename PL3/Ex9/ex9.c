/*
 * ex9.c
 *
 *  Created on: 29/04/2021
 *      Author: miguelsilva
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <wait.h>
#include <time.h>

#define ARRAY_SIZE 1000
#define SON_SIZE 10

typedef struct
{
    int arrayGeral[ARRAY_SIZE];
    int arrayFilhos[SON_SIZE];
} valores;

int main(void)
{
    int fd, data_size = sizeof(valores);
    int i, j, maximo_geral = 0;
    int status;
    pid_t pid[SON_SIZE];
    valores *numeros;

    fd = shm_open("/ex9", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
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

    numeros = (valores *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (numeros == MAP_FAILED)
    {
        perror("Error mmap!\n");
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));
    for (i = 0; i < ARRAY_SIZE; i++)
    {
        numeros->arrayGeral[i] = rand() % 1000;
    }

    for (i = 0; i < SON_SIZE; i++)
    {

        int maximo_filho = 0;
        pid[i] = fork();
        if (pid < 0)
        {
            perror("Fork Failed.");
            exit(-1);
        }
        else if (pid[i] == 0)
        {
            int k = i * 100;
            for (j = k; j < k + 99; j++)
            {
                if (numeros->arrayGeral[j] > maximo_filho)
                {
                    maximo_filho = numeros->arrayGeral[j];
                }
            }
            numeros->arrayFilhos[i] = maximo_filho;
            exit(i);
        }
    }

    for (i = 0; i < SON_SIZE; i++)
    {
        waitpid(pid[i], &status, 0);
        if (numeros->arrayFilhos[i] > maximo_geral)
        {
            maximo_geral = numeros->arrayFilhos[i];
        }
    }

    printf("Maior valor encontrado: %d \n", maximo_geral);

    if(close(fd)<0){
        perror("close");
        exit(EXIT_FAILURE);
    }

    if(shm_unlink("/ex9") < 0){
        perror("Error");
        exit(EXIT_FAILURE);
    }

    return 0;
}

//Eu Raúl Coelho revi o exercício do meu colega