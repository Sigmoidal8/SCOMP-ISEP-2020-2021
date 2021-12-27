/*
 * ex8.c
 *
 *  Created on: 29/04/2021
 *      Author: raulcoelho
 */
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

typedef struct
{
    int value;
} value;

int main()
{
    int fd, i=0 , data_size = sizeof(value);
    value *v;

    fd = shm_open("/ex08", O_CREAT | O_EXCL | O_RDWR, S_IWUSR | S_IRUSR);
    if (fd == -1)
    {
         printf("Failed shm reader\n");
        exit(EXIT_FAILURE);
    }

    int truncate = ftruncate(fd, data_size);
    if (truncate == -1)
    {
        printf("Error ftruncate!\n");
        exit(EXIT_FAILURE);
    }

    v = (value *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (v == NULL)
    {
        printf("Error mmap!\n");
        exit(EXIT_FAILURE);
    }
    
    pid_t p = fork();
    if (p == -1){
        printf("Erro no fork!");
        return 1;
    }

    v->value = 100;
    if (p > 0){
        for (i = 0; i< 1000000; i++){
            v->value += 1;
            v->value -= 1;
        }
    } else {
        for (i = 0; i < 1000000; i++){
            v->value += 1;
            v->value -= 1;
        }
        printf("Child number: %d\n", v->value);
        exit(0);
    }
    wait(NULL);
    printf("Parent number: %d\n", v->value);

    if (munmap(v, data_size) < 0)
    {
        perror("Error munmap!\n");
        exit(EXIT_FAILURE);
    }

    if (close(fd) < 0)
    {
        perror("Error close!\n");
        exit(EXIT_FAILURE);
    }

    if (shm_unlink("/ex08") < 0)
    {
        perror("erro a fechar memória");
        exit(EXIT_FAILURE);
    }

    return 0;
}

//Não pois não é foi utilizado a sincronização de processos. 

//Eu Miguel Silva, 1190778, revi o exercício do meu colega Raúl.