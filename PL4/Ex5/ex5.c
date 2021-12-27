/*
 * ex5.c
 *
 *  Created on: 13/05/2021
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

int main(){
    sem_t *sem;
	if ((sem = sem_open("/ex5", O_CREAT | O_EXCL, 0644, 0))
			== SEM_FAILED) {
		perror("Erro a criar semáforo");
		exit(1);
	}

    pid_t pid = fork();

	if (pid ==0){
		printf("I'm the child\n");
		sem_post(sem);
	}
	if(pid > 0){
		sem_wait(sem);
		printf("I'm the father\n");
	}

    if (sem_close(sem) < 0)
    {
        printf("Error  close()!\n");
        exit(EXIT_FAILURE);
    }

    if (sem_unlink("/ex5"))
    {
        printf("Error  unlink()!\n");
        exit(EXIT_FAILURE);
    }
	return 0;
}

//Eu Raúl Coelho revi o exercício do meu colega.