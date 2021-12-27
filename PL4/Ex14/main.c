/*
 * main.c
 *
 *  Created on: 15/05/2021
 *      Author: raulcoelho
 */
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>

typedef struct{
	int nr_readers;
} readers;

int main()
{	
    int i;
    pid_t pid;
    pid = fork();
    if (pid == 0)
    {
        execlp("./writer", "./writer", (char *)NULL);
        exit (1);
    }

    for (i = 0; i < 6; i++)
    {
        pid = fork();
        if (pid == 0)
        {
            execlp("./reader", "./reader", (char *)NULL);
            exit (1);
        }
    }

    sleep(20);

    pid = fork();
    if (pid == 0)
    {
        execlp("./writer", "./writer", (char *)NULL);
        exit (1);
    }

    for(i = 0 ; i < 8 ; i++){
        wait(NULL);
    }
    
    return 0;
}

//Eu Miguel Silva, 1190778, revi o exercício do meu colega Raúl.