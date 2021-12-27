/*
 * ex11.c
 *
 *  Created on: 15/05/2021
 *      Author: miguelsilva
 */


#include <errno.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define DOORS 3
#define SIZE 200

void down(sem_t *sem) {
    if (sem_wait(sem) == -1) {
        perror("Error at sem_wait().");
        exit(0);
    }
}

void up(sem_t *sem) {
    if (sem_post(sem) == -1) {
        perror("Error at sem_post().");
        exit(0);
    }
}

void close_semaphore (sem_t *sem) {
    //Fecha o semáforo
    if (sem_close(sem) == -1) {
        perror("Error at sem_close().\n");
        exit(0);
    }
}

void unlink_semaphore(char *s) {
    //Apaga o semáforo
    if (sem_unlink(s) == -1) {
        perror("Error at sem_unlink().\n");
        printf("Error: %s\n", strerror(errno));
        exit(0);
    }
}

int createPersons(int num){
	int persons;
	time_t t;
	srand((unsigned) time(&t));
	persons = rand() % num;
	return persons;
}

typedef struct {
	int emptySeats;
	int filledSeats;
	int inside;
} shared_data;

void personsEntering(shared_data *s){
	int numPersons;
	if(s->inside == 0){
		numPersons = createPersons(SIZE);
		s->emptySeats -= numPersons;
		s->filledSeats += numPersons;
		s->inside = 1;
	}else{
		numPersons = createPersons(s->emptySeats);
		s->emptySeats -= numPersons;
		s->filledSeats += numPersons;
	}
	printf("(Entering %d persons)\n", numPersons);
	sleep(1);
}

void personsLeaving(shared_data *s){
	int numPersons;
	if(s->inside == 1){
		numPersons = createPersons(s->filledSeats);
		s->emptySeats += numPersons;
		s->filledSeats -= numPersons;
		if(s->emptySeats == 0){
			s->inside = 0;
		}
	}else{
		return;
	}
	printf("(Leaving %d persons)\n", numPersons);
	sleep(1);
}

int main() {

	pid_t pid[DOORS];
	shared_data *train;
	int i, fd, data_size = sizeof(shared_data);
    char s1[SIZE] = "firstDoor";
    char s2[SIZE] = "secondDoor";
    char s3[SIZE] = "thirdDoor";

    // Cria e verifica se a memória partilhada foi devidamente aberta 
	// permissões de read, write, execute/search para user, grupo e owner 
	if((fd = shm_open("/shm_11", O_CREAT | O_RDWR | O_TRUNC, S_IRWXU | S_IRWXG | S_IRWXO)) <0) 
		perror("Failed to create shared memory");

	// Ajusta o tamanho da memória partilhada
	if(ftruncate(fd, data_size)<0)
        perror("Failed to adjust memory size");

	// Mapea a memória partilhada 
	train = (shared_data *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	// Verifica se a memória partilhada foi devidamente mapeada 
    if (train == NULL) {
        perror("No mmap()");
        exit(0);
    }
	
	sem_t *sem[3];
	if ((sem[0] = sem_open(s1, O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED) {
		perror("Erro a criar semáforo");
		exit(1);
	}
    if ((sem[1] = sem_open(s2, O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED) {
		perror("Erro a criar semáforo");
		exit(1);
	}
    if ((sem[2] = sem_open(s3, O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED) {
		perror("Erro a criar semáforo");
		exit(1);
	}

	train->inside = 0;
	
	for(i = 0; i < DOORS; i++) {
		pid[i] = fork();
		if (pid[i] < 0) {

            perror("Fork failed");
            exit(EXIT_FAILURE);

        } else if(pid[i] == 0) {

			down(sem[0]);
			printf("Leaving from first door...\n");
			personsLeaving(train);
			up(sem[0]);
			
			down(sem[1]);
			printf("Leaving from second door...\n");
			personsLeaving(train);
			up(sem[1]);
			
			down(sem[2]);
			printf("Leaving from third door...\n");
			personsLeaving(train);
			up(sem[2]);
			
			exit(EXIT_SUCCESS);
            
		} else {

			down(sem[0]);
			printf("Entering from first door...\n");
			personsEntering(train);
			up(sem[0]);
			
			down(sem[1]);
			printf("Entering from second door...\n");
			personsEntering(train);
			up(sem[1]);
			
			down(sem[2]);
			printf("Entering from third door...\n");
			personsEntering(train);
			up(sem[2]);
		}
	}

    //Desfaz o mapeamento 
	if(munmap(train, data_size)<0){ 
        perror("No munmap()");
        exit(0);
    }

	//Fecha o descritor 
	if(close(fd)<0){ 
        perror("No close()");
        exit(0);
    }

	//Apaga a memoria partilhada do sistema 
    if(shm_unlink("/shm_11")<0){ 
        perror("No unlink()");
        exit(1);
    } 

	close_semaphore(sem[0]);
    unlink_semaphore(s1);
    close_semaphore(sem[1]);
    unlink_semaphore(s2);
    close_semaphore(sem[2]);
    unlink_semaphore(s3);
	
	return 0;
}

//Eu Raúl Coelho revi o exercício do meu colega.