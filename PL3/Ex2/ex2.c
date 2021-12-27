/*
 * ex2.c
 *
 *  Created on: 04/04/2021
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
    int code;
    char description[100];
    int price;
} product;

int writer()
{
    int fd, w, data_size = sizeof(product);
    product *pr;


    fd = shm_open("/ex2", O_CREAT | O_EXCL | O_RDWR, S_IWUSR | S_IRUSR);
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

    pr = (product *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (pr == NULL)
    {
        perror("Error mmap!\n");
        exit(EXIT_FAILURE);
    }

    printf("Insert Product Code: \n");
    scanf("%d", &pr->code);

    printf("Insert Description: \n");
    scanf("%s", pr->description);

    printf("Insert Price: \n");
    scanf("%d", &pr->price);

    w = munmap(pr, data_size);
    if (w < 0)
    {
        perror("Error munmap!\n");
        exit(EXIT_FAILURE);
    }

    w = close(fd);
    if (w < 0)
    {
        perror("Error close!\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}

int reader()
{
    int fd, r,w, data_size = sizeof(product);
    product *pr;

    fd = shm_open("/ex2", O_RDWR, S_IWUSR | S_IRUSR);
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

    pr = (product *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (pr == MAP_FAILED)
    {
        perror("Error mmap!\n");
        exit(EXIT_FAILURE);
    }

    printf("Code: %d\n", pr->code);
    printf("Descripion: %s\n", pr->description);
    printf("Price: %d\n", pr->price);

    r = munmap(pr, data_size);
    if (r < 0)
    {
        perror("Error munmap!\n");
        exit(EXIT_FAILURE);
    }

    w = close(fd);
    if (w < 0)
    {
        perror("Error close!\n");
        exit(EXIT_FAILURE);
    }

     if (shm_unlink("/ex2") < 0)
    {
        perror("erro a fechar memória");
        exit(EXIT_FAILURE);
    }

    return 0;
}

int main(){
    writer();
    reader();
    return 0;
}

//Eu Miguel Silva, 1190778, revi o exercício do meu colega Raúl.