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

void sig_handler(int signum){

	write(1,"\nTo slow, that is why the program is end!\n",44);
	exit(1);
}

int main (void) {

	struct sigaction sa;
	sa.sa_handler = &sig_handler;
	sa.sa_flags = SA_SIGINFO;
	sigaction(SIGALRM, &sa, NULL);
	alarm(10);

	char str[100];
	int total=0;
	printf("Please enter a string\n");
	scanf ("%s", str);

	alarm(0);

	while( str[total] != '\0'){
		total++;
	}

	printf("This string has %d caracters\n" ,total);

	sleep(20);

	printf("Sucessful excecution!\n");

	return 0;
}

/*
* Eu Raúl Coelho revi o trabalho do meu colega Miguel Silva
*/
