#ifndef SRC_MAPA_H_
#define SRC_MAPA_H_

#include <simulador.h>
#include <stdbool.h>


tipo_mapa * mapa_create();

void mapa_destroy(tipo_mapa * mapa);

// Pone una casilla del mapa a vacío
void mapa_clean_casilla(tipo_mapa *mapa, int posy, int posx);

// Obtiene información de una casilla del mapa
tipo_casilla mapa_get_casilla(tipo_mapa *mapa, int posy, int posx);

//Obtiene la distancia entre dos posiciones del mapa
int mapa_get_distancia(tipo_mapa *mapa, int oriy,int orix,int targety,int targetx);

//Obtiene información sobre una nave a partir del equipo y el número de nave
info_nave mapa_get_nave(tipo_mapa *mapa, int equipo, int num_nave);

// Obtiene el número de naves vivas en un equipo
int mapa_get_num_naves(tipo_mapa *mapa, int equipo);

// Obtiene el símbolo de la posición y,x en el mapa
char mapa_get_symbol(tipo_mapa *mapa, int posy, int posx);

// Chequea si la casilla y,x está vacía en el mapa
bool mapa_is_casilla_vacia(tipo_mapa *mapa, int posy, int posx);

// Chequea si el contenido de casilla corresponde a una casilla vacia
bool casilla_is_vacia(tipo_mapa *mapa, tipo_casilla casilla);

// Restaura los símbolos del mapa dejando sólo las naves vivas
void mapa_restore(tipo_mapa *mapa);

// Genera la animación de un misil en el mapa
void mapa_send_misil(tipo_mapa *mapa, int origeny, int origenx, int targety, int targetx);

// Fija el contenido de "nave" en el mapa, en la posición nave.posy, nave.posx
bool mapa_set_nave(tipo_mapa *mapa, info_nave nave);

// Fija el número de naves 'numNaves' para el equipo
void mapa_set_num_naves(tipo_mapa *mapa, int equipo, int numNaves);

// Fija el símbolo 'symbol' en la posición posy, posx del mapa
void mapa_set_symbol(tipo_mapa *mapa, int posy, int posx, char symbol);


// HABLAR EN LA MEMORIA !!!
int mapa_generate_pos_nave( int equipo, int num_nave, int pos[2]);
#endif 
