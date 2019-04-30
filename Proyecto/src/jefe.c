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

tipo_jefe * jefe_init(int equipo) {
    
    tipo_jefe *new_jefe;
    new_jefe = (tipo_jefe *)malloc(sizeof(tipo_jefe));
   
    new_jefe->equipo = equipo;
    
   
    sprintf(new_jefe->tag,  estilo.jefe_tag, new_jefe->equipo); // !!!
    
    // !!! jefe_crear_naves aqui o en simulador
       
    fprintf(fpo, estilo.jefe, new_jefe->tag, estilo.ok, "Init");
    return new_jefe;
}

void jefe_run(tipo_jefe *jefe){
    fprintf(fpo, estilo.jefe, jefe->tag, estilo.ok, "Running");
}

void jefe_destroy(tipo_jefe *jefe){

}

