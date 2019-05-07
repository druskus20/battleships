#ifndef SRC_NAVE_H_
#define SRC_NAVE_H_

#include <simulador.h>
#include <stdbool.h>




void nave_launch(int equipo, int num, int *pipe_jefe);

tipo_nave * nave_create(int equipo, int num, int *pipe_jefe);
void nave_init(tipo_nave * nave);
void nave_run(tipo_nave *nave);
void nave_end(tipo_nave * nave);
void nave_destroy(tipo_nave *nave);

void nave_free_resources(tipo_nave * nave) ;
void nave_init_signal_handlers(tipo_nave * nave);
void nave_init_cola_sim(tipo_nave * nave);
void nave_mandar_msg_sim(tipo_nave * nave, char * msg);
bool nave_evaluar_fin(tipo_nave * nave);
int nave_actua (tipo_nave * nave, int accion_nave, char * extra);
char * nave_recibir_msg_jefe(tipo_nave *nave);
void nave_run_naves(tipo_nave *nave);
void nave_ready(tipo_nave * nave);
void nave_init_semaforos(tipo_nave * nave);
void nave_init_shm_mapa(tipo_nave * nave);
void nave_init_shm_readers_count(tipo_nave * nave);
void nave_actualizar_info(tipo_nave * tipo_nave, info_nave * info_nave);




int nave_get_equipo(tipo_nave *nave);
void nave_set_equipo(tipo_nave *nave, int equipo);

int nave_get_num(tipo_nave *nave);
void nave_set_num(tipo_nave *nave, int num);

char * nave_get_tag(tipo_nave *nave);
void nave_set_tag(tipo_nave *nave, char *tag);

#endif