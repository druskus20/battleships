#include "nave.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <style.h>

extern tipo_argumentos args;
extern tipo_estilo estilo;
extern FILE * fpo;

tipo_nave * nave_init(){
    tipo_nave * new_nave;
    new_nave = (tipo_nave *)malloc(sizeof(tipo_nave));
    sprintf(new_nave->tag,  estilo.nave_tag, "a", "2"); // !!!
    fprintf(fpo, estilo.nave, new_nave->tag, estilo.ok, "Llega");
    return new_nave;
}

bool nave_destroy(tipo_nave *nave){
    fprintf(fpo, estilo.nave, nave->tag, estilo.ok, "Destruida");
    return true;
}

/*
bool nave_atacar(tipo_nave *nave, tipo_nave *nave_obj){
    if (nave_alcanza(nave, nave_obj) ){
        if ((rand()%100) < nave->precision) {
            nave_recibir_ataque(nave_obj, nave->dmg);

            // !!! sprintf
            fprintf(fpo, estilo.nave, nave->tag, estilo.ok,
                 "Inflinge %d daños a %s", nave->dmg, nave_obj->tag);
            return true;
        } 
        else {
            fprintf(fpo, estilo.nave, nave->tag, estilo.ok, 
                "Falla al atacar a %s", nave_obj->tag);
            return false;

        }
    }
    else {
        fprintf(fpo, estilo.nave, nave->tag, estilo.ok, 
            "No alcanza a atacar a %s", nave_obj->tag);
        return false;
    }
}

bool nave_alcanza(tipo_nave *nave, tipo_nave *nave_obj) {
    
    //    . . . . t
    //    . . . . .
    //    n . . . t    
    //    n deberia alcanzar a ambos objetivos con "alcance = 4"
    //    4 >= |0 - 4| y 4 > |3 - 0| 
    //    es decir, se cuenta tambien en diagonal

    if  ((nave->alcance >= abs(nave->posx - nave_obj->posx)) 
        && (nave->alcance >= abs(nave->posx - nave_obj->posy)))
        return true;
    return false;
}

void nave_recibir_ataque(tipo_nave *nave, int dmg){
    nave->vida -= dmg;
    fprintf(fpo, estilo.nave, nave->tag, estilo.ok,
            "Tiene %d vida restante", nave->vida);

} 
*/

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
