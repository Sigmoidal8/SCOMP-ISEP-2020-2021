/*
 * ex13.c
 *
 *  Created on: 01/05/2021
 *      Author: miguelsilva
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
#include <time.h>

#define BUFFER 10
#define OPERATIONS 30

typedef struct{
	int occurences[10];
	char word[10][255];
	char path[10][100];
} shared_memory;



int main(void) {
	
	int fd, data_size = sizeof(shared_memory);
    shared_memory *memoria;
    	
	shm_unlink ("/ex13");
	
	fd = shm_open("/ex13", O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
    if (fd == -1)
    {
        perror("Failed shm writer!\n");
        exit(EXIT_FAILURE);
    }

    int truncate = ftruncate(fd, data_size);
    if (truncate == -1)
    {
        perror("Error ftruncate!\n");
        exit(EXIT_FAILURE);
    }

	memoria = (shared_memory*)mmap(NULL, data_size, PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    if (memoria == MAP_FAILED)
    {
        perror("Error mmap!\n");
        exit(EXIT_FAILURE);
    }

	FILE *f;
	char line[1024];
    pid_t pid;
    int i;
	
	//Definição das palavras a serem procuradas
	strcpy(memoria -> word[0], "porto");
	strcpy(memoria -> word[1], "meu");
	strcpy(memoria -> word[2], "20");
	strcpy(memoria -> word[3], "santo tirso");
	strcpy(memoria -> word[4], "Isep");
	strcpy(memoria -> word[5], "miguel");
	strcpy(memoria -> word[6], "Sou");
	strcpy(memoria -> word[7], "Estudo");
	strcpy(memoria -> word[8], "dias");
	strcpy(memoria -> word[9], "de");
	
		
	for(i = 0; i < 10; i++){
		sprintf(memoria -> path[i], "filho%d.txt", i);
		
		pid = fork();
		if(pid == -1){
			perror("Erro no fork!\n");
			exit(EXIT_FAILURE);
		}
		if(pid == 0){
			f = fopen(memoria -> path[i], "r");
			while(fgets(line, sizeof(line), f) != NULL){
				if(strstr(line, memoria -> word[i]) != NULL){
					memoria -> occurences[i]++;
				}
			}
			fclose(f);
			exit(0);
		}
	}
	
	while(wait(NULL) > 0);
	
	for(i = 0; i < 10; i++){
		printf("O filho %d encontrou a palavra '%s' %d vezes no ficheiro %s.\n", i, memoria -> word[i], memoria -> occurences[i], memoria -> path[i]);
	}

	if(close(fd) < 0){
		perror("Erro");
		exit(EXIT_FAILURE);
	}
	
	if(munmap(memoria, data_size) == -1){
		perror("Erro no munmap!\n");
		exit(EXIT_FAILURE);
	}
		
	if(shm_unlink("/ex13") < 0){
		perror("Erro");
		exit(EXIT_FAILURE);
	}	
	
	return 0;
}

//Eu Raúl Coelho revi o exercício do meu colega Miguel