/*
 * ex1.c
 *
 *  Created on: 11/03/2021
 *      Author: isep
 */
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>


int main(void)
{
    int x = 1;
    pid_t p = fork();

    if (p < 0)
    {
        perror("fork failed");
        exit(1);
    }
    else if (p == 0)
    {
        x = x + 1;
        printf("1. x = %d\n", x);
    }
    else
    {
        x = x - 1;
        printf("2. x = %d\n", x);
    }

    printf("3. %d; x = %d\n", p, x);
    return 0;
}
/*
 * a)
 *O output será:
 *1233; x = 0
 *x = 2
 *0; x = 2
 *
 * Vai aparecer o x do pai que subtrai 1 e depois o seu pid
 * de seguida aparece o x do filho que soma 1 e depois 0 que
 * é o valor atribuido pelo fork.
 *
 * b)
 * Não, pois não é obrigatório ser o processo pai a correr
 * primeiro mas sim o processo atribuído pelo escalonador.
 */

/*
 * Eu Jose Silva n1190778 revi o exercicio do meu colega raul
 */
