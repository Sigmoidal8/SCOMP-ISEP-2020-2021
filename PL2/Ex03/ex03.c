/*
 * ex03.c
 *
 *  Created on: 05/04/2021
 *      Author: miguelsilva
 */

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>

#define BUFFER_SIZE 20

int main(void) {
	char message1[BUFFER_SIZE] = "Hello World";
	char message2[BUFFER_SIZE] = "Goodbye";
	char read_msg1[BUFFER_SIZE];
	char read_msg2[BUFFER_SIZE];
	int fd[2], status;

	if (pipe(fd) == -1) {
		perror("Pipe failed\n");
		return 1;
	}
	pid_t pid, childPid;
	pid = fork();

	if (pid > 0) {
		close(fd[0]);
		write(fd[1], message1, strlen(message1) + 1);
		sleep(1);
		write(fd[1], message2, strlen(message2) + 1);
		close(fd[1]);
		childPid = wait(&status);
		printf("\nChild PID - %d \nExited with value %d.\n", childPid,
				WEXITSTATUS(status));

	} else if (pid == 0) {
		close(fd[1]);
		read(fd[0], read_msg1, BUFFER_SIZE);
		read(fd[0], read_msg2, BUFFER_SIZE);
		printf("My parent pipe message: %s\n", read_msg1);
		printf("My parent pipe message: %s\n", read_msg2);
		close(fd[0]);
		exit(1);
	} else {
		perror("fork failed");
		exit(1);
	}
	return 0;
}

/*
* Eu Raúl Coelho revi o trabalho do meu colega Miguel Silva
*/