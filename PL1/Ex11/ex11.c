/*
 * ex08.c
 *
 *  Created on: 15/03/2021
 *      Author: isep
 */
#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include<time.h>
#include<sys/wait.h>

#define ARRAY_SIZE 1000

int main(){
	int numbers[ARRAY_SIZE], results[ARRAY_SIZE];
	int maior=0, i, j;
	time_t t;
	int status[5];
	pid_t pid[5];

	srand ((unsigned) time (&t));

	for(i = 0; i < ARRAY_SIZE; i++){
		numbers[i] =  rand() % 255;
	}

	for(i = 0; i < 5; i++){
		pid[i] = fork();
		if(pid[i]<0){
			perror("fork failed");
			exit(1);
		}else if(pid[i] == 0){
			int inicio = i*200;
			int end = (i + 1) * 200; 		
			for(j = inicio; j < end; j++){
				if(numbers[j] > maior){
					maior = numbers[j];
				}
			}
			exit(maior);
		}
	}

	for(i = 0; i < 5; i++){
		waitpid(pid[i], &status[i] ,0);
	}
	for(i = 0; i < 5; i++){
		if(WIFEXITED(status[i])){
			if(WEXITSTATUS(status[i]) > maior){
				maior = WEXITSTATUS(status[i]);
			}
		}
	}

	pid_t p = fork();

	if(p<0){
			perror("fork failed");
			exit(1);
	}else if(p == 0){
		for(i = ARRAY_SIZE/2; i< ARRAY_SIZE; i++){
			results[i] = ((int) numbers[i] / maior) * 100;
			printf("Index %d: %d\n",i,results[i]);
		}
		exit(1);
	}else{
		for(i = 0; i< ARRAY_SIZE/2; i++){          //calcula os resultados
			results[i] = ((int) numbers[i] / maior) * 100;
			printf("Index %d: %d\n", i, results[i]);
		}		
	}

	wait(NULL);
	return 0;
}

/*
 * Eu Jose Silva nº1190778 revi o exercicio do meu colega Raul.
 */




