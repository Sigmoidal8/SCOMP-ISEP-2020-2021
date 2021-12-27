/*
 * consult_all.c
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

int main()
{
	int fd = shm_open("/ex10", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
	if (fd == -1)
	{
		perror("Erro abrir memoria\n");
		exit(0);
	}

	int size = sizeof(SharedData);
	if (ftruncate(fd, size) == -1)
	{
		perror("erro ftruncate\n");
		exit(0);
	}

	SharedData *sd = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (sd == NULL)
	{
		printf("Erro no mmap\n");
		exit(0);
	}

	sem_t *sem;
	sem = sem_open("/ex10_Consultall", O_CREAT, 0644, 1);
	if (sem == SEM_FAILED)
	{
		printf("Erro a criar semáforo\n");
		return 0;
	}

	sem_wait(sem);

	if (sd->index == 0)
	{
		printf("No records available.\n");
	}
	else
	{
		int i;
		for (i = 0; i < sd->index; i++)
		{
			record *r = &(sd->entry[i]);
			printf("Record %d:\nNumber: %d Name: %s Address: %s\n\n", i + 1, r->number, r->name, r->address);
		}
	}

	sem_post(sem);

	if (sem_unlink("ex10_Consultall") < 0 )
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