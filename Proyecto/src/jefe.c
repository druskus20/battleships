#include "jefe.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include "nave.h"
#include "msg.h" 

extern tipo_argumentos args;
extern tipo_estilo estilo;
extern FILE * fpo;

tipo_jefe * jefe_global; // Creada de forma global para usarla en los manejadores de señal

// Manejador de la señal Ctrl+C (SIGINT)
void jefe_manejador_SIGINT(int sig) {
    fprintf(stdout, "\n");
    // msg_OK(stdout, "SIGINT SIM"); da error por variables globales
    msg_jefeOK(stdout, jefe_global, "Finalizando ejecucion...");
    jefe_end(jefe_global);
    jefe_destroy(jefe_global);
    fflush(fpo);
    exit(EXIT_SUCCESS);
}

void jefe_launch(int equipo, int *pipe_sim) {     
        jefe_global = jefe_create(equipo, pipe_sim);
        jefe_init(jefe_global);
        jefe_run(jefe_global);

        // Elimina el manejador sigint antes de liberar
        signal(SIGINT, SIG_DFL); // CAMBIAR !!!

        jefe_end(jefe_global);
        jefe_destroy(jefe_global);
        exit(EXIT_SUCCESS);
}


tipo_jefe * jefe_create(int equipo, int *pipe_sim) {
    
    tipo_jefe *new_jefe;

    new_jefe = (tipo_jefe *)malloc(sizeof(new_jefe[0]));

    new_jefe->equipo = equipo;
    new_jefe->pipe_sim = pipe_sim;
    load_jefe_tag(equipo, new_jefe->tag);


    char out_buff[BUFF_MAX];
    sprintf(out_buff, "Creando %s", new_jefe->tag);
    msg_jefeOK(fpo, new_jefe, out_buff);
    
    return new_jefe;
}

void jefe_init(tipo_jefe *jefe) {
    msg_jefeOK(fpo, jefe, "Inicializando");
    jefe_inicializar_signal_handlers(jefe);
    jefe_init_pipes_naves(jefe);
}

void jefe_run(tipo_jefe *jefe){
    msg_jefeOK(fpo, jefe, "Comenzando");
    jefe_run_naves(jefe); 
    jefe_recibir_msg_sim(jefe);
    sleep(10);  // sigpipe err, porq el sim acaba antes q el jefe !!!
    for (int i = 0; i < N_NAVES; i++)
        jefe_mandar_msg_nave(jefe, i);
}

void jefe_end(tipo_jefe *jefe) {
    
    jefe_esperar_naves(jefe);
    
    
}
void jefe_destroy(tipo_jefe *jefe){
    char out_buff[BUFF_MAX];
    sprintf(out_buff, "Destruyendo %s", jefe->tag);
    msg_jefeOK(fpo, jefe, out_buff);
    free(jefe);
}

void jefe_run_naves(tipo_jefe *jefe){
    int pid = -1;
    msg_jefeOK(fpo, jefe, "Ejecutando naves");
    // creacion de naves
    for (int i = 0; i < N_NAVES; i++) {
        pid = fork();
        if (pid == 0) {  // nave
            //free(jefe); // !!!!!!!
            signal(SIGINT, SIG_DFL); // Momentaneamente desactivamos el manejador !!! 
            nave_launch(jefe->equipo, i, jefe->pipes_naves[i]);
            break;
        }
        else if (pid < 0) {
            msg_jefeERR(fpo, jefe, "jefe_run_naves");
            exit(EXIT_FAILURE);
        }
    }
}

void jefe_esperar_naves(tipo_jefe *jefe) {
    msg_jefeOK(fpo, jefe, "Esperando naves");
    for (int i = 0; i < N_NAVES; i++)
        wait(NULL);
}

void jefe_init_pipes_naves(tipo_jefe * jefe) { 
    msg_jefeOK(fpo, jefe,"Inicializando pipes a naves");
    int pipe_status;
    for (int i = 0; i < N_NAVES; i++){
        pipe_status = pipe(jefe->pipes_naves[i]);
        if (pipe_status == -1){
            msg_jefeERR(fpo, jefe, "jefe_init_pipes_naves");
		    exit(EXIT_FAILURE);
        }
    }
}


void jefe_recibir_msg_sim(tipo_jefe *jefe) {
    char tag[TAG_MAX];
    char out_buff[BUFF_MAX];
    char msg_buffer[MSG_MAX] =  ""; // !!! solucciona "error uninitialised value"
    int * fd; // pipe

    load_sim_tag(tag);
    sprintf(out_buff, "Esperando mensaje de %s", tag);
    msg_jefeOK(fpo, jefe, out_buff);
    fd = jefe->pipe_sim;
    // cierra el descriptor de salida en el sim
    close(fd[1]); 
    read(fd[0], msg_buffer, MSG_MAX);
    sprintf(out_buff, "Recibido mensaje: %s", msg_buffer);
    msg_jefeOK(fpo, jefe, out_buff);
}

void jefe_mandar_msg_nave(tipo_jefe *jefe, int num_nave) {
    char tag[TAG_MAX];
    char out_buff[BUFF_MAX];
    char msg_buffer[MSG_MAX];
    int * fd; // pipe

    load_nave_tag(jefe->equipo, num_nave, tag);
    sprintf(out_buff, "Mandando mensaje a %s", tag);
    msg_jefeOK(fpo, jefe, out_buff);
    fd = jefe->pipes_naves[num_nave];
    // cierra el descriptor de entrada en el jefe
    close(fd[0]); 
    int len = sprintf(msg_buffer, "HOLA %s", tag);
    write(fd[1], msg_buffer, len); // !!! quiza msg_max+1. pero al leer podría fallar por pasarse de tamaño
}

bool jefe_evaluar_fin(tipo_jefe * jefe) {
    if (jefe->naves_res == 0)
        return true;
    return false;
}




void jefe_inicializar_signal_handlers(tipo_jefe * jefe) {
    struct sigaction act_sigint; // !!! esto se puede hacer en una sub-funcion?
    msg_jefeOK(fpo, jefe, "Inicializando manejadores de señal");
    // Inicializacion del manejador SIGINT
    act_sigint.sa_handler = jefe_manejador_SIGINT;
    sigemptyset(&(act_sigint.sa_mask));
    sigaddset(&act_sigint.sa_mask, SIGALRM);
    act_sigint.sa_flags = 0;
    if (sigaction(SIGINT, &act_sigint, NULL) < 0) {
        msg_jefeERR(fpo, jefe, "sigaction de SIGINT");
        exit(EXIT_FAILURE);
    }
    // !!!!!!!!!!!!!
    // En la memoria comentar por que esta aqui, (mensajes del manejador, logica)) 
}

