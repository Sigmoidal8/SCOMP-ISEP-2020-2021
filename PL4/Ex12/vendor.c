/*
 * client.c
 *
 *  Created on: 15/05/2021
 *      Author: raulcoelho
 */
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>

typedef struct
{
    int cliente;
    int bilhete;
} data_cliente;

int main()
{
    sem_t *semaforos[2];

    if ((semaforos[0] = sem_open("ex12Cliente1", O_CREAT, 0644, 0)) == SEM_FAILED)
    {
        perror("Error at sem_open() seller 1!\n");
        exit(EXIT_FAILURE);
    }

    if ((semaforos[1] = sem_open("ex12Cliente2", O_CREAT, 0644, 1)) == SEM_FAILED)
    {
        perror("Error at sem_open() seller 2!\n");
        exit(EXIT_FAILURE);
    }

    data_cliente *data;
    int fd;

    if ((fd = shm_open("/ex12shm", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR)) == -1)
    {
        perror("shm_open error");
        exit(1);
    }

    if (ftruncate(fd, sizeof(data)) == -1)
    {
        perror("ftruncate error");
        exit(1);
    }

    if ((data = (data_cliente *)mmap(NULL, sizeof(data), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED)
    {
        perror("mmap error");
        exit(1);
    }

    int num_bilhete = 1;

    do
    {
        sem_wait(semaforos[1]);
        data->bilhete = num_bilhete;
        num_bilhete++;
        data->cliente--;
        sleep(2);
        sem_post(semaforos[0]);
    } while (data->cliente > 0);

    if (sem_close(semaforos[0]) == -1)
    {
        perror("Error at sem_close() 1!\n");
        exit(EXIT_FAILURE);
    }
    if (sem_close(semaforos[1]) == -1)
    {
        perror("Error at sem_close() 2!\n");
        exit(EXIT_FAILURE);
    }

    if (sem_unlink("ex12Cliente1") == -1)
    {
        perror("Error at sem_unlink()!\n");
        exit(EXIT_FAILURE);
    }
    if (sem_unlink("ex12Cliente2") == -1)
    {
        perror("Error at sem_unlink()!\n");
        exit(EXIT_FAILURE);
    }

    if (munmap(data, sizeof(data_cliente)) == -1)
    {
        perror("munmap failed");
        exit(EXIT_FAILURE);
    }
    if (close(fd) == -1)
    {
        perror("close failed");
        exit(EXIT_FAILURE);
    }
    if (shm_unlink("/ex12shm") < 0)
    {
        perror("Error at unlink");
        exit(EXIT_FAILURE);
    }

    return 0;
}
