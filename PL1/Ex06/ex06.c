/*
 * ex06.c
 *
 *  Created on: 13/03/2021
 *      Author: isep
 */

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>


/*
 * Exercicio original
 */

void main(){
	int i;
	int status;
	for (i = 0; i < 4; i++) {
		if (fork() == 0) {
			sleep(1); /*sleep(): unistd.h*/
		}
	}

	printf("This is the end.\n").
}


/*
 * Alinea a)
 * (Um (PNG) encontra-se nesta mesma pasta com a resolucao)
 */

/*
 * Aline b)
 * basta substituir o 4 do ciclo for por um 2.
 */

void main(){
	int i;
	int status;
	for (i = 0; i < 4; i++) {
		if (fork() == 0) {
			sleep(1); /*sleep(): unistd.h*/
			exit(1);
		}
	}

	printf("This is the end.\n").
}

/*
 * Aline c) e d)
 */
void main(){
	int i;
	int status[4];
	pid_t pid[4];

	for (i = 0; i < 4; i++) {
		if (fork() == 0) {
			sleep(1); /*sleep(): unistd.h*/
			exit(i + 1);
		}
	}

	for(i = 0; i < (sizeof(pid)/4); i++){
		if(pid[i]%2==0){
			waitpid(pid[i],&status[i],0);
		}
	}
	printf("This is the end.\n").
}

 /*
  * Eu Raúl Coelho nº1190986 revi o exercicio do meu colega Miguel
 */ 
