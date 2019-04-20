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
#include "mapa.h"
#include "simulador.h"
#include "nave.h"

// Para la lectura de argumentos
#include <getopt.h>

tipo_argumentos args;
tipo_estilo estilo;
FILE * fpo;
void leer_argumentos(int argc, char **argv) {
    // NOTA: No deben aplicarse colores en esta funcion

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
                    printf("Error. Falta argumento ""fichero_out""\n");
                    exit(EXIT_FAILURE);
                }
                if (strlen(optarg) > MAX_FICHERO_OUT){
                    printf("Error. Argumento ""fichero_out"" demasiado largo\n");
                    exit(EXIT_FAILURE);
                }
                
                args.F_fichero_out = true;
                strcpy(args.fichero_out, optarg);
				break;

            case '3' : 
			case '?' :
			default:
				printf ("Error. Ejecucion: %s <-f fichero_log> <-c>\n", argv[0]);
				exit(EXIT_FAILURE);
				break;
		}
	}
}


// Manejador de la señal Ctrl+C (SIGINT)
void manejador(int sig) {
    
    //printf("%s", sig);
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
    
	int ret=0;
    struct sigaction act;
   
    
    // Inicializacion de parametros por defecto
    args.F_fichero_out = false;
    strcpy(args.fichero_out, "");

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
            printf("Error. No se ha podido abrir el fichero: %s\n", args.fichero_out);
            exit(EXIT_FAILURE);
        }
    }
    else {
        fpo= stdout;
    }

    // Inicializacion del manejador SIGINT
    act.sa_handler = manejador;
    sigemptyset(&(act.sa_mask));
    act.sa_flags = 0;

    if (sigaction(SIGINT, &act, NULL) < 0) {
        fprintf(fpo, estilo.error_msg, "sigaction");
        exit(EXIT_FAILURE);
    }


    nave_start();
    nave_end();
    
    fprintf(fpo, estilo.ok_msg, "Fin de la simulación");
    exit(ret);

    
}




