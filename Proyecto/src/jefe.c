#include "jefe.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <style.h>
#include <unistd.h>
#include "nave.h"

extern tipo_argumentos args;
extern tipo_estilo estilo;
extern FILE * fpo;

tipo_jefe * jefe_init(int equipo, int pipe_sim[2]) {
    
    tipo_jefe *new_jefe;
    char out_buffer[STRING_MAX];
    int pipe_status;
    
    new_jefe = (tipo_jefe *)malloc(sizeof(tipo_jefe));
   
    new_jefe->equipo = equipo;
    sprintf(new_jefe->tag,  estilo.jefe_tag, new_jefe->equipo); // !!!
    new_jefe->pipe_sim = pipe_sim;
    
    // La pipe ya esta abierta
    // pipe_status = pipe(new_jefe->pipe_sim);
    //if (pipe_status == -1) {
    //    fprintf(fpo, estilo.jefe, new_jefe->tag, estilo.ok, "jefe_init/pipe");
    //    exit(EXIT_FAILURE);
    //}
    
    // !!! jefe_crear_naves aqui o en simulador
    sprintf(out_buffer, "Iniciando %s", new_jefe->tag);

    fprintf(fpo, estilo.jefe, new_jefe->tag, estilo.ok, "Init");
    return new_jefe;
}

void jefe_run(tipo_jefe *jefe){
    //fprintf(fpo, estilo.jefe, jefe->tag, estilo.ok, "Running");
    jefe_run_naves(jefe);
}

void jefe_destroy(tipo_jefe *jefe){
    fprintf(fpo, estilo.jefe, jefe->tag, estilo.ok, "Destruido");
}

void jefe_run_naves(tipo_jefe *jefe){
    int pid = -1;
    tipo_nave * nave;
    fprintf(fpo, estilo.jefe, jefe->tag, estilo.ok, "Inicializando naves");
    // creacion de naves
    for (int i = 0; i < N_EQUIPOS; i++) {
        pid = fork();
        if (pid == 0) {  // nave
            nave = nave_init(jefe->equipo, i);
            break;
        }
        else if (pid < 0) {
            fprintf(fpo, estilo.nave, jefe->tag, estilo.err, "jefe_run_naves");
            exit(EXIT_FAILURE);
        }
    }
    // Resto del codigo de jefes
    if (pid == 0) {
        nave_run(nave);
        exit(EXIT_SUCCESS);
    }
}