/*
 * ex08.c
 *
 *  Created on: 01/04/2021
 *      Author: raulcoelho
 */
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

volatile sig_atomic_t COUNTER = 0;

void handler(int sign)
{
    COUNTER++;
}

int main(int argc, char *argv[])
{
    struct sigaction sa;
    sa.sa_handler = &handler;
    sa.sa_flags = SA_NOCLDSTOP;
    sigaction(SIGUSR1, &sa, NULL);

    pid_t pid[5];
    pid_t ppid = getpid();
    int i, j;
    int status[5];

    for (i = 0; i < 5; i++)
    {
        pid[i] = fork();
        if (pid[i] == 0)
        {
            int inicio = i * 200;
            for (j = inicio; j < inicio + 200; j++)
            {
                printf("%d\n", j);
            }
            kill(ppid, SIGUSR1);
            exit(0);
        }
        else if (pid[i] < 0)
        {
            perror("fork falhou");
        }
    }
    while (COUNTER != 5)
    {
        pause();
    }
    for(i = 0; i < 5; i++){
		waitpid(pid[i], &status[i] ,0);
	}
    return 0;
}

/*
 * Eu Jose Silva nº1190778 revi o exercicio do meu colega raul
 */