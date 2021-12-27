/*
 * ex3.c
 *
 *  Created on: 13/05/2021
 *      Author: miguelsilva
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>

#define MAX_SIZE 80
#define NUM_PROCESS 10
#define NUM_STRINGS 50

typedef struct
{
    int index;
    char s[MAX_SIZE][MAX_SIZE];
} shared_array;

int main(void)
{

    shared_array *shared_data;
    int i, fd, data_size = sizeof(shared_array);
    pid_t pid;


    fd = shm_open("/ex3", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        perror("Failed shm reader\n");
        exit(EXIT_FAILURE);
    }

    int aux = ftruncate(fd, data_size);
    if (aux == -1)
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

    sem_unlink("/ex3");
    sem_t *s = sem_open("/ex3", O_CREAT | O_EXCL, 0644, 1);
    if (s == SEM_FAILED)
    {
        printf("Error creating the semaphores.\n");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < 10; i++)
    {
        sprintf(shared_data->s[0], "%s %d", "\nI'm the Father - with PID ", getpid());

        pid = fork();
        wait(NULL);

        if (pid == 0)
        {
            sem_wait(s);
            printf("I'm the child %d. My father is %d.\n", getpid(), getppid());
            sleep(2);
            sem_post(s);
            exit(0);
        }

        sem_wait(s);
        printf("%s\n", shared_data->s[0]);
        sem_post(s);
    }

    // Disconnect map
    if (munmap(shared_data, data_size) < 0)
    {
        printf("Error  munmap()!\n");
        exit(EXIT_FAILURE);
    }
    // Close fd
    if (close(fd) < 0)
    {
        printf("Error  close()!\n");
        exit(EXIT_FAILURE);
    }

    // Unlink semaphore
    if (sem_unlink("/ex3"))
    {
        printf("Error  close()!\n");
        exit(EXIT_FAILURE);
    }
    return 0;
}

//Eu Raúl Coelho revi o exercício do meu colega.