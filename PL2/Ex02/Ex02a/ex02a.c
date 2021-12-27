/*
 * ex2a.c
 *
 *  Created on: 05/04/2021
 *      Author: raulcoelho
 */
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

#define BUFFER_SIZE 80

int main(void)
{
    int fd[2];
    
    if (pipe(fd) == -1)
    {
     perror("Pipe failed\n");
     return 1;
    }
    
    pid_t pid;
    int inteiro, status;
    char str[BUFFER_SIZE];

    pid = fork();

    if(pid > 0){
        close(fd[0]);
        printf("Type a number: \n");
        scanf("%d", &inteiro);
        write(fd[1],&inteiro,sizeof(int));
        printf("Type a word: \n");
        scanf("%s", str);
        write(fd[1], str, strlen(str)+1);
        close(fd[1]);
        waitpid(pid,&status,0);
    }else if(pid == 0){
        close(fd[1]);
        read(fd[0], &inteiro,sizeof(int));
        read(fd[0], str, BUFFER_SIZE);
        printf("The number read was : %d\n", inteiro);
        printf("The word read was : %s\n", str);
        close(fd[0]);
    }else{
        perror("fork failed");
    }
    return 0;
}

/*
 * Eu Jose Silva nº1190778 revi o exercicio do meu colega raul
 */