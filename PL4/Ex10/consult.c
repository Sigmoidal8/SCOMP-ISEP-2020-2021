/*
 * consult.c
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
		exit(EXIT_FAILURE);
	}

    int size = sizeof(SharedData);

    if(ftruncate(fd, size) == -1){
		perror("erro ftruncate\n");
		exit(0);
	}

    SharedData *sd = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(sd == NULL){
		printf("Erro mmap\n");
		exit(0);
	}

    sem_t *sem;
	
	sem = sem_open("/ex10Consult", O_CREAT, 0644, 1);
	if(sem == SEM_FAILED){
		printf("Erro criar semáforo\n");
		return 0;
	}

    sem_wait(sem);

    int number;
	printf("Insert your number:\n");
	scanf("%d", &number);

    if(sd->index == 0){
		printf("No records available.\n");
	}else{
        int i;
		for(i = 0; i < sd->index; i++){
			record *r = &(sd->entry[i]);
			if(r->number == number){
				printf("Record: %d\nNumber: %d  Name: %s  Address: %s\n", i+1, r->number, r->name, r->address);
			}
		}
    }

    sem_post(sem);

		if (sem_unlink("ex10Consult") < 0 )
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