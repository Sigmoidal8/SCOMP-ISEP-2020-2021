/*
 * ex12.c
 *
 *  Created on: 29/04/2021
 *      Author: raulcoelho
 */
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define STR_SIZE 50
#define NR_DISC 10

typedef struct{
int numero;
char nome[STR_SIZE];
int disciplinas[NR_DISC];
int available;
}aluno;

int main(){
    int fd, data_size = sizeof(aluno), i;
	aluno *shared_data;

    fd = shm_open("/ex12", O_CREAT|O_EXCL| O_RDWR, S_IRUSR|S_IWUSR);
    if( fd == -1){
        printf("erro na memória partilhada");
        return -1;
    }

    int truncate = ftruncate(fd, data_size);
    if(truncate == -1){
        printf("erro no truncate");
        return -1;
    }

    shared_data =  (aluno *) mmap(NULL, data_size, PROT_READ|PROT_WRITE, MAP_SHARED,fd,0);
    
    pid_t p = fork();
    if (p == -1){
        printf("Erro no fork!");
        return 1;
    }
    if (p > 0){
        shared_data->available = 0;
        int num;
        printf("Insert the number:\n");
        scanf("%d",&num);
        shared_data->numero = num; 

        char name[STR_SIZE];
        printf("Insert the name:\n");
        scanf("%s",name);
        strcpy(shared_data->nome, name);

        for (i = 0; i < NR_DISC; i++){
            int disc;
            printf("Insert the course:\n");
            scanf("%d",&disc);
            shared_data->disciplinas[i] = disc;
        }     
        shared_data->available = 1;   
    }else{
        while (shared_data->available != 1);
        int soma = shared_data->disciplinas[0];
        int maior = shared_data->disciplinas[0];
        int menor = shared_data->disciplinas[0];
        for(i = 1; i < NR_DISC; i++){
            int valor = shared_data->disciplinas[i];
            soma += valor;
            if (valor > maior){
                maior = valor;
            }
            if (valor < menor){
                menor = valor;
            }
        }

        int media = (soma/NR_DISC);
        printf("Average: %d\n",media);
        printf("Biggest: %d\n", maior);
        printf("Lowest: %d\n",menor);

        if(close(fd) == -1){
            printf("Erro close");
            return -1;
        }
        exit(0);
    }
    
    wait(NULL);

    if (munmap(shared_data, data_size)){
        printf("erro no munmap");
        return -1;
    }

    if (close(fd) == -1){
        printf("erro no close");
        return -1;
    }
    
    if (shm_unlink("/ex12")){
        printf("erro no shm_unlink");
        return -1;
    }
    return 0;
}

//Eu Miguel Silva, 1190778, revi o exercício do meu colega Raúl.