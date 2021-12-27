/*
 * east.c
 *
 *  Created on: 15/05/2021
 *      Author: raulcoelho
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <string.h>
#include <semaphore.h>

typedef struct {
    int ocupado;
	int sentidoAtivo;
    int nrCarrosEast;
    int nrCarrosWest;
} shared_data;

int main(void) {
	
	shared_data *bridge;
	int fd, r, data_size = sizeof(shared_data);
	int sentidoEast = 1;
	int i;
	
	fd = shm_open("/shmtest", O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);
	ftruncate(fd, data_size);
	
	bridge = (shared_data*) mmap(NULL, data_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    
    sem_t *eastCanCross;
	if ((eastCanCross = sem_open("east", O_CREAT, 0644, 0)) == SEM_FAILED){
        perror("Falha na abertura do semáforo");
        exit(1);
    }
    
    sem_t *westCanCross;
	if ((westCanCross = sem_open("west", O_CREAT, 0644, 0)) == SEM_FAILED){
        perror("Falha na abertura do semáforo");
        exit(1);
    }
    
    sem_t *bridgeAccess;
	if ((bridgeAccess = sem_open("bridge", O_CREAT, 0644, 1)) == SEM_FAILED){
        perror("Falha na abertura do semáforo");
        exit(1);
    }
    
    if(bridge->nrCarrosEast == 0 && bridge->nrCarrosWest == 0) {
		bridge->ocupado = 0;
		bridge->nrCarrosEast = 0;
		bridge->nrCarrosWest = 0;
		bridge->sentidoAtivo = sentidoEast;
	}
    
    bridge->nrCarrosEast++;
    if(bridge->sentidoAtivo != sentidoEast) {
		printf("Waiting...west is crossing!\n");
		sem_wait(eastCanCross);
	}
	
	sem_wait(bridgeAccess);
	bridge->ocupado = 1;
	printf("Crossing the bridge from East to West\n");
	sleep(10);
	printf("I have crossed the bridge!\n");
	sem_post(bridgeAccess);

	bridge->ocupado = 0;
	bridge->nrCarrosEast--;
	
    if(bridge->nrCarrosEast == 0) {
		bridge->sentidoAtivo = 2;
		for(i=0;i<bridge->nrCarrosWest;i++) {
			sem_post(westCanCross);
		}
	}
    
	if (munmap(bridge, data_size))
	{
		printf("erro no munmap");
		return -1;
	}

	if (close(fd) < 0)
	{
		printf("Error  close()!\n");
		exit(EXIT_FAILURE);
	}

	if(sem_unlink("east")<0 || sem_unlink("west")<0 || sem_unlink("bridge")<0 ){
		perror("ERROR");
		exit(0);
	}

	return 0;
}
