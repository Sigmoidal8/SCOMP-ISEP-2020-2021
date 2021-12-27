/*
 * ex17.c
 *
 *  Created on: 15/05/2021
 *      Author: miguelsilva
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include <time.h>

#define NUM_FILHOS 305     /* número de processos filho */
#define MAX_VISITANTES 300 /* quantidade máxima de visitantes */
#define DURACAO_SHOW 5     // tempo que o show dura

#define ID_VIP 0      /* id visitante VIP */
#define ID_ESPECIAL 1 /* id Special */
#define ID_NORMAL 2   /* id visitante Normal */

#define SEM_NAME_1 "semaforo1" /* primeiro sem */
#define SEM_NAME_2 "semaforo2" /* segundo sem */
#define SEM_NAME_3 "semaforo3" /* terceiro sem */
#define SEM_NAME_4 "semaforo4" /*quarto sem */

typedef struct
{
    int n_vip;
    int num_especial;
    int size;
} shared_memory;

shared_memory *start_buffer();

int main()
{
    int i, j = 0, id;
    shared_memory *shared = start_buffer();

    sem_t *sem_vip;        /* limita entrada de visitantes VIP */
    sem_t *sem_especial;   /* limita a entrada de visitantes Special */
    sem_t *sem_normal;     /* limita a entrada de visitantes Normal */
    sem_t *sem_visitantes; /* limita a capacidade do cinema */
    sem_t *sem_mutex;

    if ((sem_vip = sem_open("semaforo1", O_CREAT, 0644, 1)) == SEM_FAILED)
    {
        perror("Failed sem_open\n");
        return 1;
    }
    if ((sem_especial = sem_open("semaforo2", O_CREAT, 0644, 1)) == SEM_FAILED)
    {
        perror("Failed sem_open\n");
        return 1;
    }
    if ((sem_normal = sem_open("semaforo3", O_CREAT, 0644, 1)) == SEM_FAILED)
    {
        perror("Failed sem_open\n");
        return 1;
    }
    if ((sem_visitantes = sem_open("semaforo4", O_CREAT, 0644, MAX_VISITANTES)) == SEM_FAILED)
    {
        perror("Failed sem_open\n");
        return 1;
    }
    if ((sem_mutex = sem_open("semaforo5", O_CREAT, 0644, 1)) == SEM_FAILED)
    {
        perror("Failed sem_open\n");
        return 1;
    }

    for (i = 0; i < NUM_FILHOS; i++)
    {
        if (i % 3 == ID_VIP)
        {
            shared->n_vip++; /* aumenta os VIPs na fila de espera */
        }
        if (i % 3 == ID_ESPECIAL)
        {
            shared->num_especial++; /* aumenta os especias na fila de espera aumenta */
        }
    }

    for (i = 0; i < NUM_FILHOS; i++)
    {
        pid_t pid = fork();
        if (pid == -1)
        {
            perror("Failed fork\n");
            exit(1);
        }
        else if (pid == 0)
        {
            while (1)
            {
                id = i % 3; /* atribui uma classificação ao visitante VIP, Special ou Normal */

                if (id == ID_VIP)
                {                             /* se o visitante for VIP */
                    sem_wait(sem_vip);        /* entrada de um VIP */
                    sem_wait(sem_visitantes); /*numero de lugares vazios diminui */

                    sem_wait(sem_mutex);
                    printf("Visitante VIP %d entrou\n", i + 1);
                    shared->n_vip--;
                    j++;
                    if (j <= MAX_VISITANTES)
                    {
                        sem_post(sem_vip);
                    }
                    sem_post(sem_mutex);
                }
                else if (id == ID_ESPECIAL)
                {                             /* se o visitante for especial */
                    sem_wait(sem_especial);   /* entrada de um especial */
                    sem_wait(sem_visitantes); /* numero de lugares vazios diminui */

                    sem_wait(sem_mutex);
                    printf("Visitante Special %d entrou\n", i + 1);
                    shared->num_especial--;
                    j++;
                    if (j <= MAX_VISITANTES)
                    {
                        sem_post(sem_especial);
                    }
                    sem_post(sem_mutex);
                }
                else
                {                             /* se o visitante for Normal */
                    sem_wait(sem_normal);     /* entrada de um visitante Normal */
                    sem_wait(sem_visitantes); /* numero de lugares vazios diminui */

                    sem_wait(sem_mutex);
                    printf("Visitante Normal %d entrou\n", i + 1);
                    j++;
                    if (j <= MAX_VISITANTES)
                    {
                        sem_post(sem_normal);
                    }
                    sem_post(sem_mutex);
                }

                sleep(DURACAO_SHOW); /* tempo que os visitantes ficam no cinema assisitndo */

                sem_post(sem_visitantes); /* auementa quantiadde de lugares vagos */
                printf("Visitante %d saiu\n", i + 1);

                sem_wait(sem_mutex);
                if (shared->n_vip > 0)
                { /* VIPs entram em primeiro lugar */
                    sem_post(sem_vip);
                }
                else if (shared->n_vip <= 0 && shared->num_especial > 0)
                { /* especiais entram em segundo lugar */
                    sem_post(sem_especial);
                }
                else if (shared->n_vip <= 0 && shared->num_especial <= 0)
                {
                    sem_post(sem_normal); /* Normais entram em último lugar */
                }
                sem_post(sem_mutex);
                exit(0);
            }
        }
    }

    for (i = 0; i < NUM_FILHOS; i++)
        wait(NULL);

    if(sem_unlink("semaforo1")<0||sem_unlink("semaforo2")<0||sem_unlink("semaforo3")<0||sem_unlink("semaforo4")<0||sem_unlink("semaforo5")<0){
        perror("ERROR");
        exit(1);
    }

    return 0;
}

shared_memory *start_buffer()
{
    int fd, ft, data_size = sizeof(shared_memory);
    shared_memory *shared;

    fd = shm_open("/ex17", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        perror("Erro ao criar memória partilhada!");
        return 0;
    }

    ft = ftruncate(fd, data_size);
    if (ft == -1)
    {
        perror("Erro na alocação da memória.");
        return 0;
    }

    shared = (shared_memory *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (shared == MAP_FAILED || shared == 0)
    {
        perror("Erro ao mapear a memória.");
        return 0;
    }

    // Inicializar os valores da head e tail
    shared->n_vip = 0;
    shared->num_especial = 0;

    if(shm_unlink("/ex17")<0){
        perror("ERROR");
        exit(1);
    }

    return shared;
}

//Eu Raúl Coelho revi o exercício do meu colega.