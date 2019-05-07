#ifndef SRC_PJEFE_H_
#define SRC_PJEFE_H_

#include <psim.h>
#include <stdbool.h>





proc_nave * jefe_crear_naves(proc_jefe *jefe);
proc_jefe * jefe_create(int equipo, int *pipe_sim);

void jefe_launch(int equipo, int *pipe_sim);
void jefe_init(proc_jefe *jefe);
void jefe_run(proc_jefe *jefe);
void jefe_end(proc_jefe *jefe);
void jefe_destroy(proc_jefe *jefe);

void jefe_init_pipes_naves(proc_jefe * jefe);
void jefe_run_naves(proc_jefe *jefe);
void jefe_esperar_naves();
char * jefe_recibir_msg_sim(proc_jefe *jefe);
void jefe_mandar_msg_nave(proc_jefe *jefe, int num_nave);
bool jefe_evaluar_fin(proc_jefe * jefe);
int jefe_actua (proc_jefe * jefe, int accion_jefe, char * extra);

#endif