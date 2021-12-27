 /* ex7.c
 *
 *  Created on: 05/04/2021
 *      Author: raulcoelho
 */
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>

#define vec_size 1000

int main(void)
{
    int fd[5][2];
    int total[vec_size];
    int vec1[vec_size], vec2[vec_size], i, j;

    srand(time(NULL));
    for (i = 0; i < vec_size; i++)
    {
        vec1[i] = rand() % 50;
        vec2[i] = rand() % 50;
    }

    pid_t pid[5];
    int status;

    for (i = 0; i < 5; i++)
    {
        if (pipe(fd[i]) == -1)
        {
            perror("Pipe failed\n");
            return 1;
        }
        pid[i] = fork();
        if (pid[i] == 0)
        {
            close(fd[i][0]);
            int inicio = i * (vec_size / 5);
            int fim = (i + 1) * (vec_size / 5);
            int tmp = 0;
            for (j = inicio; j < fim; j++)
            {
                tmp = vec1[j] + vec2[j];
                write(fd[i][1], &tmp, sizeof(int));
            }
            close(fd[i][1]);
            exit(0);
        }
        else if (pid[i] < 0)
        {
            perror("fork failed");
            exit(1);
        }
        else
        {
            wait(&status);
            close(fd[i][1]);

            int inicio = i * (vec_size / 5);
            int fim = (i + 1) * (vec_size / 5);

            for (j = inicio; j < fim; j++)
            {
                int tmp = 0;
                read(fd[i][0], &tmp, sizeof(int));
                total[j] = tmp;
                printf("total na posição %d = %d\n", j, total[j]);
            }
            close(fd[i][0]);
        }
    }
    return 0;
}

/*
 * Eu Jose Silva nº1190778 revi o exercicio do meu colega raul
 */
