/*
 * ex12.c
 *
 *  Created on: 06/04/2021
 *      Author: miguelsilva
 */

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <time.h>

#define NUM_PRODUTOS 5

typedef struct{
	int cod_barras;
	float preco;
	char name[50];
} database;
	
int main(void){
	int i,status;																	
	int fd[2];
	int fd2[2];
	pid_t pid[5];
	int produtoEscolhido;
	time_t t;

	if (pipe(fd) == -1)	{
		perror("Pipe failed");
		return 1;
	}

	if (pipe(fd2) == -1)	{
		perror("Pipe failed");
		return 1;
	}
	
	for(i=0;i<5;i++){
		pid[i]=fork();
		if (pid[i] == -1){
			printf("Fork Failed!\n");
			exit(1);
		}
		if(pid[i]==0){
			close(fd[1]);
			close(fd2[0]);

			srand((unsigned)time(&t) ^ getpid());
    		produtoEscolhido = rand() % 5 + 1;
			printf("Pesquisa pelo produto com codigo de barras: %d\n",produtoEscolhido);	

			write(fd2[1], &produtoEscolhido, sizeof(int));

			database produtoRecebido[1];							      //Espaço reservado para 1 produto
			read(fd[0],&produtoRecebido[0],sizeof(database)); 
			
			printf("Nome do produto: %s\nPreço do produto: %.2f\n\n",produtoRecebido[0].name,produtoRecebido[0].preco);
			close(fd[0]);
			close(fd2[1]);											
			exit(0);	
		}
	}

//inicializacao da estrutura
	database produtos[NUM_PRODUTOS];
	produtos[0].cod_barras = 1;
	produtos[1].cod_barras = 2;
	produtos[2].cod_barras = 3;
	produtos[3].cod_barras = 4;
	produtos[4].cod_barras = 5;
	produtos[0].preco = 5;
	produtos[1].preco = 10;
	produtos[2].preco = 15;
	produtos[3].preco = 20;
	produtos[4].preco = 25;
	strcpy(produtos[0].name, "Arroz");
	strcpy(produtos[1].name, "Fiambre");
	strcpy(produtos[2].name, "Atum");
	strcpy(produtos[3].name, "Bebida");
	strcpy(produtos[4].name, "Queijo");	

	int j,k;
	for(k = 0; k < 5; k++){
		close(fd[0]);
		close(fd2[1]);

		int cod_barras;

		read(fd2[0], &cod_barras, sizeof(int));
											
		for(j=0;j<NUM_PRODUTOS;j++){										
			if(cod_barras==produtos[j].cod_barras){				
				write(fd[1],&produtos[j],sizeof(database));	
			}
		}
	}	
		close(fd[1]);	
		for(i = 0; i < NUM_PRODUTOS; i++) {	
	 	waitpid(pid[i], &status, 0);		 
	}								
	return 0;
}

/*
 * Eu Raul Coelho revi o exercicio do meu colega Miguel Silva
 */