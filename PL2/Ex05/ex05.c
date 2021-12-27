/*
 * ex5.c
 *
 *  Created on: 05/04/2021
 *      Author: raulcoelho
 */
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

#define BUFFER_SIZE 256

int main(void)
{
    int up[2], down[2];

    if (pipe(up) == -1 || pipe(down) == -1)
    {
        perror("Pipe failed\n");
        return 1;
    }

    pid_t pid;
    int status, i = 0;
    char in[BUFFER_SIZE];
    char out[BUFFER_SIZE];

    pid = fork();

    if (pid > 0)
    {
        close(up[1]);
        read(up[0], out, BUFFER_SIZE);
        close(up[0]);

        for(i = 0; out[i] != '\0'; i++)
        {
            if (out[i] >= 'A' && out[i] <= 'Z')
            {
                out[i] = out[i] + 32;
            }
            else if (out[i] >= 'a' && out[i] <= 'z')
            {
                out[i] = out[i] - 32;
            }
        }

        close(down[0]);
        write(down[1], out, strlen(out)+1);
        close(down[1]);

        waitpid(pid, &status, 0);
    }
    else if (pid == 0)
    {
        close(up[0]);
        printf("Type a message :\n");
        scanf("%s", in);
        write(up[1], in, strlen(in)+1);
        close(up[1]);

        close(down[1]);
        read(down[0], in, BUFFER_SIZE);
        close(down[0]);
        printf("The message was: %s\n", in);
        exit(0);
    }
    else
    {
        perror("fork failed");
    }
    return 0;
}

/*
 * Eu Jose Silva nº1190778 revi o exercicio do meu colega raul
 */