/*
 * ex13.c
 *
 *  Created on: 15/05/2021
 *      Author: miguelsilva
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <string.h>
#include <semaphore.h>
#define BUFFER 10

typedef struct
{
    int buffer[BUFFER];
    int inicio;
    int fim;
    int size;
} buffer_circular;

int verificar_full(buffer_circular *shared)
{
    if ((shared->inicio + 1) % shared->size == shared->fim)
    {
        return 1;
    }
    return 0;
}

int verificar_empty(buffer_circular *shared)
{
    if (shared->inicio == shared->fim)
    {
        return 1;
    }
    return 0;
}

int main(void)
{

    sem_t *semMemory;
    sem_t *semFull;
    sem_t *semEmpty;
    sem_t *semFilhos;

    if ((semMemory = sem_open("semMemory", O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED)
    {
        sem_unlink("semMemory");
        perror("sem_open error");
        exit(-1);
    }
    if ((semFull = sem_open("semFull", O_CREAT | O_EXCL, 0644, 10)) == SEM_FAILED)
    {
        sem_unlink("semFull");
        perror("sem_open error");
        exit(-1);
    }
    if ((semEmpty = sem_open("semEmpty", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED)
    {
        sem_unlink("semEmpty");
        perror("sem_open error");
        exit(-1);
    }
    if ((semFilhos = sem_open("semFilhos", O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED)
    {
        sem_unlink("semFilhos");
        perror("sem_open error");
        exit(-1);
    }

    int fd, fd2, data_size = sizeof(buffer_circular);

    buffer_circular *shared;

    shm_unlink("/shmtest");

    fd = shm_open("shmtest", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);

    if (fd < 0)
    {
        perror("Failed opening shared memory");
        exit(-1);
    }

    fd2 = ftruncate(fd, data_size);

    if (fd2 < 0)
    {
        perror("Failed defining the size of the area");
        exit(-1);
    }

    shared = (buffer_circular *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if (shared == MAP_FAILED || shared == 0)
    {
        perror("Error mapping memory");
        exit(-1);
    }

    shared->size = BUFFER;

    shared->inicio = 0;
    shared->fim = 0;

    int i,j=0,k=0,l;

    for (l = 0; l < 2; l++)
    {

        int pid = fork();
        if (pid < 0)
        {
            perror("Error on fork\n");
            exit(-1);
        }

        if (pid == 0)
        {
            while (k < 3)
            {
                for (i = 0; i < 5; i++)
                {

                    sem_wait(semFull); 
                    sem_wait(semMemory);

                    shared->buffer[shared->inicio] = ((i + 1) + (k * 5) + (l * 15));
                    shared->inicio = (shared->inicio + 1) % shared->size;

                    sem_post(semMemory);
                    sem_post(semEmpty); 
                }

                k++;
            }
            exit(0);
        }
    }

    while (j < 3)
    {

        for (i = 0; i < 10; i++)
        {

            sem_wait(semFilhos);
            sem_wait(semEmpty); 
            sem_wait(semMemory); 

            int lido;
            lido = shared->buffer[shared->fim];
            printf("Foi lido o valor %d da posicao %d\n", lido, shared->fim);
            shared->fim = (shared->fim + 1) % shared->size;

            sem_post(semMemory);
            sem_post(semFull);
            sem_post(semFilhos);
        }
        j++;
    }

    for (i = 0; i < 2; i++)
    {
        wait(NULL);
    }

    if (munmap(shared, 300) < 0)
    {
        perror("Error on munmap\n");
        exit(-1);
    }

    if (sem_unlink("semMemory") < 0)
    {
        perror("Error unlinking semaphore");
        exit(-1);
    }
    if (sem_unlink("semFull") < 0)
    {
        perror("Error unlinking semaphore");
        exit(-1);
    }

    if (sem_unlink("semEmpty") < 0)
    {
        perror("Error unlinking semaphore");
        exit(-1);
    }
    if (sem_unlink("semFilhos") < 0)
    {
        perror("Error unlinking semaphore");
        exit(-1);
    }

    return 0;
}
//Eu Raúl Coelho revi o exercício do meu colega