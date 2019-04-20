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

#include <simulador.h>
#include <gamescreen.h>
#include <mapa.h>

tipo_argumentos args;
tipo_estilo estilo;
FILE * fpo;

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
void end_exec(){
	if(args.F_fichero_out)
		fclose(fpo);
}

int main(int argc, char *argv[]) {

    char out_buffer[STRING_MAX];

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


	screen_init();

	sleep(1);

	screen_end();


	end_exec();
	
	exit(EXIT_SUCCESS);
}
