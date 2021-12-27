/*
 * ex10.c
 *
 *  Created on: 02/04/2021
 *      Author: miguelsilva
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <string.h>

char str[100];

void sig_handler(int signum){

	if (str[0] == '\0'){
		write(1,"\nTo slow, that is why the program is end!\n",44);
		exit(1);
	}
}

int main (void) {

	pid_t pid;
	int total=0;

	struct sigaction sa;
	sa.sa_handler = &sig_handler;
	sa.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1, &sa, NULL);

	pid_t fatherpid = getpid();

	pid = fork();

	if(pid < 0){
		perror("Fork Failed");
		exit(1);
	}else if(pid > 0){
		printf("Please enter a string\n");
		scanf ("%s", str);

		while( str[total] != '\0'){
			total++;
		}
	}else{
		sleep(10);
		kill(fatherpid, SIGUSR1);
		exit(1);
	}

	printf("This string has %d caracters\n" ,total);

	sleep(20);

	printf("Sucessful excecution!\n");

	return 0;
}

/*
* Eu Raúl Coelho revi o trabalho do meu colega Miguel Silva
*/
