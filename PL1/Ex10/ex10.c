/*
 * ex10.c
 *
 *  Created on: 18/03/2021
 *      Author: isep
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

#define ARRAY_SIZE 2000

int main(){
	int numbers[ARRAY_SIZE];
	int n,i,j;
	int ocurrence=255; //valor por ommissao
	time_t t;	             //needed to randomize the array
	int status[10];
	pid_t pid[10];

    srand ((unsigned) time (&t));

 	for(i = 0 ;i < ARRAY_SIZE ; i++){
 		numbers[i] = (int) rand() % 200;
 	}

 	n = (int) rand()%200;

 	for(i = 0;i < 10;i++){
 		pid[i]=fork();
		 if(pid[i]<0){
			perror("fork failed");
			exit(1);
		}else if(pid[i]==0){
 			int k = i*200;
 			for(j = k;j< k + 200; j++){
 				if(numbers[j] == n){
 					ocurrence = j-k;
 					exit(ocurrence);
 				}
 			}
 			exit(ocurrence);
 		}
 	}


 	for(i = 0;i < 10; i++){
 			waitpid(pid[i],&status[i],0);
 	}

 	for(i = 0;i < 10;i++){
 		if(WIFEXITED(status[i])){	             //verifica se terminou corretamente o processo
 			if(WEXITSTATUS(status[i])==255){     // verifica se terminou com o valor por omissao
 				printf("No filho numero= %d nao ha o numero n= %d\n\n",i,n);
 			}else{
 				printf("No filho numero= %d o n esta no index %d\n\n",i,WEXITSTATUS(status[i]));
 			}
 		}
 	}
 		return 0;
}

/*
 * Eu Raúl Coelho nº1190986 revi o exercicio do meu colega Miguel
 */