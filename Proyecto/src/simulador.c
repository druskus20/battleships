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
#include <mapa.h>
#include "simulador.h"

// Para la lectura de argumentos
#include <getopt.h>

tipo_argumentos argumentos;
tipo_estilo estilo;

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
	while ((opt = getopt_long_only(argc, argv, "f:i:h", options, &long_index)) != -1)
	{
		switch (opt)
		{
			case '1' :     
                argumentos.F_color = true;
                strcpy(estilo.sim, SIM_C);
                strcpy(estilo.jefe, JEFE_C);
                strcpy(estilo.nave, NAVE_C);
                strcpy(estilo.ok_msg, OK_MSG_C);
                strcpy(estilo.error_msg, ERROR_MSG_C);
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
                
                argumentos.F_fichero_out = true;
                strcpy(argumentos.fichero_out, optarg);
				break;

            case '3' : 
			case '?' :
			default:
				printf ("Error. Ejecucion: %s <-f fichero_log> <-nc>\n", argv[0]);
				exit(EXIT_FAILURE);
				break;
		}
	}
}


int main(int argc, char **argv) {
    
	int ret=0;

    // Inicializacion de parametros por defecto
    argumentos.F_fichero_out = false;
    strcpy(argumentos.fichero_out, "");

    argumentos.F_color = false;
    strcpy(estilo.sim, SIM);
    strcpy(estilo.jefe, JEFE);
    strcpy(estilo.nave, NAVE);
    strcpy(estilo.ok_msg, OK_MSG);
    strcpy(estilo.error_msg, ERROR_MSG);
    
    leer_argumentos(argc, argv);

    exit(ret);
}



