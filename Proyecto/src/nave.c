#include "nave.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <style.h>

extern tipo_argumentos args;
extern tipo_estilo estilo;
extern FILE * fpo;


tipo_nave * nave_init(int equipo, int id) {
    
    tipo_nave *new_nave;
    char out_buffer[STRING_MAX];
    
    new_nave = (tipo_nave *)malloc(sizeof(tipo_nave));
   
    new_nave->equipo = equipo;
    new_nave->id = id;
    sprintf(new_nave->tag,  estilo.nave_tag, new_nave->equipo, new_nave->id); // !!!
    
    // !!! jefe_crear_naves aqui o en simulador
    sprintf(out_buffer, "Iniciando %s", new_nave->tag);

    fprintf(fpo, estilo.nave, new_nave->tag, estilo.ok, out_buffer);
    return new_nave;
}

void nave_run(tipo_nave *nave){
   // fprintf(fpo, estilo.nave, nave->tag, estilo.ok, "Running");
}

void nave_destroy(tipo_nave *nave){
    fprintf(fpo, estilo.nave, nave->tag, estilo.ok, "Destruida");
}


char * nave_get_tag(tipo_nave *nave) {
    return nave->tag;
}

void nave_set_tag(tipo_nave *nave, char *tag) {
    strcpy(nave->tag, tag);
}

int nave_get_dmg(tipo_nave *nave) {
    return nave->dmg;
}


void nave_set_dmg(tipo_nave *nave, int dmg) {
    nave->dmg = dmg;
}


int nave_get_alcance(tipo_nave *nave) {
    return nave->alcance;
}


void nave_set_alcance(tipo_nave *nave, int alcance) {
    nave->alcance = alcance;
}


int nave_get_vida(tipo_nave *nave) {
    return nave->vida;
}
void nave_set_vida(tipo_nave *nave, int vida) {
    nave->vida = vida;
}

int nave_get_posx(tipo_nave *nave) {
    return nave->posx;
}
void nave_set_posx(tipo_nave *nave, int posx) {
    nave->posx = posx;
}

int nave_get_posy(tipo_nave *nave) {
    return nave->posy;
}
void nave_set_posy(tipo_nave *nave, int posy) {
    nave->posy = posy;
}

int nave_get_equipo(tipo_nave *nave) {
    return nave->equipo;
}
void nave_set_equipo(tipo_nave *nave, int equipo) {
    nave->equipo = equipo;
}

int nave_get_num_nave(tipo_nave *nave) {
    return nave->num_nave;
}
void nave_set_num_nave(tipo_nave *nave, int num_nave) {
    nave->num_nave = num_nave;
}

bool nave_is_viva(tipo_nave *nave) {
    return nave->viva;
}
void nave_set_viva(tipo_nave *nave, bool viva){
    nave->viva = viva;
}
