/*
 * ex6.c
 *
 *  Created on: 13/05/2021
 *      Author: raulcoelho
 */
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>

int main()
{
    int i = 0;
    sem_t *sem[2];
    if ((sem[0] = sem_open("/ex5_1", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED)
    {
        perror("Erro a criar semáforo");
        exit(1);
    }
    if ((sem[1] = sem_open("/ex5_2", O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED)
    {
        perror("Erro a criar semáforo");
        exit(1);
    }

    pid_t pid = fork();

    for (i = 0; i < 15; i++)
    {
        if (pid == 0)
        {
            sem_wait(sem[1]);
            printf("I'm the child\n");
            sem_post(sem[0]);
        }
        if (pid > 0)
        {
            sem_wait(sem[0]);
            printf("I'm the father\n");
            sem_post(sem[1]);
        }
    }

    if (sem_close(sem[0]) < 0 || sem_close(sem[1]) < 0)
    {
        printf("Error  close()!\n");
        exit(EXIT_FAILURE);
    }

    if (sem_unlink("ex5_1") < 0 || sem_unlink("ex5_2") < 0)
    {
        printf("Error  unlink()!\n");
        exit(EXIT_FAILURE);
    }
    return 0;
}

//Eu Miguel Silva, 1190778, revi o exercício do meu colega Raúl.