/*
 * ex9.c
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

void buy_chips(){
    printf("Buying chips\n");
    sleep(5);
}

void buy_beer(){
    printf("Buying beer\n");
    sleep(5);
}

void eat_and_drink(){
    printf("Eating\n");
    sleep(2);
}

int main(){
    sem_t *sem[2];
	if ((sem[0] = sem_open("/ex9p1", O_CREAT | O_EXCL, 0644, 0))
			== SEM_FAILED) {
		perror("Erro a criar semáforo");
		exit(1);
	}
    if ((sem[1] = sem_open("/ex9p2", O_CREAT | O_EXCL, 0644, 0))
			== SEM_FAILED) {
		perror("Erro a criar semáforo");
		exit(1);
	}

    pid_t pid = fork();

    if(pid == 0){
        buy_chips();
        sem_post(sem[0]);
        sem_wait(sem[1]);
        eat_and_drink();
        exit(0);
    }

    if(pid > 0){
        buy_beer();
        sem_post(sem[1]);
        sem_wait(sem[0]);
        eat_and_drink();
    }

    if (sem_close(sem[0]) < 0 || sem_close(sem[1]) < 0)
    {
        printf("Error  close()!\n");
        exit(EXIT_FAILURE);
    }

    if (sem_unlink("ex9p1") < 0 || sem_unlink("ex9p2") < 0)
    {
        printf("Error  unlink()!\n");
        exit(EXIT_FAILURE);
    }
    
    printf("Both processes have eaten\n");
    return 0;
}

//Eu Miguel Silva, 1190778, revi o exercício do meu colega Raúl.