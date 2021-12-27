/*
 * ex14.c
 *
 *  Created on: 01/05/2021
 *      Author: miguelsilva
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>

#define BUFFER 10
#define OPERATIONS 30

typedef struct
{
    int buffer[BUFFER];
    int head;
    int tail;
    int size;
} circular_buffer;

int check_buffer_is_full(circular_buffer *shared)
{
    if ((shared->head + 1) % shared->size == shared->tail)
    {
        return 1;
    }
    return 0;
}

int check_buffer_is_empty(circular_buffer *shared)
{
    if (shared->head == shared->tail)
    {
        return 1;
    }
    return 0;
}

int main(void)
{

    int i;
    int fd, data_size = sizeof(circular_buffer);
    circular_buffer *shared;

    shm_unlink("/ex14");

    fd = shm_open("/ex14", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
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

    shared = (circular_buffer *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (shared == MAP_FAILED)
    {
        perror("Error mmap!\n");
        exit(EXIT_FAILURE);
    }

    // Defenir o tamanho do buffer e inicializar os valores da head e tail
    shared->size = BUFFER;
    shared->head = 0;
    shared->tail = 0;

    pid_t pid = fork();
    if (pid < 0)
    {
        perror("Fork Failed.");
        exit(-1);
    }

    if (pid == 0)
    {
        for (i = 0; i < OPERATIONS; i++)
        {
            while (check_buffer_is_empty(shared))
            {
                printf("O buffer está vazio. Nada pode ser lido.\n");
                sleep(1);
            }
            int value_read;
            // Ler o valor no buffer
            value_read = shared->buffer[shared->tail];
            printf("Foi lida na posição %d do buffer o valor %d\n", shared->tail, value_read);
            // Avançar com a "cauda" do buffer uma posição
            shared->tail = (shared->tail + 1) % shared->size;
        }
        exit(0);
    }
    else if (pid > 0)
    {
        for (i = 0; i < OPERATIONS; i++)
        {
            while (check_buffer_is_full(shared))
            {
                printf("O buffer está cheio. Nada pode ser escrito.\n");
                sleep(1);
            }
            // Escrever o valor no buffer
            shared->buffer[shared->head] = i;
            printf("Foi escrito na posição %d do buffer o valor %d\n", shared->head, i);
            // Avançar com a "cabeça" do buffer uma posição
            shared->head = (shared->head + 1) % shared->size;
        }
        wait(NULL);
        if (munmap(shared, 150) < 0)
        {
            perror("ERROR");
            exit(1);
        }
        if (shm_unlink("/ex14") < 0)
        {
            perror("ERROR");
            exit(1);
        }
        if(close(fd) < 0){
            perror("ERROR");
            exit(1);
        }
    }

    return 0;
}

//Eu Raúl Coelho revi o exercício do meu colega