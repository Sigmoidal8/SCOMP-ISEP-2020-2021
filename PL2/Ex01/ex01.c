/*
 * ex1.c
 *
 *  Created on: 05/04/2021
 *      Author: raulcoelho
 */

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

int main(void)
{
    int read_msg;
    int write_msg;
    int fd[2];
    
    if (pipe(fd) == -1)
    {
     perror("Pipe failed\n");
     return 1;
    }

    pid_t ppid = getpid();
    pid_t pid;
    int status;

    printf("PID : %d\n", ppid);
    write_msg=ppid;

    pid = fork();

    if(pid > 0){
        close(fd[0]);
        write(fd[1],&write_msg,sizeof(int));
        close(fd[1]);
        //waitpid(pid, &status ,0);
    }else if(pid == 0){
        close(fd[1]);
        read(fd[0], &read_msg, sizeof(read_msg));
        printf("My parent PID is : %d\n", read_msg);
        close(fd[0]);
    }else{
        perror("fork failed");
    }
    return 0;
}

/*
 * Eu Jose Silva nº1190778 revi o exercicio do meu colega raul
 */