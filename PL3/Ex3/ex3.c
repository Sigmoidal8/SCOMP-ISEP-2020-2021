/*
 * ex3.c
 *
 *  Created on: 04/04/2021
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
#include <string.h>
#include <sys/time.h>

#define SIZE 50

long startMemory, finishMemory;

typedef struct
{
    char phrase[25];
    int number;
} combo;

long getMicrotime()
{
    struct timeval currentTime;
    gettimeofday(&currentTime, NULL);
    return currentTime.tv_sec * (int)1e6 + currentTime.tv_usec;
}

void fill_array(combo *array, int size)
{
    int i;
    srand(time(NULL) * getpid());
    for (i = 0; i < size; i++)
    {
        strcpy(array[i].phrase, "ISEP – SCOMP 2020");
        array[i].number = (rand() % 500);
    }
}

int writer()
{
    int data_size = SIZE * sizeof(combo);
    shm_unlink("/ex03");
    int fd = shm_open("/ex03", O_EXCL | O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        printf("Failed shm writer!\n");
        exit(EXIT_FAILURE);
    }

    int truncate = ftruncate(fd, data_size);
    if (truncate == -1)
    {
        printf("Error ftruncate!\n");
        exit(EXIT_FAILURE);
    }

    startMemory = getMicrotime();
    combo *shared_data = (combo *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (shared_data == MAP_FAILED)
    {
        printf("Error mmap!\n");
        exit(EXIT_FAILURE);
    }

    fill_array(shared_data, SIZE);

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
    return 0;
}

int reader()
{
    int data_size = SIZE * sizeof(combo);

    int fd = shm_open("/ex03", O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
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

    combo *shared_data = (combo *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (shared_data == MAP_FAILED)
    {
        printf("Error mmap!\n");
        exit(EXIT_FAILURE);
    }

    finishMemory = getMicrotime();
    int i;
    for (i = 0; i < SIZE; i++)
    {
        printf("Array[%d] = %d\n", i, shared_data[i].number);
    }

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

    if (shm_unlink("/ex03") < 0)
    {
        perror("erro a fechar memória");
        exit(EXIT_FAILURE);
    }

    return 0;
}

int main()
{
    int p[2], status = 0;
    long start, finish;
    combo array[SIZE];

    if (pipe(p) == -1)
    {
        perror("Pipe failed\n");
        return 1;
    }

    pid_t pid = fork();

    if (pid > 0)
    {
        close(p[0]);
        fill_array(array, SIZE);
        start = getMicrotime();
        write(p[1], array, sizeof(combo) * SIZE);
        close(p[1]);
    }
    else
    {
        close(p[1]);
        read(p[0], &array, sizeof(combo) * SIZE);
        close(p[0]);

        finish = getMicrotime();

        int i;
        for (i = 0; i < SIZE; i++)
        {
            printf("Array[%d] = %d\n", i, array[i].number);
        }
        exit(finish);
    }

    wait(&status);
    double total = (double)(status - start) / CLOCKS_PER_SEC;
    printf("Pipe total time: %f microseconds.\n", total);

    writer();
    reader();

    total = (double)(finishMemory - startMemory) / CLOCKS_PER_SEC;
    printf("Shared Memory total time: %f microseconds.\n", total);

    return 0;

}

//Eu Miguel Silva, 1190778, revi o exercício do meu colega Raúl.