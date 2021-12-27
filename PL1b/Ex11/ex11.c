/*
 * ex11.c
 *
 *  Created on: 02/04/2021
 *      Author: raulcoelho
 */
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

typedef struct
{
    char cmd[32];
    int tempo;
} comando;

int COMANDOS = 3;

int main(int argc, char *argv[])
{
    comando array[COMANDOS];
    strcpy(array[0].cmd,"./comando1.o");
    array[0].tempo=15;
    strcpy(array[1].cmd,"./comando2.o");
    array[1].tempo=5;
    strcpy(array[2].cmd,"./comando3.o");
    array[2].tempo=20;

    int i;
    int status[3];
    pid_t pid[3];

    for (i = 0; i < COMANDOS; i++)
    {
        pid[i] = fork();
        if (pid[i] < 0)
        {
            perror("fork failed");
        }
        else if(pid[i] == 0)
        {
            alarm(array[i].tempo);
            execl(array[i].cmd, array[i].cmd, (char *) NULL);
            exit(-1); /* means exec failed */
        }
    }
    for(i = 0; i < 1; i++){
		waitpid(pid[i], &status[i] ,0);
	}
    return 0;
}

/*
 * Eu Jose Silva nº1190778 revi o exercicio do meu colega raul
 */