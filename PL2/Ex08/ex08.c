/*
 * ex08.c
 *
 *  Created on: 06/04/2021
 *      Author: miguelsilva
 */

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <time.h>

struct structure{
	char string[4];
	int ronda;
} struct1;

int main(void){
	int i,j,status[10];
	int fd[2];
	pid_t pid[10];

	if (pipe(fd) == -1){
		perror("Pipe failed\n");
		return 1;
	}
	
	for(i=0;i<10;i++){											
		pid[i]=fork();
		if (pid[i] == -1){
			printf("Fork Failed!");
			exit(1);
		}else if(pid[i]==0){	
			close(fd[1]);
			read(fd[0],&struct1,sizeof(struct1));				
			close(fd[0]);
			printf("%s - %d\n", struct1.string, struct1.ronda);
			exit(struct1.ronda);								
		}
	}	
	
	close(fd[0]);
	strcpy(struct1.string,"Win");
	for(i=0;i<10;i++){									
		printf("RONDA %d!\n",i+1);						
		struct1.ronda=i+1;								
		write(fd[1],&struct1,sizeof(struct1));
		sleep(2);
	}
	close(fd[1]);
	
	for(i=0;i<10;i++){                  				
		waitpid(pid[i],&status[i],0);
	}
	
	for(i=0;i<10;i++){									
		for(j=0;j<10;j++){
			if(WEXITSTATUS(status[j])==i){
				printf("Vencedor da ronda %d: PID: %d\n",WEXITSTATUS(status[j])+1,pid[j]);
			}
		}
	}
	return 0;
}

/*
* Eu Raúl Coelho revi o trabalho do meu colega Miguel Silva
*/