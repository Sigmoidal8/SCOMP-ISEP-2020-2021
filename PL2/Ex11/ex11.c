/*
 * ex11.c
 *
 *  Created on: 08/04/2021
 *      Author: raulcoelho
 */
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(void)
{
    int gerado = 0;
    int recebido = 0;
    int i;
    time_t t;

    pid_t pid[5];
    int fd[6][2], status;

    for (i = 0; i < 6; i++)
    {
        if (pipe(fd[i]) == -1)
        {
            perror("Pipe failed\n");
            return 1;
        }
    }

    for (i = 0; i < 5; i++)
    {
        pid[i] = fork();
        if (pid[i] == 0)
        {
            srand((unsigned)time(&t) ^ getpid());
            gerado = rand() % 500 + 1;
            printf("PID: %d Number: %d\n", getpid(), gerado);
            close(fd[i][1]);
            read(fd[i][0], &recebido, sizeof(int));
            close(fd[i][0]);

            close(fd[i + 1][0]);
            if (gerado > recebido)
            {
                write(fd[i + 1][1], &gerado, sizeof(int));
            }
            else
            {
                write(fd[i + 1][1], &recebido, sizeof(int));
            }
            close(fd[i + 1][1]);
            exit(0);
        }
        else if (pid[i] < 0)
        {
            perror("failed fork");
            exit(0);
        }
    }

    srand((unsigned)time(&t) ^ getpid());
    gerado = rand() % 500 + 1;
    printf("PID: %d Number: %d\n", getpid(), gerado);

    close(fd[0][0]);
    write(fd[0][1], &gerado, sizeof(int));
    close(fd[0][1]);

    for (i = 0; i < 5; i++)
    {
        waitpid(pid[i], &status, 0);
    }

    close(fd[5][1]);
    read(fd[5][0], &recebido, sizeof(int));
    close(fd[5][0]);

    printf("Biggest number: %d\n", recebido);

    return 0;
}

/*
 * Eu Jose Silva nº1190778 revi o exercicio do meu colega raul
 */
