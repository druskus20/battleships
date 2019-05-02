#ifndef SRC_NAVE_H_
#define SRC_NAVE_H_

#include <simulador.h>
#include <stdbool.h>




void nave_launch(int equipo, int num, int pipe_jefe[2]);

tipo_nave * nave_create(int equipo, int num, int pipe_jefe[2]);
void nave_init(tipo_nave * nave);
void nave_run(tipo_nave *nave);
void nave_end(tipo_nave * nave);
void nave_destroy(tipo_nave *nave);


void nave_run_naves(tipo_nave *nave);



int nave_get_vida(tipo_nave *nave);
void nave_set_vida(tipo_nave *nave, int vida);

int nave_get_posx(tipo_nave *nave);
void nave_set_posx(tipo_nave *nave, int posx);

int nave_get_posy(tipo_nave *nave);
void nave_set_posy(tipo_nave *nave, int posy);

int nave_get_equipo(tipo_nave *nave);
void nave_set_equipo(tipo_nave *nave, int equipo);

int nave_get_num(tipo_nave *nave);
void nave_set_num(tipo_nave *nave, int num);

bool nave_is_viva(tipo_nave *nave);
void nave_set_viva(tipo_nave *nave, bool viva);

char * nave_get_tag(tipo_nave *nave);

void nave_set_tag(tipo_nave *nave, char *tag);

int nave_get_dmg(tipo_nave *nave);

void nave_set_dmg(tipo_nave *nave, int dmg); 


int nave_get_alcance(tipo_nave *nave);

void nave_set_alcance(tipo_nave *nave, int alcance);

#endif