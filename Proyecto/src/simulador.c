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
                strcpy(estilo.ok_status, OK_SC);
                strcpy(estilo.err_status, ERROR_SC);
                strcpy(estilo.nave_tag, NAVE_C);
                strcpy(estilo.jefe_tag, JEFE_C);
                strcpy(estilo.sim_tag, SIM_C);    
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
                sprintf(out_buffer, "Ejecucion: %s <-f fichero_log> <-c>", argv[0]);
                msg_ERR(fpo, out_buffer);
				exit(EXIT_FAILURE);
				break;
		}
	}
}


// Manejador de la señal Ctrl+C (SIGINT)
void manejador_SIGINT(int sig) {
    fprintf(fpo, "\n");
    msg_OK(fpo, "Ending the program");
    if (args.fichero_out)   
        fclose(fpo);
    exit(EXIT_SUCCESS);
}


int main(int argc, char **argv) {
    
    struct sigaction act;
    char out_buffer[STRING_MAX];
    
    tipo_sim  * sim; 
                     
    // Inicializacion de parametros por defecto
    args.F_fichero_out = false;
    strcpy(args.fichero_out, "");
    fpo = stdout;

    args.F_color = false;
    strcpy(estilo.std_msg, STD_MSG);
    strcpy(estilo.status_msg, STATUS_MSG);
    strcpy(estilo.ok_status, OK_S);
    strcpy(estilo.err_status, ERROR_S);
    strcpy(estilo.nave_tag, NAVE);
    strcpy(estilo.jefe_tag, JEFE);
    strcpy(estilo.sim_tag, SIM);
    
    leer_argumentos(argc, argv);

    // Apertura de fichero de log
    if (args.F_fichero_out) {
        fpo = fopen(args.fichero_out, "w");
        if  (!fpo) {
            sprintf(out_buffer, "No se ha podido abrir el fichero: %s", args.fichero_out);
            msg_ERR(stdout, out_buffer);
            exit(EXIT_FAILURE);
        }
    }

    sim = sim_create();

    // Inicializacion del manejador SIGINT
    act.sa_handler = manejador_SIGINT;
    sigemptyset(&(act.sa_mask));
    act.sa_flags = 0;
    if (sigaction(SIGINT, &act, NULL) < 0) {
        msg_simERR(fpo, sim, "sigaction");
        exit(EXIT_FAILURE);
    }
    
    // Inicializacion de semaforo simulador
    if((sim->sem_sim = sem_open(SEM_SIMULADOR, O_CREAT, S_IRUSR | S_IWUSR, 0)) == SEM_FAILED){
        msg_simERR(fpo, sim, "sem_open de ""sem_sim""");
		exit(EXIT_FAILURE);
	}  

    sim_init(sim);
    sem_post(sim->sem_sim);  // avisa al monitor
    sim_run(sim);
    sim_end(sim);

    // Removemos el manejador de señal para evitar errores 
    // mientras liberamos recursos.
    signal(SIGINT, SIG_DFL);
    sem_close(sim->sem_sim);
    sem_unlink(SEM_SIMULADOR); // !!! funciona si se cierra antes que monitor?
    if (args.fichero_out)   
        fclose(fpo);
    
    sim_destroy(sim);
    return 0;
}

tipo_sim * sim_create() {
    tipo_sim * sim;
    char out_buffer[STRING_MAX];

    sim = (tipo_sim *)malloc(sizeof(tipo_sim));
    load_sim_tag(sim->tag);

    sprintf(out_buffer, "Creando %s", sim->tag);
    msg_simOK(fpo, sim, out_buffer);
    return sim;
}

void sim_init(tipo_sim * sim) {
    msg_simOK(fpo, sim, "Inicializando");
    sim_init_pipes_jefes(sim);
}

void sim_run(tipo_sim * sim) {
    // Comienzo simulador
    msg_simOK(fpo, sim, "Comenzando");
    msg_simOK(fpo, sim, "Ejecutando jefes");
    sim_run_jefes(sim);

}

void sim_end(tipo_sim * sim) {
    msg_simOK(fpo, sim, "Esperando jefes");
    sim_esperar_jefes(sim);
    msg_simOK(fpo, sim, "Finalizando");  // quizas ponerlo fuera
}

void sim_destroy(tipo_sim * sim) {
    char out_buffer[STRING_MAX];
    sprintf(out_buffer, "Destruyendo %s", sim->tag);
    msg_simOK(fpo, sim, out_buffer);    
    free(sim);
    exit(EXIT_SUCCESS);
}

// Inicializa pipes a jefes
void sim_init_pipes_jefes(tipo_sim * sim) { 
    int pipe_status;
    for (int i = 0; i < N_EQUIPOS; i++){
        pipe_status = pipe(sim->pipes_jefes[i]);
        if (pipe_status == -1){
            msg_simERR(fpo, sim, "sim_init_pipes_jefes");
		    exit(EXIT_FAILURE);
        }
    }
}

// Ejecuta los jefes
void sim_run_jefes(tipo_sim *sim) {      
    int pid = -1;
    tipo_jefe * jefe;
    msg_simOK(fpo, sim, "Inicializando jefes");
    // creacion de jefes
    for (int i = 0; i < N_EQUIPOS; i++) {
        pid = fork();
        if (pid == 0){  // jefe
            jefe = jefe_create(i, sim->pipes_jefes[i]);
            break;
        }
        else if (pid < 0) {
            msg_simERR(fpo, sim, "sim_run_jefes");
            exit(EXIT_FAILURE);
        }
    }
    // Resto del codigo de jefes
    if (pid == 0) {
        jefe_init(jefe);
        jefe_run(jefe);
        jefe_end(jefe);
        exit(EXIT_SUCCESS);
    }
}

void sim_esperar_jefes(tipo_sim *sim) {
    for (int i = 0; i < N_EQUIPOS; i++)
        wait(NULL);
}

