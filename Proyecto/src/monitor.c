#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <math.h>
#include <stdbool.h>
#include <unistd.h>
#include <getopt.h>
#include <semaphore.h>

#include "simulador.h"
#include <gamescreen.h>
#include <mapa.h>


void mapa_print(tipo_mapa *mapa)
{
	int i,j;

	for(j=0;j<MAPA_MAXY;j++) {
		for(i=0;i<MAPA_MAXX;i++) {
			tipo_casilla cas=mapa_get_casilla(mapa,j, i);
			//printf("%c",cas.simbolo);
			screen_addch(j, i, cas.simbolo);
		}
		//printf("\n");
	}
	screen_refresh();
}


int main(int argc, char *argv[]) {

	sem_t * sem_sim;
	tipo_mapa * mapa;
	
	// Inicializacion de semaforos  
    if((sem_sim = sem_open(SEM_SIMULADOR, O_CREAT, S_IRUSR | S_IWUSR, 0)) == SEM_FAILED){
    		fprintf(stdout, "Error. sem_open de ""sem_sim""");
		exit(EXIT_FAILURE);
	}

	sem_wait(sem_sim);

    int fd_shm = shm_open(SHM_MAP_NAME,
                          O_RDONLY, 0);
    if(fd_shm == -1) {
        exit(EXIT_FAILURE);
    }
    
    mapa  = mmap(NULL, sizeof(*mapa),
                        PROT_READ, MAP_SHARED, fd_shm, 0);
    if(mapa == MAP_FAILED){
         exit(EXIT_FAILURE);
    }
    
	screen_init();


	
	while(true){ // !!!!! mapa->fin :)
		mapa_print(mapa);
		sleep(SCREEN_REFRESH);
		
	}
	screen_end();
	munmap(mapa, sizeof(*mapa));
	sem_close(sem_sim);
	
	exit(EXIT_SUCCESS);
}



void init_shm_mapa(tipo_nave * nave)  {

    
}