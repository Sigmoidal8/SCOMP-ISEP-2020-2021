/*
 * ex5.c
 *
 *  Created on: 28/04/2021
 *      Author: miguelsilva
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <time.h>

typedef struct
{
    int num1;
    int num2;
} array;

int main(void)
{
    int fd, i = 0, j = 0, data_size = sizeof(array);
    array *numeros;

    shm_unlink("/ex5");

    fd = shm_open("/ex5", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        perror("Failed shm writer!\n");
        exit(EXIT_FAILURE);
    }

    int truncate = ftruncate(fd, data_size);
    if (truncate == -1)
    {
        perror("Error ftruncate!\n");
        exit(EXIT_FAILURE);
    }

    numeros = (array *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (numeros == MAP_FAILED || numeros == 0)
    {
        perror("Error mmap!\n");
        exit(EXIT_FAILURE);
    }

    numeros->num1 = 8000;
    numeros->num2 = 200;

    pid_t p = fork();

    if (p < 0)
    {
        perror("Fork Failed.");
        exit(-1);
    }
    else if (p > 0)
    {
        while (i < 1000000)
        {
            numeros->num1 += 1;
            numeros->num2 -= 1;
            i++;
        }
    }
    else if (p == 0)
    {
        while (j < 1000000)
        {
            numeros->num1 -= 1;
            numeros->num2 += 1;
            j++;
        }
        exit(0);
    }
    wait(0);

    printf("Número 1: %d  # # #  Número 2: %d\n", numeros->num1, numeros->num2);

    if(close(fd) < 0){
        perror("Error");
        exit(EXIT_FAILURE);
    }

    if(shm_unlink("/ex5") < 0){
        perror("Error");
        exit(EXIT_FAILURE);
    }

    return 0;
}

//Não uma vez que não é usado nenhum mecanismo de sincronização

//Eu Raúl Coelho 1190986 revi o exercício do meu colega Miguel