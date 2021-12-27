/*
 * ex9.c
 *
 *  Created on: 06/04/2021
 *      Author: raulcoelho
 */
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/wait.h>

#define SALES_NUMBER 50

typedef struct
{
    int customer_code;
    int product_code;
    int quantity;
} sales;

int main(void)
{
    sales sales_array[SALES_NUMBER];
    int products[SALES_NUMBER],j;
    int i;
    srand(time(NULL));

    for (i = 0; i < SALES_NUMBER; i++)
    {
        sales_array[i].customer_code = rand() % 50 + 1;
        sales_array[i].product_code = rand() % 50 + 1;
        sales_array[i].quantity = rand() % 50 + 1;
    }

    pid_t pid[10];
    int fd[2], status;
    if (pipe(fd) == -1)
    {
        perror("Pipe failed\n");
        return 1;
    }

    for (i = 0; i < 10; i++)
    {
        pid[i] = fork();
        if (pid[i] == 0)
        {
            close(fd[0]);
            int inicio = i * (SALES_NUMBER / 10);
            int fim = (i + 1) * (SALES_NUMBER / 10) - 1;
            for (j = inicio; j < fim; j++)
            {
                if (sales_array[j].quantity > 20)
                {
                    write(fd[1], &(sales_array[j].product_code), sizeof(int));
                }
            }
            close(fd[1]);
            exit(0);
        }
        else if (pid[i] < 0)
        {
            perror("failed fork");
            exit(0);
        }
    }

    int product;
    int retorno;
    close(fd[1]);
    int n_codes = 0;
    while ((retorno = read(fd[0], &product, sizeof(int)) != 0))
    {
        products[n_codes] = product;
        n_codes++;
    }
    close(fd[0]);

    for (i = 0; i < 10; i++)
    {
        waitpid(pid[i], &status, 0);
    }
    printf("Products with quantity over 20 in a single purchase:\n");
    int f;
    for (f = 0; f < n_codes; f++)
    {
        printf("Product code: %d\n", products[f]);
    }
    return 0;
}

/*
 * Eu Jose Silva nº1190778 revi o exercicio do meu colega raul
 */
