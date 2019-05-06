#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "types.h"
#include "msg.h"

// Para la lectura de argumentos
#include <getopt.h>

// !!! no estoy muy seguro de esto
#include "simulador.h"
#include "mapa.h"


// Lee las flags y argumentos introducidos y modifica los parametros
// de ejecucion necesarios
void leer_argumentos(int argc, char **argv) {
    // NOTA: No deben aplicarse colores en esta funcion
    char out_buff[BUFF_MAX];
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
                estiloMSG_set_colorful();
				break;
	
			case '2' :
                if (!optarg) {
                    msg_ERR(fpo, "Falta argumento ""fichero_out""");
                    exit(EXIT_FAILURE);
                }

                if (strlen(optarg) > MAX_FICHERO_OUT){
                    msg_ERR(fpo, "Argumento ""fichero_out"" demasiado largo");
                    exit(EXIT_FAILURE);
                }
                
                args.F_fichero_out = true;
                strcpy(args.fichero_out, optarg);
				break;

            case '3' : 
			case '?' :
			default:
                sprintf(out_buff, "Ejecucion: %s <-f fichero_log> <-c>", argv[0]);
                msg_ERR(fpo, out_buff);
				exit(EXIT_FAILURE);
				break;
		}
	}
}


// Establece los parametros de ejecucion por defecto
void set_default_params() {
    args.F_fichero_out = false;
    strcpy(args.fichero_out, "");
    fpo = stdout;
    args.F_color = false;
    estiloMSG_set_default();
}

// Abre un fichero de log
void abrir_log() {
    char out_buff[BUFF_MAX];
    fpo = fopen(args.fichero_out, "w");
    if  (!fpo) {
        sprintf(out_buff, "No se ha podido abrir el fichero: %s", args.fichero_out);
        msg_ERR(stdout, out_buff);
        exit(EXIT_FAILURE);
    }
}


// Rutina principal
int main(int argc, char **argv) {
    

    set_default_params();
    leer_argumentos(argc, argv);

    if (args.F_fichero_out) 
        abrir_log();

    srand(time(NULL));

    msg_OK(fpo, "Lanzando simulacion");
    sim_launch();
    msg_OK(fpo, "Finalizando simulacion");

    if (args.fichero_out)   
        fclose(fpo);

    return EXIT_SUCCESS;
}