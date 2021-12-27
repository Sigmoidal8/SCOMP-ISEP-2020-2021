/*
 * ex04.c
 *
 *  Created on: 05/04/2021
 *      Author: miguelsilva
 */

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>

#define BUFFER_SIZE 80

int main(void) {
	int fd[2];
	FILE *ficheiro;
	int tam = 0;
	char message[256];

	if (pipe(fd) == -1) {
		perror("Pipe failed\n");
		return 1;
	}

	pid_t pid = fork();

	if (pid > 0) {
		close(fd[0]);
		ficheiro = fopen("ex04.txt", "r");
		if (ficheiro == NULL) {
			exit(EXIT_FAILURE);
		}
		while ((tam = fread(message, sizeof(char), 1, ficheiro)) > 0) {
			write(fd[1], message, tam);
		}
		fclose(ficheiro);
		close(fd[1]);
		wait(0);
	} else if (pid == 0) {
		close(fd[1]);
		char data[80];
		while (read(fd[0], data, sizeof(data))) {
			printf("%s\n", data);
		}
		close(fd[0]);
		exit(1);
	} else {
		perror("fork failed");
		exit(1);
	}
	return 0;
}

/*
* Eu Raúl Coelho verifiquei o trabalho do meu
* colega Miguel Silva
*/
