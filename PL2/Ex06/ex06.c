/*
 * ex6.c
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
    int fd[2];

    if (pipe(fd) == -1)
    {
        perror("Pipe failed\n");
        return 1;
    }

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
        pid[i] = fork();
        if (pid[i] == 0)
        {
            int inicio = i * (vec_size / 5);
            int tmp = 0;
            for (j = inicio; j < inicio + (vec_size / 5); j++)
            {
                tmp += vec1[j] + vec2[j];
            }
            close(fd[0]);
            write(fd[1], &tmp, sizeof(int));
            close(fd[1]);
            exit(0);
        }
        else if (pid[i] < 0)
        {
            perror("fork failed");
        }
    }

    int total = 0;
    close(fd[1]);
    for (i = 0; i < 5; i++)
    {
        wait(&status);
        int tmp = 0;
        read(fd[0], &tmp, sizeof(int));
        printf("Filho calculou: %d\n", tmp);
        total += tmp;
    }
    close(fd[0]);

    printf("Total: %d\n", total);
    return 0;
}

/*
Pipes behave FIFO(First in First out), Pipes behave like a queue structure. 
So basically it just reads the info that the first child wrote in the pipe and
then second etc. 
*/

/*
 * Eu Jose Silva nº1190778 revi o exercicio do meu colega raul
 */