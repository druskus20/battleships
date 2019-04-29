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
#include <semaphore.h>
#include <unistd.h>
// Para la lectura de argumentos
#include <getopt.h>

#include "mapa.h"
#include "simulador.h"
#include "nave.h"



tipo_argumentos args;
tipo_estilo estilo;
FILE * fpo;


void leer_argumentos(int argc, char **argv) {
    // NOTA: No deben aplicarse colores en esta funcion
    char out_buffer[STRING_MAX];
	int long_index = 0;
	char opt;
    

    static struct option options[] =
	{
		{"c",  no_argument,       0,  '1'},
        {"f",  required_argument, 0,  '2'},
		{"h",  no_argument,       0,  '3'},
		{0,    0,                 0,   0 }
	};

	// Lectura de flags de ejecución
	while ((opt = getopt_long_only(argc, argv, "1:2:3", options, &long_index)) != -1)
	{
		switch (opt)
		{
			case '1' :     
                args.F_color = true;
                strcpy(estilo.sim, SIM_MC);
                strcpy(estilo.jefe, JEFE_MC);
                strcpy(estilo.nave, NAVE_MC);
                strcpy(estilo.ok_msg, OK_MC);
                strcpy(estilo.error_msg, ERROR_MC);
                strcpy(estilo.ok, OK_SC);
                strcpy(estilo.err, ERROR_SC);
				break;
	
			case '2' :
                if (!optarg) {
                    fprintf(fpo, estilo.error_msg, "Error. Falta argumento ""fichero_out""");  
                    exit(EXIT_FAILURE);
                }
                if (strlen(optarg) > MAX_FICHERO_OUT){
                    fprintf(fpo, estilo.error_msg, "Error. Argumento ""fichero_out"" demasiado largo");
                    exit(EXIT_FAILURE);
                }
                
                args.F_fichero_out = true;
                strcpy(args.fichero_out, optarg);
				break;

            case '3' : 
			case '?' :
			default:
                sprintf(out_buffer, "Error. Ejecucion: %s <-f fichero_log> <-c>", argv[0]);
                fprintf(fpo, estilo.error_msg, out_buffer);
				exit(EXIT_FAILURE);
				break;
		}
	}
}

// Libera recursos
void end_exec() {
    if (args.fichero_out)   
        fclose(fpo);
}

// Manejador de la señal Ctrl+C (SIGINT)
void manejador_SIGINT(int sig) {
    fprintf(fpo, "\n");
    fprintf(fpo, estilo.ok_msg, "Ending the program");
    end_exec();
    exit(EXIT_SUCCESS);
}

void imprimir_semaforo(sem_t *sem) {
    int sval;
    if (sem_getvalue(sem, &sval) == -1) {
          
        fprintf(fpo, estilo.ok_msg, "Fin de la simulación");
		sem_unlink(SEM_SIMULADOR);
        exit(EXIT_FAILURE);
    }
    printf("Valor del semáforo: %d\n", sval);
    fflush(stdout);
}


int main(int argc, char **argv) {
    
    struct sigaction act;
    char out_buffer[STRING_MAX];
    sem_t * sem_sim;

    // Inicializacion de parametros por defecto
    args.F_fichero_out = false;
    strcpy(args.fichero_out, "");
    fpo = stdout;

    args.F_color = false;
    strcpy(estilo.sim, SIM_M);
    strcpy(estilo.jefe, JEFE_M);
    strcpy(estilo.nave, NAVE_M);
    strcpy(estilo.ok_msg, OK_M);
    strcpy(estilo.error_msg, ERROR_M);
    strcpy(estilo.ok, OK_S);
    strcpy(estilo.err, ERROR_S);
    
    leer_argumentos(argc, argv);

    if (args.F_fichero_out) {
        fpo= fopen(args.fichero_out, "w");
        if  (!fpo) {
            sprintf(out_buffer, "Error. No se ha podido abrir el fichero: %s\n", args.fichero_out);
            fprintf(fpo, estilo.error_msg, out_buffer);
            exit(EXIT_FAILURE);
        }
    }

    // Inicializacion del manejador SIGINT
    act.sa_handler = manejador_SIGINT;
    sigemptyset(&(act.sa_mask));
    act.sa_flags = 0;

    if (sigaction(SIGINT, &act, NULL) < 0) {
        fprintf(fpo, estilo.error_msg, "sigaction");
        exit(EXIT_FAILURE);
    }

  
	
	// Inicializacion de semaforos  
//	if((sem_sim = sem_open(SEM_SIMULADOR, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 0))== SEM_FAILED){
    if((sem_sim = sem_open(SEM_SIMULADOR, O_CREAT, S_IRUSR | S_IWUSR, 0)) == SEM_FAILED){
    		fprintf(fpo, estilo.error_msg, "sem_open de ""sem_sim""");
		exit(EXIT_FAILURE);
	}


    fprintf(fpo, estilo.ok_msg, "Comienza la simulación");
    // COMIENZO DE LA SIMULACION ---------------------------------
    sem_post(sem_sim);

   
       
    fprintf(fpo, estilo.ok_msg, "Fin de la simulación");    
    // FIN DE LA SIMULACION --------------------------------------

    // Removemos el manejador de señal para evitar errores 
    // mientras liberamos recursos.
    signal(SIGINT, SIG_DFL);
    sem_close(sem_sim);
    sem_unlink(SEM_SIMULADOR); // !!! funciona si se cierra antes que monitor?
    end_exec();
    exit(EXIT_SUCCESS);
}




