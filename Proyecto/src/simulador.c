#include "simulador.h"
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




#include "jefe.h" // !!! funciona??
// Para la lectura de argumentos
#include <getopt.h>

// !!! no estoy muy seguro de esto
#include "mapa.h"
#include "jefe.h"
#include "nave.h"



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
                strcpy(estilo.nave_tag, NAVE_C);
                strcpy(estilo.jefe_tag, JEFE_C);
                strcpy(estilo.sim_tag, SIM_C);    
				break;
	
			case '2' :
                if (!optarg) {
                    fprintf(fpo, estilo.error_msg, "Falta argumento ""fichero_out""");  
                    exit(EXIT_FAILURE);
                }
                if (strlen(optarg) > MAX_FICHERO_OUT){
                    fprintf(fpo, estilo.error_msg, "Argumento ""fichero_out"" demasiado largo");
                    exit(EXIT_FAILURE);
                }
                
                args.F_fichero_out = true;
                strcpy(args.fichero_out, optarg);
				break;

            case '3' : 
			case '?' :
			default:
                sprintf(out_buffer, "Ejecucion: %s <-f fichero_log> <-c>", argv[0]);
                fprintf(fpo, estilo.error_msg, out_buffer);
				exit(EXIT_FAILURE);
				break;
		}
	}
}

// Manejador de la señal Ctrl+C (SIGINT)
void manejador_SIGINT(int sig) {
    fprintf(fpo, "\n");
    fprintf(fpo, estilo.ok_msg, "Ending the program");
    if (args.fichero_out)   
        fclose(fpo);
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
   
    tipo_sim  * sim; // !!! quizas haga falta hacerlo con memoria dinamica por tema de procesos y  memoria entre procesos
                  // !!! No deberia, porque el jefe no interviene con simulador, pero quizas las naves... 

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
    strcpy(estilo.nave_tag, NAVE);
    strcpy(estilo.jefe_tag, JEFE);
    strcpy(estilo.sim_tag, SIM);
    
    
    leer_argumentos(argc, argv);



    // Apertura de fichero de log
    if (args.F_fichero_out) {
        fpo = fopen(args.fichero_out, "w");
        if  (!fpo) {
            sprintf(out_buffer, "No se ha podido abrir el fichero: %s", args.fichero_out);
            fprintf(stdout, estilo.error_msg, out_buffer);      // !!! creo que solo se puede stdout
            exit(EXIT_FAILURE);
        }
    }

    sim = (tipo_sim *)malloc(sizeof(tipo_sim));
    strcpy(sim->tag, estilo.sim_tag); 

    sprintf(out_buffer, "Comenzando %s", sim->tag);
    fprintf(fpo, estilo.ok_msg, out_buffer);
    fprintf(fpo, estilo.sim, sim->tag, estilo.ok, "Comenzando inicialización");
    

	// Inicializacion de semaforo simulador
    if((sim->sem_sim = sem_open(SEM_SIMULADOR, O_CREAT, S_IRUSR | S_IWUSR, 0)) == SEM_FAILED){
        fprintf(fpo, estilo.sim, sim->tag, estilo.err, "sem_open de ""sem_sim""");
		exit(EXIT_FAILURE);
	}  

    // Inicializacion del manejador SIGINT
    act.sa_handler = manejador_SIGINT;
    sigemptyset(&(act.sa_mask));
    act.sa_flags = 0;
    if (sigaction(SIGINT, &act, NULL) < 0) {
         fprintf(fpo, estilo.sim, sim->tag, estilo.err, "sigaction");
        exit(EXIT_FAILURE);
    }
    fprintf(fpo, estilo.sim, sim->tag, estilo.ok, "Fin de inicialización");
    sem_post(sim->sem_sim);  // avisa al monitor


    
    // Comienzo simulador
    fprintf(fpo, estilo.sim, sim->tag, estilo.ok, "Comenzando simulación");
    sim_run_jefes(sim);
    fprintf(fpo, estilo.sim, sim->tag, estilo.ok, "Esperando a los jefes");
    sim_esperar_jefes(sim);

    
    fprintf(fpo, estilo.sim, sim->tag, estilo.ok,"Fin de simulación");  // quizas ponerlo fuera
    sprintf(out_buffer, "Finalizando %s", sim->tag);
    fprintf(fpo, estilo.ok_msg, out_buffer);


    // Removemos el manejador de señal para evitar errores 
    // mientras liberamos recursos.
    signal(SIGINT, SIG_DFL);
    sem_close(sim->sem_sim);
    sem_unlink(SEM_SIMULADOR); // !!! funciona si se cierra antes que monitor?
    if (args.fichero_out)   
        fclose(fpo);
    exit(EXIT_SUCCESS);
}



// Ejecuta los jefes
void sim_run_jefes(tipo_sim *sim) {      
    int pid = -1;
    tipo_jefe * jefe;
    fprintf(fpo, estilo.sim, sim->tag, estilo.ok, "Inicializando jefes");
    // creacion de jefes
    for (int i = 0; i < N_EQUIPOS; i++) {
        pid = fork();
        if (pid == 0){  // jefe
            jefe = jefe_init(i+1);
            break;
        }
        else if (pid < 0) {
            fprintf(fpo, estilo.sim, sim->tag, estilo.err, "sim_run_jefes");
            exit(EXIT_FAILURE);
        }
    }
    // Resto del codigo de jefes
    if (pid == 0) {
        jefe_run(jefe);
        exit(EXIT_SUCCESS);
    }
}

void sim_esperar_jefes(tipo_sim *sim){
    for (int i = 0; i < N_EQUIPOS; i++)
        wait(NULL);
}
