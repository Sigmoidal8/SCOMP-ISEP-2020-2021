/*
 * ex2b.c
 *
 *  Created on: 05/04/2021
 *      Author: raulcoelho
 */
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

#define BUFFER_SIZE 80

typedef struct
{
    char str[BUFFER_SIZE];
    int inteiro;
} message;

int main(void)
{
    int fd[2];

    if (pipe(fd) == -1)
    {
        perror("Pipe failed\n");
        return 1;
    }

    pid_t pid;
    int status;
    message pipe_message;

    pid = fork();

    if (pid > 0)
    {
        close(fd[0]);
        printf("Type a number: \n");
        scanf("%d", &pipe_message.inteiro);
        printf("Type a word: \n");
        scanf("%s", pipe_message.str);
        write(fd[1], &pipe_message, sizeof(message));
        close(fd[1]);
        waitpid(pid, &status, 0);
    }
    else if (pid == 0)
    {
        close(fd[1]);
        read(fd[0], &pipe_message, sizeof(message));
        printf("The number read was : %d\n", pipe_message.inteiro);
        printf("The word read was : %s\n", pipe_message.str);
        close(fd[0]);
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