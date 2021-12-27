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

int main(void)
{
    int fd[2];
    int i, j, maximo_geral = 0;
    pid_t pid[SON_SIZE];
    int arrayGeral[ARRAY_SIZE];

    if (pipe(fd) == -1)
    {
        perror("Pipe failed\n");
        exit(1);
    }

    srand(time(NULL));
    for (i = 0; i < ARRAY_SIZE; i++)
    {
        arrayGeral[i] = rand() % 1000 + 1;
    }

    for (i = 0; i < SON_SIZE; i++)
    {
        pid[i] = fork();
        if (pid < 0)
        {
            perror("Fork Failed.");
            exit(-1);
        }
        else if (pid[i] == 0)
        {
            close(fd[0]);
            int k = i * 100;
            int max = 0;
            for (j = k; j < k + 99; j++)
            {
                if (arrayGeral[j] > max)
                    max = arrayGeral[j];
            }
            write(fd[1], &max, sizeof(int));
            close(fd[1]);
            exit(0);
        }
    }

    for (i = 0; i < SON_SIZE; i++)
    {
        wait(NULL);
    }

    close(fd[1]);

    for (i = 0; i < SON_SIZE; i++)
    {
        int temp = 0;
        read(fd[0], &temp, sizeof(int));
        if (temp > maximo_geral)
        {
            maximo_geral = temp;
        }
        printf("Valor Máximo do Filho %d: %d\n", i, temp);
    }
    printf("Maior valor de todos: %d\n", maximo_geral);
    close(fd[0]);

    return 0;
}

//Eu Raúl Coelho revi o exercício do meu colega.