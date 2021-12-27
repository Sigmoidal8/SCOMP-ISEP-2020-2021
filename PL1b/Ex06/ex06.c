/*
 * ex06.c
 *
 *  Created on: 01/04/2021
 *      Author: raulcoelho
 */
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

volatile sig_atomic_t USR1_COUNTER = 1;

void handle_signal(int signo)
{
	sigset_t mask;
	sigfillset(&mask);
	sigprocmask(SIG_BLOCK, &mask, 0);
    
	printf("SIGUSR1 signal captured, USR1_COUNTER = %d\n", USR1_COUNTER++);
	sleep(5);

	sigprocmask(SIG_UNBLOCK, &mask, 0);
}

int main(int argc, char *argv[])
{
	struct sigaction act;
	act.sa_handler = &handle_signal;
	act.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1, &act, NULL);

	pid_t pid, ppid;
	ppid = getpid();
	pid = fork();
	srand(time(NULL));
	if (pid == 0)
	{
		int i;
		kill(ppid, 10);
		for (i = 0; i < 12; i++)
		{
			int random_number = rand() % 16;
			kill(ppid, random_number);
			struct timespec tim, tim2;
			tim.tv_nsec = 10000000;
			tim.tv_sec = 0;
			nanosleep(&tim, &tim2);
		}
	}
	else
	{
		while (1)
		{
			printf("I'm working!\n");
			sleep(1);
		}
	}

	return 0;
}

/*
 * Eu Jose Silva nº1190778 revi o exercicio do meu colega raul
 */