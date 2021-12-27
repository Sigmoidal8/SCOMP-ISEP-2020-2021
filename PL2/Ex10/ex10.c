/*
 * ex10.c
 *
 *  Created on: 06/04/2021
 *      Author: miguelsilva
 */

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <time.h>

int main(void){
	int fd1[2], fd2[2], credit = 20, num = 1, betChild, betParent;

	printf("\nCredito Inicial: %d\n\n", credit);

	if (pipe(fd1) == -1){
		perror("Pipe 1 failed\n");
		return 1;
	}

	if (pipe(fd2) == -1){
		perror("Pipe 2 failed\n");
		return 1;
	}

	pid_t pid = fork();

	if (pid == -1){
		printf("Fork Failed!");
		exit(1);
	}
	else if (pid == 0){
		srand((unsigned) time(NULL) * getpid());
		close(fd1[1]);
		close(fd2[0]);
		while(credit > 0){
			read(fd1[0], &num, sizeof(int));
			if(num == 1){
				betChild = 1 + rand() % 5; 
				write(fd2[1], &betChild, sizeof(int));
				read(fd1[0], &credit, sizeof(int));
				printf("Credito atual: %d\n", credit);
			} else {
				close(fd1[0]);
				close(fd2[1]);
				exit(0);
			}
		}
	}
	else{
		srand((unsigned)time(NULL));
		close(fd1[0]);
		close(fd2[1]);
		printf("A começar o jogo..Aguarde um momento:\n\n");
		while (credit > 0){
			betParent = 1 + rand() % 5;
			if (credit > 0){
				num = 1;
			}else{
				num = 0;
			}

			write(fd1[1], &num, sizeof(int));
			read(fd2[0], &betChild, sizeof(int));

			printf("Aposta Filho: %d; Resultado Pai: %d\n", betChild, betParent);
			
			if (betChild == betParent){
				credit = credit + 10; 
			}else{
				credit = credit - 5; 
			}
			write(fd1[1], &credit, sizeof(int));
		}
		close(fd1[1]);
		close(fd2[0]);
	}
return 0;
}

/*
* Eu Raúl Coelho revi o trabalho do meu colega Miguel Silva
*/