#ifndef SRC_JEFE_H_
#define SRC_JEFE_H_

#include <simulador.h>
#include <stdbool.h>





tipo_nave * jefe_crear_naves(tipo_jefe *jefe);
tipo_jefe * jefe_create(int equipo, int pipe_sim[2]);

void jefe_launch(int equipo, int pipe_sim[2]);
void jefe_init(tipo_jefe *jefe);
void jefe_run(tipo_jefe *jefe);
void jefe_end(tipo_jefe *jefe);
void jefe_destroy(tipo_jefe *jefe);

void jefe_run_naves(tipo_jefe *jefe);
void jefe_esperar_naves(tipo_jefe *jefe);

#endif