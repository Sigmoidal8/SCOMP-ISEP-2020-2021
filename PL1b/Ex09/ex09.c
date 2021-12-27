/*
 * ex10.c
 *
 *  Created on: 02/04/2021
 *      Author: miguelsilva
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

sig_atomic_t verification = 0;

void sig_handler(int signal) {

	if (signal == SIGUSR1) {
		verification = 1;
	}
}

int main(void) {

	struct sigaction sa;
	sa.sa_handler = &sig_handler;
	sa.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1, &sa, NULL);

	srand(time(NULL));
	pid_t pid =fork();

	if(pid < 0){
		perror("Fork Failed");
		exit(0);
	}else if(pid > 0){
		sleep(3);
		printf("O processo pai demorou 3 segundos a executar a tarefa A\n");
		kill(pid,SIGUSR1);
	}else{
		int random = rand () % 5 + 1;
		sleep(random);
		while(!verification){

		}
		printf("O processo filho esta a executar a tarefa C\n");
	}
	return 0;
}

/*
* Eu Raúl Coelho revi o trabalho do meu colega Miguel Silva
*/
