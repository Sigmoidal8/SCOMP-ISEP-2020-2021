/*
* Alinea A)
* 
* A função signal () não bloqueia (necessariamente) a chegada de outros sinais enquanto o manipulador atual está em execução; 
* Sigaction () pode bloquear outros sinais até que o manipulador atual retorne. Daí escolher a função signal.
*
* Alinea B)
*
* sigaction ( int sig, const struct sigaction * act, struct sigaction * oact )
* act sets the action details for signal sig, and oact (if not NULL) is used to store the 
* previously set action details
*/


void handle_USR1(int signo){

    write(STDOUT_FILENO, “\nCatch USR1 !\n”, 13);
}

int main(int argc, char *argv[]){

    struct sigaction act;

    memset(&act, 0, sizeof(struct sigaction));

    sigemptyset(&act.sa_mask); /* No signals blocked */
    act.sa_handler = handle_USR1;
    sigaction(SIGUSR1, &act, NULL);
}

/*
* Alinea E)
* SIG_DFL - Default action
* SIG_IGN - para ignorar o sinal
* void (sa_handler) (int) 
*/

/*
* Eu Raúl Coelho verifiquei o trabalho do meu colega
* Miguel Silva
*/