/*
 * ex07.c
 *
 *  Created on: 13/03/2021
 *      Author: isep
 */


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#define ARRAY_SIZE 1000
int main (){
	int numbers[ARRAY_SIZE];	/* array to lookup */
	int n;			            /* the number to find */
	time_t t;			        /* needed to initialize random number generator (RNG) */
	int i;
	pid_t pid;
	int childAux = 0;
	int parentAux = 0;
	int status;

	srand ((unsigned) time (&t));     /* intializes RNG (srand():stdlib.h; time(): time.h) */

    for (i = 0; i < ARRAY_SIZE; i++){  /* initialize array with random numbers (rand(): stdlib.h) */
      numbers[i] = rand () % 50;
    }

    n = rand () % 50;  /* initialize n */

    pid = fork();

    if(pid < 0){
    	perror("O Fork falhoul\n");
    	exit(-1);
    }

    if(pid == 0){          // O filho procura na primeira metade do array
    	for(i = 0; i<ARRAY_SIZE/2; i++){
    		if(numbers[i] == n){
    			childAux++;
   			}
    	}
    	printf("ChildAux= %d\n", childAux);
    	exit(childAux);
    }

    for (i = 500; i<1000; i++){ // O pai procura na segunda metade do array
    	if(numbers[i] == n){
    		parentAux++;
    	}
    }

    printf("ParentAux = %d\n", parentAux);

    waitpid(pid, &status, 0);

    if(WIFEXITED(status)){
    	int total = WEXITSTATUS(status) + parentAux;
     	printf("Total de vezes que n apareceu no array %d\n", total);
    }

    return 0;
}

/*
 * Eu Raúl Coelho nº1190986 revi o exercicio do meu colega Miguel
 */
