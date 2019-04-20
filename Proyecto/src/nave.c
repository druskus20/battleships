#include "nave.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <style.h>

extern tipo_argumentos args;
extern tipo_estilo estilo;
extern FILE * fpo;

bool nave_start(){
    //FILE * fp = fopen(args.fichero_out, "w");
    fprintf(fpo, estilo.nave, "a", "1", estilo.ok, "Starting");
    
    fprintf(fpo, estilo.ok_msg, "Color desactivado");

    return true;
}

bool nave_end(){
    fprintf(fpo, estilo.ok_msg, "Color test");
    fprintf(fpo, estilo.nave, "a", "1", estilo.ok, "Ending");
    return true;
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
