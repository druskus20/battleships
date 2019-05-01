#include "jefe.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/wait.h>
#include "nave.h"
#include "msg.h" 

extern tipo_argumentos args;
extern tipo_estilo estilo;
extern FILE * fpo;




tipo_jefe * jefe_create(int equipo, int pipe_sim[2]) {
    
    tipo_jefe *new_jefe;
   
    int pipe_status;
    
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
    msg_jefeOK(fpo, jefe, "Ejecutando naves");
    jefe_run_naves(jefe);


}

void jefe_end(tipo_jefe *jefe) {
    msg_jefeOK(fpo, jefe, "Esperando naves");
    jefe_esperar_naves(jefe);
    msg_jefeOK(fpo, jefe, "Finalizando");
    
}
void jefe_destroy(tipo_jefe *jefe){
    char out_buffer[STRING_MAX];
    sprintf(out_buffer, "Destruyendo %s", jefe->tag);
    msg_jefeOK(fpo, jefe, out_buffer);
    exit(EXIT_SUCCESS);

}

void jefe_run_naves(tipo_jefe *jefe){
    int pid = -1;
    tipo_nave * nave;
    msg_jefeOK(fpo, jefe, "Inicializando naves ");
    // creacion de naves
    for (int i = 0; i < N_NAVES; i++) {
        pid = fork();
        if (pid == 0) {  // nave
            nave = nave_create(jefe->equipo, i);
            break;
        }
        else if (pid < 0) {
            msg_jefeERR(fpo, jefe, "jefe_run_naves");
            exit(EXIT_FAILURE);
        }
    }
    // Resto del codigo de jefes
    if (pid == 0) {
        nave_init(nave);
        nave_run(nave);
        nave_end(nave);
        exit(EXIT_SUCCESS);
    }
}

void jefe_esperar_naves(tipo_jefe *jefe) {
    for (int i = 0; i < N_NAVES; i++)
        wait(NULL);
}