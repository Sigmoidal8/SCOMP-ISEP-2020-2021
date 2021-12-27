/*
 * ex15.c
 *
 *  Created on: 15/05/2021
 *      Author: miguelsilva
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include <time.h>

#define NUM_FILHOS 10        // número de filhos
#define MAX_VISITANTES 5    // quantidade máxima de visitantes
#define WAITING_TIME 2      // tempo de espera
#define DURACAO_SHOW 3      // tempo que o show dura

int main() {
	int i;
	srand(time(NULL));

	sem_t *sem_show;            /* controla o início do espetáculo */
	sem_t *sem_visitantes;      /* controla a quantidade de lugares vagos na sala */
	sem_t *sem_show_entrada;    /* controla a entrada na sala */
	sem_t *sem_show_saida;      /* controla a saída da sala */

	/* criar semáforos */
	if((sem_show = sem_open("semaforo1", O_CREAT, 0644, 0)) == SEM_FAILED) {
		perror("Failed sem_open\n");
		return 1;
	}
	if((sem_visitantes = sem_open("semaforo2", O_CREAT, 0644, MAX_VISITANTES)) == SEM_FAILED) {
		perror("Failed sem_open\n");
		return 1;
	}
	if((sem_show_entrada = sem_open("semaforo3", O_CREAT, 0644, 1)) == SEM_FAILED) {
		perror("Failed sem_open\n");
		return 1;
	}
	if((sem_show_saida = sem_open("semaforo4", O_CREAT, 0644, 1)) == SEM_FAILED) {
		perror("Failed sem_open\n");
		return 1;
	}

	for(i = 0; i < NUM_FILHOS; i++) {
		pid_t pid = fork();
		if(pid == -1) {
			perror("Failed fork\n");
			exit(1);
		} else if(pid == 0) {
			while(1) {

				sem_wait(sem_show_entrada);
				sem_post(sem_show_entrada); /* sem_post para o próximo visitante entrar */
				printf("Visitante %d à espera\n", i + 1);

				if((i + 1) == MAX_VISITANTES) { /* se a sala estiver cheia o show comeaa */
					sem_post(sem_show);
				}

				sem_wait(sem_visitantes); /* visitante entra na sala */
				printf("Visitante %d entrou\n", i + 1);

				sem_wait(sem_show);
				sem_post(sem_show); /* sen_post  para o próximo visitante assistir */
				printf("Visitante %d a assistir\n", i + 1);

				sleep(DURACAO_SHOW); /* duracao do show */

				sem_wait(sem_show_saida);
				sem_post(sem_show_saida);
				sem_post(sem_visitantes); /* lugares vagos na sala aumenta */
				printf("Visitante %d saiu\n", i + 1);

				exit(0);
			}
		}
	}

	for(i = 0; i < NUM_FILHOS; i++)
		wait(NULL);

	/* fechar os semáforos */
	if(sem_unlink("semaforo1")<0||sem_unlink("semaforo2")<0||sem_unlink("semaforo3")<0||sem_unlink("semaforo4")<0){
		perror("ERROR");
		exit(0);
	}

	return 0;
}

//Eu Raúl Coelho revi o exercício do meu colega.