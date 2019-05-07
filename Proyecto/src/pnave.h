#ifndef SRC_PNAVE_H_
#define SRC_PNAVE_H_

#include "psim.h"
#include <stdbool.h>




void nave_launch(int equipo, int num, int *pipe_jefe);

proc_nave * nave_create(int equipo, int num, int *pipe_jefe);
void nave_init(proc_nave * nave);
void nave_run(proc_nave *nave);
void nave_end(proc_nave * nave);
void nave_destroy(proc_nave *nave);

void nave_free_resources(proc_nave * nave) ;
void nave_init_signal_handlers(proc_nave * nave);
void nave_init_cola_sim(proc_nave * nave);
void nave_mandar_msg_sim(proc_nave * nave, char * msg);
bool nave_evaluar_fin(proc_nave * nave);
int  nave_actua (proc_nave * nave, int accion_nave, char * extra);
char * nave_recibir_msg_jefe(proc_nave *nave);
void nave_run_naves(proc_nave *nave);
void nave_ready(proc_nave * nave);
void nave_init_semaforos(proc_nave * nave);
void nave_init_shm_mapa(proc_nave * nave);
void nave_init_shm_readers_count(proc_nave * nave);
void nave_actualizar_info(proc_nave * proc_nave, info_nave * info_nave);



void nave_down_mapa(proc_nave * nave);
void nave_up_mapa(proc_nave * nave);

int nave_get_equipo(proc_nave *nave);
void nave_set_equipo(proc_nave *nave, int equipo);

int nave_get_num(proc_nave *nave);
void nave_set_num(proc_nave *nave, int num);

char * nave_get_tag(proc_nave *nave);
void nave_set_tag(proc_nave *nave, char *tag);

#endif