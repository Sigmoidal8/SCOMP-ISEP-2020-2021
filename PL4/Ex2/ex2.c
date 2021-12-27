/*
 * ex2.c
 *
 *  Created on: 13/05/2021
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
    int i, j, num;
    char semName[100];
    FILE *reading, *writing;
    pid_t pid;
    sem_t *sem[NUMBER_PER_CHILD];

    sprintf(semName, "%s%d", "/ex2", 1);
    if ((sem[0] = sem_open(semName, O_CREAT, 0644, 1)) == SEM_FAILED)
    {
        perror("Erro a criar semaforo");
        exit(1);
    }

    for (i = 1; i < CHILDS; i++)
    {
        sprintf(semName, "%s%d", "/ex2", i + 1);
        if ((sem[i] = sem_open(semName, O_CREAT, 0644, 0)) == SEM_FAILED)
        {
            perror("Erro a criar semaforo");
            exit(0);
        }
    }

    srand((unsigned)time(NULL));
    FILE *inputFile;
    inputFile = fopen("Numbers.txt", "w");
    for (i = 0; i < NUMBERS; i++)
    {
        fprintf(inputFile, "%d ", rand() % 100);
    }
    fclose(inputFile);

    writing = fopen("Output.txt", "w");
    fclose(writing);

    for (i = 0; i < CHILDS; i++)
    {
        pid = fork();
        if (pid == 0) 
        {
            sem_wait(sem[i]);

            reading = fopen("Numbers.txt", "r");
            if (reading != NULL)
            {
                writing = fopen("Output.txt", "a");

                for (j = 0; j < i * NUMBER_PER_CHILD; j++)
                {
                    fscanf(reading, "%d", &num);
                }
                for (; j < (i + 1) * NUMBER_PER_CHILD; j++)
                {
                    fscanf(reading, "%d", &num);
                    fprintf(writing, "%d\n", num);
                }

                fclose(writing);
                fclose(reading);
            }
            if (i < CHILDS - 1)
            {
                sem_post(sem[i + 1]);
            }

            sem_close(sem[i]);
            sprintf(semName, "%s%d", "ex2", i + 1);
            sem_unlink(semName);
            exit(0);
        }
        else 
        {
            wait(NULL);
        }
    }
    writing = fopen("Output.txt", "r");
    if (writing == NULL)
    {
        printf("Não abriu o ficheiro");
    }
    else
    {
        while (fscanf(writing, "%d", &num) != EOF)
        {
            printf("%d\n", num);
        }
    }

    if (fclose(writing) < 0)
    {
        printf("Error  close()!\n");
        exit(EXIT_FAILURE);
    }

    if(sem_unlink("semName")<0){
		perror("ERROR");
		exit(0);
	}

    return EXIT_SUCCESS;
}

//Eu Miguel Silva, 1190778, revi o exercício do meu colega Raúl.