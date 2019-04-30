#ifndef SRC_JEFE_H_
#define SRC_JEFE_H_

#include <simulador.h>
#include <stdbool.h>

tipo_nave * jefe_crear_naves(tipo_jefe *jefe);

tipo_jefe * jefe_init();
void jefe_destroy(tipo_jefe *jefe);

#endif