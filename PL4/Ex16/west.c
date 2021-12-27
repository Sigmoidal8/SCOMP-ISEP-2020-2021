/*
 * west.c
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

typedef struct
{
	int ocupado;
	int sentidoAtivo;
	int nrCarrosEast;
	int nrCarrosWest;
} shared_data;

int main(void)
{

	shared_data *bridge;
	int fd, r, data_size = sizeof(shared_data);
	int sentidoWest = 2;
	int i;

	fd = shm_open("/shmtest", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
	ftruncate(fd, data_size);

	bridge = (shared_data *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	sem_t *east;
	if ((east = sem_open("east", O_CREAT, 0644, 0)) == SEM_FAILED)
	{
		perror("Falha na abertura do semáforo");
		exit(1);
	}

	sem_t *west;
	if ((west = sem_open("west", O_CREAT, 0644, 0)) == SEM_FAILED)
	{
		perror("Falha na abertura do semáforo");
		exit(1);
	}

	sem_t *bridgeAccess;
	if ((bridgeAccess = sem_open("bridge", O_CREAT, 0644, 1)) == SEM_FAILED)
	{
		perror("Falha na abertura do semáforo");
		exit(1);
	}

	if (bridge->nrCarrosEast == 0 && bridge->nrCarrosWest == 0)
	{
		bridge->ocupado = 0;
		bridge->nrCarrosEast = 0;
		bridge->nrCarrosWest = 0;
		bridge->sentidoAtivo = sentidoWest;
	}

	bridge->nrCarrosWest++;
	if (bridge->sentidoAtivo != sentidoWest)
	{
		printf("Waiting, east is crossing the bridge!\n");
		sem_wait(west);
	}

	sem_wait(bridgeAccess);
	bridge->ocupado = 1;
	printf("Crossing the bridge from West to East\n");
	sleep(10);
	printf("I have crossed the bridge!\n");
	sem_post(bridgeAccess);

	bridge->ocupado = 0;
	bridge->nrCarrosWest--;

	if (bridge->nrCarrosWest == 0)
	{
		bridge->sentidoAtivo = 1;
		for (i = 0; i < bridge->nrCarrosEast; i++)
		{
			sem_post(east);
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

	if (sem_unlink("east") < 0 || sem_unlink("west") < 0 || sem_unlink("bridge") < 0)
	{
		perror("ERROR");
		exit(0);
	}

	return 0;
}
