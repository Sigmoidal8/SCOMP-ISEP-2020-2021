/*
 * ex1.c
 *
 *  Created on: 06/05/2021
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

#define CHILDS 8
#define NUMBER_PER_CHILD 200
#define NUMBERS 1600

int main()
{
    int i, j;
    sem_t *semaphore1 = sem_open("/ex1read", O_CREAT | O_EXCL, 0644, 1);
    if (semaphore1 == SEM_FAILED)
    {
        printf("Erro a criar o semaforo de ler!\n");
        exit(EXIT_FAILURE);
    }

    sem_t *semaphore2 = sem_open("/ex1write", O_CREAT | O_EXCL, 0644, 1);
    if (semaphore2 == SEM_FAILED)
    {
        printf("Erro a criar o semaforo de escrever!\n");
        exit(EXIT_FAILURE);
    }

    srand((unsigned)time(NULL));
    FILE *inputFile;
    inputFile = fopen("Numbers.txt", "w");
    for (i = 0; i < NUMBERS; i++)
    {
        fprintf(inputFile, "%d ", rand() % 100);
    }
    fclose(inputFile);

    FILE *reading;
    FILE *writing;

    for (i = 0; i < CHILDS; i++)
    {
        pid_t pid = fork();
        if (pid == -1)
        {
            perror("Failed fork");
        }
        if (pid == 0)
        {
            int nums[200], count = 0;
            int inicio = i * NUMBER_PER_CHILD;
            int fim = inicio + NUMBER_PER_CHILD;
            sem_wait(semaphore1);
            reading = fopen("Numbers.txt", "r");
            for (j = 0; j < fim; j++)
            {
                if (j < inicio)
                {
                    fscanf(reading, "%*d ");
                }else{
                    fscanf(reading, "%d ", &nums[count]);
                    count++;
                }
            }
            fclose(reading);
            sem_post(semaphore1);

            sem_wait(semaphore2);
            writing = fopen("Output.txt", "a");
            for (j = 0; j < 200; j++)
            {
                fprintf(writing, "%d\n", nums[j]);
            }
            fclose(writing);
            sem_post(semaphore2);
            exit(0);
        }
    }

    for (i = 0; i < CHILDS; i++)
    {
        wait(NULL);
    }

    int num;
    FILE *output;
    output = fopen("output.txt", "r");
    if (output == NULL)
    {
        printf("Erro a abrir o ficheiro!");
        exit(EXIT_FAILURE);
    }
    while (fscanf(output, "%d\n", &num) != EOF)
    {
        printf("%d\n", num);
    }

    if (fclose(output) < 0)
    {
        printf("Error  close()!\n");
        exit(EXIT_FAILURE);
    }

    if(sem_unlink("ex1read")<0||sem_unlink("ex1write")<0){
		perror("ERROR");
		exit(0);
	}

    return 0;
}

//Eu Miguel Silva, 1190778, revi o exercício do meu colega Raúl.