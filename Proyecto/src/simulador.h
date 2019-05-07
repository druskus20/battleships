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
extern tipo_estiloMSG estiloMSG;             // Ver msg.c            
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
void sim_mandar_msg_jefe(tipo_sim *sim, int equipo, char msg[MSG_MAX]);
char * sim_recibir_msg_nave(tipo_sim * sim);
void sim_init_cola_nave(tipo_sim * sim);
bool sim_evaluar_fin(tipo_sim * sim);
void sim_init_signal_handlers();
int sim_actua(tipo_sim * sim, int accion_sim, char * extra);
void sim_init_semaforos(tipo_sim * sim);
int parse_accion(char * accion);
void sim_esperar_naves_ready(tipo_sim * sim);
void sim_free_resources(tipo_sim * sim);
void sim_init_mapa_shm(tipo_sim * sim);
void sim_init_shm_readers_count(tipo_sim * sim) ;
    
#endif 
