/*
 * ex7.c
 *
 *  Created on: 13/05/2021
 *      Author: miguelsilva
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>

void semPost(sem_t *semaphore){
	if(sem_post(semaphore) == -1){
		printf("sem_post error.\n");
		exit(EXIT_FAILURE);
	}
}

void semWait(sem_t *semaphore){
	if(sem_wait(semaphore) == -1){
		printf("sem_wait error.\n");
		exit(EXIT_FAILURE);
	}
}

int main(){
	int i, status;

	sem_t *s1 = sem_open("/sem1",O_CREAT|O_EXCL, 0644, 0);
	sem_t *s2 = sem_open("/sem2",O_CREAT|O_EXCL, 0644, 0);
	sem_t *s3 = sem_open("/sem3",O_CREAT|O_EXCL, 0644, 0);
	
	if(s1 == SEM_FAILED || s2 == SEM_FAILED || s3 == SEM_FAILED){
		printf("Error creating the semaphores.\n");
		exit(EXIT_FAILURE);
	} 
	
	for(i = 0; i < 3; i++){
		
		pid_t pid = fork();
		if(pid == -1){
			perror("Error on fork system call.\n");
            exit(EXIT_FAILURE);
		}
		
		if(pid == 0){
			if(i == 0){				
				printf("\nSistemas ");
				fflush(stdout);	
				semPost(s2);    //incrementa s2 para o segundo processo poder avançar
				
				semWait(s1);    //espera que o terceiro processo incremente s1
				printf("a ");
				semPost(s2);   //incrementa s2 para o segundo processo poder avançar
				
				exit(EXIT_SUCCESS);
			}
			if(i == 1){				
				semWait(s2);   //espera que o primeiro processo incremente s2
				printf("de ");
				fflush(stdout);	
				semPost(s3);   //incrementa s3 para o terceiro processo poder avançar
				
				semWait(s2);   //espera que o primeiro processo incremente s2
				printf("melhor ");
				semPost(s3);  //incrementa s3 para o terceiro processo poder avançar
				
				exit(EXIT_SUCCESS);
			}
			if(i == 2){				
				semWait(s3);      //espera que o segundo processo incremente s3
				printf("Computadores - ");
				fflush(stdout);
				semPost(s1);     //incrementa s1 para o primeiro processo poder avançar
				
				semWait(s3);    //espera que o segundo processo incremente s3
				printf("disciplina!\n\n");
				
				exit(EXIT_SUCCESS);
			}
		}
	}
	
	for(i = 0; i < 3; i++){		//waits for all child processes before unlinking the 3 semaphores
		wait(&status);
	}
	
    if(sem_unlink("/sem1") < 0 || sem_unlink("/sem2") < 0 ||sem_unlink("/sem3") < 0){
		perror("Error sem_unlink.\n");
		exit(EXIT_FAILURE);
	} 
	
	return EXIT_SUCCESS;
}

/*
 * In multi-process or multi-thread code. The buffer doesn't always flush on a 
 * call to a printf(), so to know the true order of execution of multiple 
 * processes the fflush is used after every print.
 */


//Eu Raúl Coelho revi o exercício do meu colega.