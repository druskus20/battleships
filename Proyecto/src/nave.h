#ifndef SRC_NAVE_H_
#define SRC_NAVE_H_

#include <simulador.h>
#include <stdbool.h>

int nave_get_vida(tipo_nave *nave);
void nave_set_vida(tipo_nave *nave, int vida);

int nave_get_posx(tipo_nave *nave);
void nave_set_posx(tipo_nave *nave, int posx);

int nave_get_posy(tipo_nave *nave);
void nave_set_posy(tipo_nave *nave, int posy);

int nave_get_equipo(tipo_nave *nave);
void nave_set_equipo(tipo_nave *nave, int equipo);

int nave_get_num_nave(tipo_nave *nave);
void nave_set_num_nave(tipo_nave *nave, int num_nave);

bool nave_is_viva(tipo_nave *nave);
void nave_set_viva(tipo_nave *nave, bool viva);

#endif