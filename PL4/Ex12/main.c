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

int main(){
    int fd;
	data_cliente* data;

	if((fd = shm_open("/ex12shm", O_CREAT|O_RDWR, S_IRUSR|S_IWUSR)) == -1){
		perror("shm_open error");
		exit(1);
	}
	if(ftruncate(fd,sizeof(data)) == -1){
		perror("ftruncate error");
		exit(1);
	}
	if((data = (data_cliente*) mmap(NULL, sizeof(data), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED){
		perror("mmap error");
		exit(1);
	}

    data->cliente=0;

    int i;
    pid_t pid;
    for(i=0; i<5; i++){
        pid=fork();
		if(pid==0){
			execlp("./client","./client",(char*)NULL);
		}
    }
    sleep(2);
    execlp("./vendor","./vendor",(char*)NULL);
    return 0;
}

//Eu Miguel Silva, 1190778, revi o exercício do meu colega Raúl.