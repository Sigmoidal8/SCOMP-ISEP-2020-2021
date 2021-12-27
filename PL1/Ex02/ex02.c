/*
 * ex02.c
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
    char *vec[3];
    *vec = "I am ...\n";
    *(vec + 1) = "the ...\n";
    *(vec + 2) = "father!\n";

    int i;
    for (i = 0; i < 3; i++)
    {
        pid_t p = fork();
        if (p < 0)
        {
            perror("fork failed");
            exit(1);
        }
        else if (p == 0)
        {
            printf("I will never join you!\n");
            exit(0);
        }
        else
        {
            printf("%s", *(vec + i));
            sleep(1);
        }
    }
return 0;
}

/*
 * Eu Jose Silva n1190778 revi o exercicio do meu colega raul
 */
