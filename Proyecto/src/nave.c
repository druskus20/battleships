#include "nave.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "msg.h"

extern tipo_argumentos args;
extern tipo_estilo estilo;
extern FILE * fpo;

tipo_nave * nave_create(int equipo, int id) {
    
    tipo_nave *new_nave;
    char out_buffer[STRING_MAX];

    new_nave = (tipo_nave *)malloc(sizeof(tipo_nave));
   
    new_nave->equipo = equipo;
    new_nave->id = id;
    load_nave_tag(equipo, id, new_nave->tag);
    
    sprintf(out_buffer, "Creando %s", new_nave->tag);
    msg_naveOK(fpo, new_nave, out_buffer);
    
    return new_nave;
}
void nave_init(tipo_nave * nave){
    msg_naveOK(fpo, nave, "Inicializando");
}

void nave_run(tipo_nave *nave){
    msg_naveOK(fpo, nave, "Comenzando");
    sleep(1);
}

void nave_end(tipo_nave * nave){
    msg_naveOK(fpo, nave, "Finalizando");
}

void nave_destroy(tipo_nave *nave){
    char out_buffer[STRING_MAX];
    sprintf(out_buffer, "Destruyendo %s", nave->tag);
    msg_naveOK(fpo, nave, out_buffer);
    free(nave);
    exit(EXIT_SUCCESS);
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
