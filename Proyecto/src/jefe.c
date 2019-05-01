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


void jefe_manejador_SIGINT(int sig) {
    exit(EXIT_SUCCESS);
}


void jefe_launch(int equipo, int pipe_sim[2]) {
        tipo_jefe * jefe;
        struct sigaction act;

        signal(SIGALRM, SIG_DFL);

        // Establece el manejador de sigint especifico del jefe
        
        act.sa_handler = jefe_manejador_SIGINT;
        sigemptyset(&(act.sa_mask));
        //sigaddset(&act.sa_mask, SIGALRM); !!! No hace falta porque esta puesta "por defecto"
        act.sa_flags = 0;
        if (sigaction(SIGINT, &act, NULL) < 0) {
            msg_ERR(fpo, "sigaction de SIGINT en jefe_launch");
            exit(EXIT_FAILURE);
        }

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

    // La pipe ya esta abierta
    // pipe_status = pipe(new_jefe->pipe_sim);
    //if (pipe_status == -1) {
    //    fprintf(fpo, estilo.jefe, new_jefe->tag, estilo.ok, "jefe_init/pipe");
    //    exit(EXIT_FAILURE);
    //}
    
    // !!! jefe_crear_naves aqui o en simulador
    char out_buffer[STRING_MAX];
    sprintf(out_buffer, "Creando %s", new_jefe->tag);
    msg_jefeOK(fpo, new_jefe, out_buffer);
    
    return new_jefe;
}

void jefe_init(tipo_jefe *jefe) {
    msg_jefeOK(fpo, jefe, "Inicializando");
}

void jefe_run(tipo_jefe *jefe){
    msg_jefeOK(fpo, jefe, "Comenzando");
    sleep(1);
    jefe_run_naves(jefe);
}

void jefe_end(tipo_jefe *jefe) {
    
    jefe_esperar_naves(jefe);
    msg_jefeOK(fpo, jefe, "Finalizando");
    
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
            nave_launch(jefe->equipo, i);
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