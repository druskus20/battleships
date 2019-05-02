#ifndef SRC_SIMULADOR_H_
#define SRC_SIMULADOR_H_

#include <stdbool.h>
#include <stdio.h>
#include <semaphore.h>
#include "msg.h"
#include "types.h"

// variables globales 
// !!! por que solo funcionan aqui??
tipo_argumentos args;
FILE * fpo;      
extern tipo_estilo estilo;             // Ver msg.c            
extern char symbol_equipos[N_EQUIPOS]; // Ver mapa.c


tipo_sim * sim_create();
void sim_init(tipo_sim * sim);
void sim_run(tipo_sim * sim); 
void sim_end(tipo_sim * sim); 
void sim_destroy(tipo_sim * sim); 


void sim_run_jefes(tipo_sim *sim);
void sim_esperar_jefes(tipo_sim *sim);
void sim_init_pipes_jefes(tipo_sim *sim);
void sim_mandar_msg_jefe(tipo_sim *sim, int equipo);
#endif 
