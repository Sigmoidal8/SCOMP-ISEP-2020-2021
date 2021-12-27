/*
 * insert.c
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
#include "data.h"

int main(){
    int fd = shm_open("/ex10", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
	if(fd == -1){
		perror("Erro a abrir memória partilhada\n");
		exit(0);
	}

	int size = sizeof(SharedData);
	if(ftruncate(fd, size) == -1){
		perror("Erro ftruncate\n");
		exit(0);
	}

	SharedData *sd = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if(sd == NULL){
		printf("Erro no mmap\n");
		exit(0);
	}
	
	sem_t *sem;
	sem = sem_open("/ex10Insert", O_CREAT, 0644, 1);
	if(sem == SEM_FAILED){
		printf("Erro no sem_open\n");
		return 0;
	}

	sem_wait(sem);

    record *r = &(sd->entry[sd->index]);

	printf("Insert number:\n");
	scanf("%d", &(r->number));

	printf("Insert name:\n");
	char aux[50];
	scanf("%s", aux);
	strcpy(r->name, aux);

	printf("Insert address:\n");
    char aux2[100];
	scanf("%s", aux2);
	strcpy(r->address, aux2);

    sd->index++;
	sem_post(sem);

		if (sem_unlink("ex10Insert") < 0 )
	{
		printf("Error  unlink()!\n");
		exit(EXIT_FAILURE);
	}

	if (munmap(sd, size))
	{
		printf("erro no munmap");
		return -1;
	}

	if (close(fd) < 0)
	{
		printf("Error  close()!\n");
		exit(EXIT_FAILURE);
	}

	return 0;
}

//Eu Miguel Silva, 1190778, revi o exercício do meu colega Raúl.