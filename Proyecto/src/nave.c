#include "nave.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include "msg.h"

extern tipo_argumentos args;
extern tipo_estilo estilo;
extern FILE * fpo;

tipo_nave * nave; // Creada de forma global para usarla en los manejadores de señal


void nave_launch(int equipo, int num, int pipe_jefe[2]) {
   

    nave = nave_create(equipo, num, pipe_jefe);
    nave_init(nave);
    nave_run(nave);
    nave_end(nave);
    nave_destroy(nave);
    exit(EXIT_SUCCESS);
}
tipo_nave * nave_create(int equipo, int num, int pipe_jefe[2]) {
    
    tipo_nave *new_nave;
    char out_buffer[STRING_MAX];

    new_nave = (tipo_nave *)malloc(sizeof(tipo_nave));
   
    new_nave->equipo = equipo;
    new_nave->num = num;
    new_nave->pipe_jefe = pipe_jefe;
    load_nave_tag(equipo, num, new_nave->tag);
    
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
    nave_recibir_msg_jefe(nave);
}

void nave_end(tipo_nave * nave){
    msg_naveOK(fpo, nave, "Finalizando");
}

void nave_destroy(tipo_nave *nave){
    char out_buffer[STRING_MAX];
    sprintf(out_buffer, "Destruyendo %s", nave->tag);
    msg_naveOK(fpo, nave, out_buffer);
    free(nave);
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

int nave_get_num(tipo_nave *nave) {
    return nave->num;
}
void nave_set_num(tipo_nave *nave, int num) {
    nave->num = num;
}

bool nave_is_viva(tipo_nave *nave) {
    return nave->viva;
}
void nave_set_viva(tipo_nave *nave, bool viva){
    nave->viva = viva;
}


void nave_recibir_msg_jefe(tipo_nave *nave) {
    char tag[TAG_MAX];
    char out_buffer[STRING_MAX];
    char msg_buffer[MSG_MAX];
    int * fd; // pipe

    load_jefe_tag(nave->equipo, tag);
    sprintf(out_buffer, "Esperando mensaje de %s", tag);
    msg_naveOK(fpo, nave, out_buffer);
    fd = nave->pipe_jefe;
    // cierra el descriptor de salida en el sim
    close(fd[1]); 
    read(fd[0], msg_buffer, MSG_MAX);
    sprintf(out_buffer, "Recibido mensaje: %s", msg_buffer);
    msg_naveOK(fpo, nave, out_buffer);
}
