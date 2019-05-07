#ifndef SRC_PSIM_H_
#define SRC_PSIM_H_

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


proc_sim * sim_create();
void sim_init(proc_sim * sim);
void sim_run(proc_sim * sim); 
void sim_end(proc_sim * sim); 
void sim_destroy(proc_sim * sim); 

// Libera los recursos de tipo cola, memoria compartida etc
void sim_free_resources(proc_sim * sim);
void sim_run_jefes(proc_sim *sim);
void sim_esperar_jefes();
void sim_init_pipes_jefes(proc_sim *sim);
void sim_mandar_msg_jefe(proc_sim *sim, int equipo, char msg[MSG_MAX]);
char * sim_recibir_msg_nave(proc_sim * sim);
void sim_init_cola_nave(proc_sim * sim);
bool sim_evaluar_fin(proc_sim * sim);
void sim_init_signal_handlers();
int sim_actua(proc_sim * sim, int accion_sim, char * extra);
void sim_init_semaforos(proc_sim * sim);
int parse_accion(char * accion);
void sim_esperar_naves_ready(proc_sim * sim);
void sim_free_resources(proc_sim * sim);
void sim_init_mapa_shm(proc_sim * sim);
void sim_init_shm_readers_count(proc_sim * sim) ;

void sim_down_mapa(proc_sim * sim);
void sim_up_mapa(proc_sim * sim);

#endif 
