#ifndef SRC_SIMULADOR_H_
#define SRC_SIMULADOR_H_

#include <stdbool.h>
#include "style.h"

/*** SCREEN ***/
#define MAPA_MAXX 20         // Número de columnas del mapa
#define MAPA_MAXY 20         // Número de filas del mapa
#define SCREEN_REFRESH 10000 // Frequencia de refresco del mapa en el monitor
#define SYMB_VACIO '.'       // Símbolo para casilla vacia
#define SYMB_TOCADO '%'      // Símbolo para tocado
#define SYMB_DESTRUIDO 'X'   // Símbolo para destruido
#define SYMB_AGUA 'w'        // Símbolo para agua

/*** SIMULACION ***/
#define VIDA_MAX 50       // Vida inicial de una nave
#define ATAQUE_ALCANCE 20 // Distancia máxima de un ataque
#define ATAQUE_DANO 10    // Daño de un ataque
#define MOVER_ALCANCE 1   // Máximo de casillas a mover
#define TURNO_SECS 5      // Segundos que dura un turno

#define N_EQUIPOS 3 // Número de equipos
#define N_NAVES 3   // Número de naves por equipo

#define MAX_FICHERO_OUT 100
#define STYLE_STRING_L 100

extern char symbol_equipos[N_EQUIPOS]; // Símbolos de los diferentes equipos en el mapa (mirar mapa.c)

/*** ARGUMENTOS ***/
// Argumentos de ejecucion
typedef struct {
	bool F_color;
	bool F_fichero_out;
	char fichero_out[MAX_FICHERO_OUT];
} tipo_argumentos;
// Se usará con una variable "extern" si es necesario

/*** ESTILO ***/
// Formato para los mensajes en la terminal
typedef struct {
	char sim[STYLE_STRING_L];
	char jefe[STYLE_STRING_L];	
	char nave[STYLE_STRING_L];	
	char ok_msg[STYLE_STRING_L];		
	char error_msg[STYLE_STRING_L];
} tipo_estilo;

/*** NAVE ***/
// Información de nave
typedef struct {
	int vida; 	  // Vida que le queda a la nave
	int posx; 	  // Columna en el mapa
	int posy;  	  // Fila en el mapa
	int equipo;   // Equipo de la nave
	int num_nave; // Numero de la nave en el equipo
	bool viva;    // Si la nave está viva o ha sido destruida
} tipo_nave;


/*** MAPA ***/
// Información de una casilla en el mapa
typedef struct {
	char simbolo; // Símbolo que se mostrará en la pantalla para esta casilla
	int equipo;   // Si está vacia = -1. Si no, número de equipo de la nave que está en la casilla
	int num_nave; // Número de nave en el equipo de la nave que está en la casilla
} tipo_casilla;

// Información del mapa
typedef struct {
	tipo_nave info_naves[N_EQUIPOS][N_NAVES];
	tipo_casilla casillas[MAPA_MAXY][MAPA_MAXX];
	int num_naves[N_EQUIPOS]; 					  // Número de naves vivas en un equipo
} tipo_mapa;

#define SHM_MAP_NAME "/shm_naves"

#endif 
