/*
 * ex12.c
 *
 *  Created on: 04/04/2021
 *      Author: miguelsilva
 */
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

volatile sig_atomic_t verification = 0;
volatile sig_atomic_t processes_ended = 0;

int simula1()
{
	sleep(2);
	return 1;
}

int simula2()
{
	sleep(2);
	return 1;
}

void handler_sigusr1(int sign)
{
	puts("Aqui1");
	verification = 1;
	processes_ended++;
}

void handler_sigusr2(int sign)
{
	puts("Aqui2");
	processes_ended++;
}

void handler_sigusr3(int sign)
{
	puts("Aqui3");
	int s2 = simula2();
	exit(1);
}

int main(int argc, char *argv[])
{
	struct sigaction act;
	memset(&act, 0, sizeof(struct sigaction));
	sigemptyset(&act.sa_mask);
	act.sa_handler = handler_sigusr1;
	act.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1, &act, NULL);

	struct sigaction act1;
	memset(&act1, 0, sizeof(struct sigaction));
	sigemptyset(&act1.sa_mask);
	act1.sa_handler = handler_sigusr2;
	act1.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR2, &act1, NULL);

	struct sigaction act2;
	memset(&act2, 0, sizeof(struct sigaction));
	sigemptyset(&act2.sa_mask);
	act2.sa_handler = handler_sigusr3;
	act2.sa_flags = SA_SIGINFO;
	sigaction(SIGQUIT, &act2, NULL);

	pid_t pid[50];
	int i, status;

	pid_t ppid = getpid();

	for (i = 0; i < 50; i++)
	{
		pid[i] = fork();
		if (pid[i] < 0)
		{
			perror("fork falhou");
			exit(1);
		}

		if (pid[i] == 0)
		{
			int s1 = simula1();
			if (s1 == 1)
			{
				kill(ppid, SIGUSR1);
			}
			else
			{
				kill(ppid, SIGUSR2);
			}
		}
	}

	while (processes_ended < 25)
	{
		sleep(1);
	}

	if (verification == 1)
	{
		for (i = 0; i < 50; i++)
		{
			kill(pid[i], SIGQUIT);
		}
		for (i = 0; i < 50; i++)
		{
			waitpid(pid[i], &status, 0);
		}
		printf("Efficient algorithm\n");
	}
	else
	{
		for (i = 0; i < 50; i++)
		{
			kill(pid[i], SIGKILL);
		}
		printf("Inefficient algorithm\n");
	}

	printf("Program ended\n");
	return 0;
}
