/*
 * reader.c
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

typedef struct {
	int nrLeitores;
    int nrEscritores;
    char frase[50];
} shared_data_type;

int main(void) {
    sem_t *nrEscritores;
    sem_t *podeLer;
    sem_t *podeEscrever;
    int fd, r, data_size = sizeof(shared_data_type);
	shared_data_type *shared_data;
	fd = shm_open("/ex14", O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);
	if (fd == -1){
		printf("Erro na criação da memória partilhada!");
		return 1;
	}
	if (ftruncate (fd, data_size) == -1){
        printf("Erro no ftruncate!");
        return 1;
    }
	shared_data = (shared_data_type*)mmap(NULL, data_size, PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);

    if((nrEscritores = sem_open("ex14sem1", O_CREAT)) == SEM_FAILED){
        printf("erro1");
    }
    if((podeLer = sem_open("ex14sem2", O_CREAT)) == SEM_FAILED){
        printf("erro2");
    }
    if((podeEscrever = sem_open("ex14sem3", O_CREAT)) == SEM_FAILED){
        printf("erro3");
    }

    sem_wait(nrEscritores);
    shared_data->nrLeitores++;
    sem_post(nrEscritores);

    sem_wait(podeLer);
    printf("%s", shared_data->frase);
    printf("Numero de readers: %d\n", shared_data->nrLeitores);
    sleep(5);
    sem_wait(nrEscritores);
    shared_data->nrLeitores--;
    if (shared_data->nrEscritores > 0){
        sem_post(podeEscrever);
    }else{
        sem_post(podeLer);
    }
    sem_post(nrEscritores);

    r = munmap(shared_data, data_size);
    if (r == -1){
       printf("Error  munmap()!\n");
        exit(EXIT_FAILURE);
    }
    r = close(fd);
    if (r == -1){
        printf("Error  close()!\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}