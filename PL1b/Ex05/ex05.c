

#include<stdio.h>
#include<unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>

void handle_SIGQUIT(int signo, siginfo_t *sinfo, void *context) {
    write(1,"I won’t let the process end with CTRL-\\!\n",43);
}

int main(int argc, char *argv[])  {
	struct sigaction act;
	memset (&act, 0, sizeof(struct sigaction));
	sigemptyset(&act.sa_mask);
	act.sa_sigaction = handle_SIGQUIT;
	act.sa_flags = SA_SIGINFO;
	sigaction(SIGQUIT, &act, NULL);
	while(1);
}


/*
* Alinea A)
* O CTRL-C termina a execução do programa
*
* Alinea C)
*  O CTRL-\ termina a execução do programa dando quit
*
* Alinea E)
*  O processo termina a sua execução com o sigint mas com o sigquit aparece a mensagem definida em cima.
*
* Alinea F)
*  Elimina o primeiro job e nao o primeiro processo
*
* Alinea G)
*  Usa-se write em vez de printf porque o write serve para uma sequencia ja ordenada de bytes que e o nosso caso.
*/

/*
* Eu Raúl Coelho revi o trabalho do meu colega 
* Miguel Silva
*/

