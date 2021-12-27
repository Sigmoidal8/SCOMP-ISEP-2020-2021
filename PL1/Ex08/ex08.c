/*
 * ex08.c
 *
 *  Created on: 15/03/2021
 *      Author: isep
 */
#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>

int main(){
	pid_t p;
	if(fork() == 0){
		printf("PID = %d\n", getpid());exit(0);
	}

	if((p = fork()) == 0){
		printf("PID = %d\n", getpid());
	}

	printf("Parent PID = %d\n", getpid());

	printf("Waiting... (for PID=%d)\n",p);
	waitpid(p,NULL,0);

	printf("Enter Loop...\n");
	while(1);
	return 0;
}
/*
 * a) they are all listed because a loop is initiated and so
 * no process is terminated.
 *
 * b) ao listar os processos o filho tem a tag defunt pois esta no estado zombie pois terminou antes do pai
 *
 * c) ao matar o pai os filhos tambem morrem
 *
 */

/*
 * Eu Jose Silva n1190778 revi o exercicio do meu colega raul
 */
