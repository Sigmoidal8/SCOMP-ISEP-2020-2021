/*
 * ex09.c
 *
 *  Created on: 18/03/2021
 *      Author: isep
 */


#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main(){
	int i, j, status;
	pid_t pid[10];

	for(i=0;i<10;i++){
		pid[i]=fork();
		if(pid[i]<0){
			perror("fork failed");
			exit(1);
		}else if(pid[i]==0){
			int k = (i*100)+1;
			for(j= k; j< k + 100; j++){
				printf("%d;\n",j);
			}
			exit(0);
		}
	}

	for(i=0;i<10;i++){
		waitpid(pid[i],&status,0);
	}

	return 0;
}

/*
 * Alinea a)
 *
 * O output não está ordenado, visto que todos os processos filhos estão a concorrer pelo
 * processador. Para que fosse ordenado teríamos que executar um filho sempre que um processo
 * filho anterior terminasse. Dessa forma ia correr um processo de cada vez, fazendo com que a
 * numeração fosse do 1 ao 1000 por ordem crescente.
 */

/*
 * Eu Raúl Coelho nº1190986 revi o exercicio do meu colega Miguel
 */

