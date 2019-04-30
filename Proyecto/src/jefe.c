#include "jefe.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <style.h>
#include <unistd.h>

extern tipo_argumentos args;
extern tipo_estilo estilo;
extern FILE * fpo;

tipo_jefe * jefe_init() {
    tipo_jefe *new_jefe;
    new_jefe = (tipo_jefe *)malloc(sizeof(tipo_jefe));
    return new_jefe;
}

void jefe_destroy(tipo_jefe *jefe){

}


// !!! cambiar retornos y arreglar una vez funcione jefe
tipo_nave * jefe_crear_naves(tipo_jefe *jefe) {      
    int pid = -1;
     // !!! Inicializar tipo_jefe fuera o dentro del bucle. 
    for (int i = 0; i < N_NAVES; i++) {
        pid = fork();
        switch (pid) {
            case CHILD:
                break;
            case FATHER:
                break;
            default:
                fprintf(fpo, estilo.error_msg, "sim_crear_jefes");
                exit(EXIT_FAILURE);
        }   
    }
    return NULL;
}