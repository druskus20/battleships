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

tipo_jefe * jefe; // Creada de forma global para usarla en los manejadores de señal

void jefe_launch(int equipo, int pipe_sim[2]) {
        

        signal(SIGALRM, SIG_DFL);
        signal(SIGINT, SIG_DFL);
        
        jefe = jefe_create(equipo, pipe_sim);
        jefe_init(jefe);
        jefe_run(jefe);
        jefe_end(jefe);
        jefe_destroy(jefe);
        exit(EXIT_SUCCESS);
}

tipo_jefe * jefe_create(int equipo, int pipe_sim[2]) {
    
    tipo_jefe *new_jefe;

    new_jefe = (tipo_jefe *)malloc(sizeof(tipo_jefe));
   
    new_jefe->equipo = equipo;
    new_jefe->pipe_sim = pipe_sim;
    load_jefe_tag(equipo, new_jefe->tag);


    char out_buffer[STRING_MAX];
    sprintf(out_buffer, "Creando %s", new_jefe->tag);
    msg_jefeOK(fpo, new_jefe, out_buffer);
    
    return new_jefe;
}

void jefe_init(tipo_jefe *jefe) {
    msg_jefeOK(fpo, jefe, "Inicializando");
    jefe_init_pipes_naves(jefe);
}

void jefe_run(tipo_jefe *jefe){
    msg_jefeOK(fpo, jefe, "Comenzando");
    jefe_run_naves(jefe); 
    jefe_recibir_msg_sim(jefe);
    jefe_mandar_msg_nave(jefe, 0);
    jefe_mandar_msg_nave(jefe, 1);
    jefe_mandar_msg_nave(jefe, 2);
    jefe_mandar_msg_nave(jefe, 3);
}

void jefe_end(tipo_jefe *jefe) {
    msg_jefeOK(fpo, jefe, "Esperando naves");
    jefe_esperar_naves(jefe);
    
    
}
void jefe_destroy(tipo_jefe *jefe){
    char out_buffer[STRING_MAX];
    sprintf(out_buffer, "Destruyendo %s", jefe->tag);
    msg_jefeOK(fpo, jefe, out_buffer);
    free(jefe);
}

void jefe_run_naves(tipo_jefe *jefe){
    int pid = -1;
    msg_jefeOK(fpo, jefe, "Ejecutando naves");
    // creacion de naves
    for (int i = 0; i < N_NAVES; i++) {
        pid = fork();
        if (pid == 0) {  // nave
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
    char out_buffer[STRING_MAX];
    char msg_buffer[MSG_MAX];
    int * fd; // pipe

    load_sim_tag(tag);
    sprintf(out_buffer, "Esperando mensaje de %s", tag);
    msg_jefeOK(fpo, jefe, out_buffer);
    fd = jefe->pipe_sim;
    // cierra el descriptor de salida en el sim
    close(fd[1]); 
    read(fd[0], msg_buffer, MSG_MAX);
    sprintf(out_buffer, "Recibido mensaje: %s", msg_buffer);
    msg_jefeOK(fpo, jefe, out_buffer);
}

void jefe_mandar_msg_nave(tipo_jefe *jefe, int num_nave) {
    char tag[TAG_MAX];
    char out_buffer[STRING_MAX];
    char msg_buffer[MSG_MAX];
    int * fd; // pipe

    load_nave_tag(jefe->equipo, num_nave, tag);
    sprintf(out_buffer, "Avisando a nave %s", tag);
    msg_jefeOK(fpo, jefe, out_buffer);
    fd = jefe->pipes_naves[num_nave];
    // cierra el descriptor de entrada en el jefe
    close(fd[0]); 
    sprintf(msg_buffer, "HOLA %s", tag);
    write(fd[1], msg_buffer, MSG_MAX); // !!! quiza msg_max+1. pero al leer podría fallar por pasarse de tamaño
}