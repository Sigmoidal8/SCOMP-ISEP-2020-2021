/*
 * ex11.c
 *
 *  Created on: 02/04/2021
 *      Author: raulcoelho
 */
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
void handle_alarm(int sign){
    write(STDOUT_FILENO,"The command 1 didn't end in its allowed time!\n",47);
    exit(0);
}

int main(int arg, char *argv[]){
    struct sigaction sa;
    sa.sa_handler = &handle_alarm;
    sa.sa_flags = SA_SIGINFO;
    sigaction(SIGALRM, &sa, NULL);

    printf("Comando 1\n");
    sleep(10);
    printf("Execution ended!\n");
    return 0;
}