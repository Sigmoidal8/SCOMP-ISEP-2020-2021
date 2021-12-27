/*
 * ex1.c
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
    int number;
    char name[50];
} student;

int writer()
{
    int fd, data_size = sizeof(student);
    student *st;

    shm_unlink("/ex1");
    fd = shm_open("/ex1", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
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

    st = (student *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (st == NULL)
    {
        perror("Error mmap!\n");
        exit(EXIT_FAILURE);
    }

    printf("Insert a name\n");
    scanf("%s", st->name);

    printf("Insert the number\n");
    scanf("%d", &st->number);

    return 1;
}

int reader()
{
    int fd, data_size = sizeof(student);
    student *st;
    fd = shm_open("/ex1", O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == 0)
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
    st = (student *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if (st == NULL)
    {
        perror("Error mmap!\n");
        exit(EXIT_FAILURE);
    }

    printf("\nThe data inserted was:\n");
    printf("%d\n", st->number);
    printf("%s\n", st->name);

    return 1;
}

int main(void)
{
    int w = writer();
    if (w == 0)
    {
        return w;
    }

    int r = reader();
    if (r == 0)
    {
        return r;
    }

     if (shm_unlink("/ex1") < 0)
    {
        perror("erro a fechar memória");
        exit(EXIT_FAILURE);
    }

    return 1;
}

//Eu Miguel Silva, 1190778, revi o exercício do meu colega Raúl.
