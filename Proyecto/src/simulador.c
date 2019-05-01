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
#include <errno.h>
#include <unistd.h>

// Para la lectura de argumentos
#include <getopt.h>

// !!! no estoy muy seguro de esto
#include "mapa.h"
#include "jefe.h"
#include "nave.h"


// Lee las flags y argumentos introducidos y modifica los parametros
// de ejecucion necesarios
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
                estilo_set_colorful();
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

// Manejador de la señal Ctrl+C (SIGINT), con mensajes
void sim_manejador_SIGINT(int sig) {
    fprintf(stdout, "\n");
    // msg_OK(stdout, "SIGINT SIM"); da error por variables globales
    msg_signal("Finalizando ejecucion...");
    fflush(stdout);
    sleep(1);
    exit(EXIT_SUCCESS);
}



// Manejador de la señal Ctrl+C (SIGINT)
void sim_manejador_SIGALRM(int sig) {
    char out_buffer[STRING_MAX];
    sprintf(out_buffer, "Nuevo %s", estilo.turno_tag);
    msg_OK(fpo, out_buffer);
}

// Establece los parametros de ejecucion por defecto
void set_default_params() {
    args.F_fichero_out = false;
    strcpy(args.fichero_out, "");
    fpo = stdout;
    args.F_color = false;
    estilo_set_default();
}

// Abre un fichero de log
void abrir_log() {
    char out_buffer[STRING_MAX];
    fpo = fopen(args.fichero_out, "w");
    if  (!fpo) {
        sprintf(out_buffer, "No se ha podido abrir el fichero: %s", args.fichero_out);
        msg_ERR(stdout, out_buffer);
        exit(EXIT_FAILURE);
    }
}



// Rutina principal
int main(int argc, char **argv) {
    
    struct sigaction act_sigint, act_sigalrm;
    char out_buffer[STRING_MAX];
    sem_t *sem_sim; // semaforo monitor-simulador	
    tipo_sim  * sim; 
                     
    set_default_params();
    leer_argumentos(argc, argv); 
    if (args.F_fichero_out) 
        abrir_log();
    sim = sim_create();

    // Inicializacion de semaforo simulador
    if((sem_sim = sem_open(SEM_SIMULADOR, O_CREAT, S_IRUSR | S_IWUSR, 0)) == SEM_FAILED){
        msg_simERR(fpo, "sem_open de ""sem_sim""");
		exit(EXIT_FAILURE);
	}  

    // Inicializacion del manejador SIGINT
    act_sigint.sa_handler = sim_manejador_SIGINT;
    sigemptyset(&(act_sigint.sa_mask));
    act_sigint.sa_flags = 0;
    if (sigaction(SIGINT, &act_sigint, NULL) < 0) {
        msg_simERR(fpo, "sigaction de SIGINT");
        exit(EXIT_FAILURE);
    }

    // Inicializacion del manejador SIGALRM
    act_sigalrm.sa_handler = sim_manejador_SIGALRM;
    sigemptyset(&(act_sigalrm.sa_mask));
    act_sigalrm.sa_flags = 0;
    if (sigaction(SIGALRM, &act_sigalrm, NULL) < 0) {
        sprintf(out_buffer, "sigaction de SIGALRM: %s", strerror(errno));
        msg_simERR(fpo, out_buffer);
        exit(EXIT_FAILURE);
    }  


    // alarm(TURNO_INTERVAL);  
   
    sim_init(sim);
    sem_post(sem_sim);   // avisa al monitor
    sim_run(sim);
    sleep(10);
    sim_end(sim);
    
    sim_destroy(sim);

    // Elimina el manejador sigint
    signal(SIGINT, SIG_DFL);
    sem_close(sem_sim);
    sem_unlink(SEM_SIMULADOR); // !!! funciona si se cierra antes que monitor?
    if (args.fichero_out)   
        fclose(fpo);

    return EXIT_SUCCESS;
}



tipo_sim * sim_create() {
    tipo_sim * sim;
    char out_buffer[STRING_MAX];


    sim = (tipo_sim *)malloc(sizeof(tipo_sim));
    load_sim_tag(sim->tag);

    sprintf(out_buffer, "Creando %s", sim->tag);
    msg_simOK(fpo, out_buffer);
    return sim;
}

void sim_init(tipo_sim * sim) {
    msg_simOK(fpo, "Inicializando");
    sim_init_pipes_jefes(sim);
}

void sim_run(tipo_sim * sim) {
    // Comienzo simulador
    msg_simOK(fpo, "Comenzando");
    sleep(1);

    sim_run_jefes(sim);
}

void sim_end(tipo_sim * sim) {
    sim_esperar_jefes(sim);
    msg_simOK(fpo, "Finalizando");  // quizas ponerlo fuera
}

void sim_destroy(tipo_sim * sim) {
    char out_buffer[STRING_MAX];
    sprintf(out_buffer, "Destruyendo %s", sim->tag);
    msg_simOK(fpo, out_buffer);    
    free(sim);
}

// Inicializa pipes a jefes
void sim_init_pipes_jefes(tipo_sim * sim) { 
    msg_simOK(fpo, "Inicializando pipes");
    int pipe_status;
    for (int i = 0; i < N_EQUIPOS; i++){
        pipe_status = pipe(sim->pipes_jefes[i]);
        if (pipe_status == -1){
            msg_simERR(fpo, "sim_init_pipes_jefes");
		    exit(EXIT_FAILURE);
        }
    }
}

// Ejecuta los jefes
void sim_run_jefes(tipo_sim *sim) {      
    int pid = -1;
    struct sigaction act; 
    tipo_jefe * jefe;
    int i;
    msg_simOK(fpo, "Ejecutando jefes");
    // creacion de jefes
    for (i = 0; i < N_EQUIPOS; i++) {
        pid = fork();
        if (pid == 0) {  // jefe
            jefe_launch(i, sim->pipes_jefes[i]);
            break;  
        }
        else if (pid < 0) {
            msg_simERR(fpo, "sim_run_jefes");
            exit(EXIT_FAILURE);
        }   
    }
}

void sim_esperar_jefes(tipo_sim *sim) {
    msg_simOK(fpo, "Esperando jefes");
    for (int i = 0; i < N_EQUIPOS; i++)
        wait(NULL);
}

