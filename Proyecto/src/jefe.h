#ifndef SRC_JEFE_H_
#define SRC_JEFE_H_

#include <simulador.h>
#include <stdbool.h>

tipo_nave * jefe_crear_naves(tipo_jefe *jefe);
tipo_jefe * jefe_init(int equipo, int pipe_sim[2]);
void jefe_destroy(tipo_jefe *jefe);
void jefe_run(tipo_jefe *jefe);
void jefe_run_naves(tipo_jefe *jefe);

#endif