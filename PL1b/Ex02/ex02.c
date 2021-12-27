/*
 * ex02.c
 *
 *  Created on: 25/03/2021
 *      Author: isep
 */
#include<stdio.h>
#include<unistd.h>
#include<signal.h>

int main() {
	printf("Enter PID: ");
	int pid;
	scanf("%d", &pid);
	printf("Enter Signal: ");
	int signal;
	scanf("%d", &signal);

	kill(pid,signal);

	return 0;
}

/*
 * Eu Jose Silva nº1190778 revi o exercicio do meu colega raul
 */

