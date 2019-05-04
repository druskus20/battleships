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

void sim_launch();
tipo_sim * sim_create();
void sim_init(tipo_sim * sim);
void sim_run(tipo_sim * sim); 
void sim_end(tipo_sim * sim); 
void sim_destroy(tipo_sim * sim); 


void sim_run_jefes(tipo_sim *sim);
void sim_esperar_jefes();
void sim_init_pipes_jefes(tipo_sim *sim);
void sim_mandar_msg_jefe(tipo_sim *sim, int equipo);
void sim_recibir_msg_nave(tipo_sim * sim);
void sim_init_cola_nave(tipo_sim * sim);
bool sim_evaluar_fin(tipo_sim * sim);
void sim_inicializar_signal_handlers();
#endif 
