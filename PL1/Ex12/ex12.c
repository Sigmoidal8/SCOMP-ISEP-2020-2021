/*
 * ex12.c
 *
 *  Created on: 18/03/2021
 *      Author: isep
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>

int spawn_childs(int n){
	pid_t pids[n];
	int i;
	for(i=0;i<n;i++){
		pids[i]=fork();
		if(pids[i]<0){
			perror("fork failed");
			exit(1);
		}
		if(pids[i]==0){
			return i+1;
		}
	}
	return 0;  // se for o pai retorna 0
}

int main(){
	int i, n=6, index;
	int status[n]; //saves the status of every process

	index = spawn_childs(n);

	if(index>0){
		exit(index*2);
	}

	for(i=0;i<n;i++){            //waits for every child and saves their status
		wait(&status[i]);
	}

	for(i=0;i<n;i++){            //Prints the exit status of every child
			if(WIFEXITED(status[i])){
					printf("Child number %d\n",WEXITSTATUS(status[i]));
			}
		}


		return 0;
	}

/*
 * Eu Raúl Coelho nº1190986 revi o exercicio do meu colega Miguel
 */
