/*
 * client.c
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

typedef struct
{
    int cliente;
    int bilhete;
} data_cliente;

int main()
{
    sem_t *semaforos[2];

    if ((semaforos[0] = sem_open("ex12Cliente1", O_CREAT, 0644,0)) == SEM_FAILED)
    {
        perror("Error at sem_open() cliente1!\n");
        exit(EXIT_FAILURE);
    }
    if ((semaforos[1] = sem_open("ex12Cliente2", O_CREAT, 0644,1)) == SEM_FAILED)
    {
        perror("Error at sem_open() cliente2!\n");
        exit(EXIT_FAILURE);
    }

    data_cliente* data;
    int fd;
	if((fd = shm_open("/ex12shm", O_CREAT|O_RDWR, S_IRUSR|S_IWUSR)) == -1){
		perror("shm_open error");
		exit(1);
	}

		/* TUNCATE SHARED MEMORY SIZE */
	if(ftruncate(fd,sizeof(data_cliente)) == -1){
		perror("ftruncate error");
		exit(1);
	}

	/* Map da shared memory */
	if((data = (data_cliente*) mmap(NULL, sizeof(data), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED){
		perror("mmap error");
		exit(1);
	}

    data->cliente++;
    sem_wait(semaforos[0]);
    printf("Bilhete: %d\n",data->bilhete);
    sem_post(semaforos[1]);

    return 0;
}