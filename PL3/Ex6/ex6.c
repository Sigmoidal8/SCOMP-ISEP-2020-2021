/*
 * ex6.c
 *
 *  Created on: 28/04/2021
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

#define ARRAY_SIZE 1000000

typedef struct
{
    int numbers[ARRAY_SIZE];
    int verif;
} shared_memory;

void pipes()
{
    int i = 0, fd[2];
    pid_t pid;
    time_t start, finish;
    int arrayGeral[ARRAY_SIZE];

    if (pipe(fd) == -1)
    {
        perror("Pipe failed\n");
        exit(1);
    }

    srand(time(NULL));

    for (i = 0; i < ARRAY_SIZE; i++)
    {
        arrayGeral[i] = rand() % 10;
    }

    start = clock();

    pid = fork();
    if (pid < 0)
    {
        perror("Fork Failed.");
        exit(-1);
    }
    else if (pid == 0)
    {
        close(fd[0]);
        for (i = 0; i < ARRAY_SIZE; i++)
        {
            write(fd[1], &arrayGeral[i], sizeof(arrayGeral[i]));
        }
        close(fd[1]);
        exit(0);
    }
    else
    {
        close(fd[1]);
        int array2[ARRAY_SIZE];
        for (i = 0; i < ARRAY_SIZE; i++)
        {
            read(fd[0], &array2[i], sizeof(array2[i]));
            close(fd[0]);
        }
        wait(NULL);

        finish = clock();

        double time = ((double)(finish - start)) / CLOCKS_PER_SEC;

        printf("\nA transferência por pipe demorou: %.2f segundos\n", time);
    }
}

void sharedd()
{
    int i, fd, data_size = sizeof(shared_memory);
    double totaltime;
    time_t start, finish;
    pid_t pid;
    int arrayGeral[ARRAY_SIZE];
    shared_memory *memory_area;

    start = clock();

    fd = shm_open("/ex6", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
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

    memory_area = (shared_memory *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (memory_area == MAP_FAILED)
    {
        perror("Error mmap!\n");
        exit(EXIT_FAILURE);
    }

    pid = fork();

    memory_area->verif = 0;
    if (pid < 0)
    {
        perror("Fork Failed.");
        exit(-1);
    }
    else if (pid == 0)
    {
        // Wait for switch to turn to 1
        while (memory_area->verif)
            ;
        for (i = 0; i < ARRAY_SIZE; i++)
        {
            memory_area->numbers[i] = i;
        }
        exit(0);
    }
    else
    {
        for (i = 0; i < ARRAY_SIZE; i++)
        {
            arrayGeral[i] = memory_area->numbers[i];
        }
    }
    wait(NULL);

    if (munmap(memory_area, data_size) < 0)
    {
        printf("Error  munmap()!\n");
        exit(EXIT_FAILURE);
    }
    if (close(fd) < 0)
    {
        printf("Error  close()!\n");
        exit(EXIT_FAILURE);
    }

    finish = clock();

    totaltime = ((double)(finish - start)) / CLOCKS_PER_SEC;

    printf("\nA transferência por shared memory demorou: %.2f segundos\n", totaltime);
    if(shm_unlink("/ex6")<0){
        perror("Error");
        exit(EXIT_FAILURE);
    }
}

int main(void)
{
    pipes();
    sharedd();
    return 1;
}

//Eu Raúl Coelho 110986 revi o exercício do meu colega.