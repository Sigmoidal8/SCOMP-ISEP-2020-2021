/*
 * ex04.c
 *
 *  Created on: 11/03/2021
 *      Author: isep
 */

#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>

int main(void) {
	int a=0, b, c, d;
	b = (int) fork();
	c = (int) getpid(); /* getpid(), getppid(): unistd.h*/
	d = (int) getppid();
	a = a + 5;
	printf("\na=%d, b=%d, c=%d, d=%d\n",a,b,c,d);
}

/*
 * Exercicio 4
 * a)
 * A variavel a) tera o mesmo valor nos dois processos, já as outras serão diferentes,
 * mas a variável b do pai será igual a c do filho e a variável c do pai será igual
 * à variável d do filho.
 *
 * b)
 * (encontra-se um png com a resolucao nesta mesma pasta)
 *
 */

/*
 * Eu Raúl Coelho nº1190986 revi o exercicio do meu colega Miguel
 */
