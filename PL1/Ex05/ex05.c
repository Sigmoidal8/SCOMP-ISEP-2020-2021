/*
 * ex05.c
 *
 *  Created on: 13/03/2021
 *      Author: isep
 */

#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
	int status, i;
	pid_t p[2];

	for (i = 0; i < 2; i++)
	{
		p[i] = fork();

		if (p < 0)
		{
			perror("fork failed");
			exit(1);
		}
		else if (p == 0)
		{
			{
				sleep(i + 1);
				exit(i + 1);
			}
			else if (p[i] < 0)
			{
				perror("fork failed");
				exit(0);
			}
		}
		waitpid(p[0], &status, 0); //Zero para esperar que o primeiro processo filho acabe
		printf("Valor do primeiro filho: %d\n", WEXITSTATUS(status));
		waitpid(p[1], &status, 0); //Zero para esperar que o segundo filho acabe
		printf("Valor do segundo filho: %d\n", WEXITSTATUS(status));

		return 0;
	}

	/*
 * Eu Raúl Coelho nº1190986 revi o exercicio do meu colega Miguel
 */
