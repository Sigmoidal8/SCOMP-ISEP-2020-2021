/*
 * ex07.c
 *
 *  Created on: 01/04/2021
 *      Author: raulcoelho
 */
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int signal_sent = 0;

void handle_signal(int sign)
{
	sigset_t waiting_mask;
	sigpending(&waiting_mask);

	int i;
	for (i = 1; i < 65; i++)
	{
		if (sigismember(&waiting_mask, i))
		{
			printf("O signal %d foi bloqueado\n", i);
		}else{
			printf("O signal %d não foi bloqueado\n", i);
		}
	}
	signal_sent = 1;
}

int main(int argc, char *argv[])
{
	/*
	sigset_t mask;
	sigfillset(&mask);
	sigdelset(&mask,SIGUSR1);
	*/
	sigprocmask(SIG_BLOCK, NULL, 0);

	struct sigaction act;
	act.sa_handler = &handle_signal;
	act.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1, &act, NULL);

	while (signal_sent == 0)
	{
		printf("Working\n");
		sleep(1);
	}
	return 1;
}

/*
 * Eu Jose Silva nº1190778 revi o exercicio do meu colega raul
 */