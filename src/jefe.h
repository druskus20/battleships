#ifndef SRC_JEFE_H_
#define SRC_JEFE_H_

#include "simulador.h"
#include <stdbool.h>

// Lanza un proceso jefe
void jefe_launch(int equipo, int *pipe_sim);

// Funciones generales de proceso
// Crea una estructura para almacenar los datos del proceso
tipo_jefe * jefe_create(int equipo, int *pipe_sim);
// Inicializa las conexiones entre procesos 
void jefe_init(tipo_jefe *jefe);
// Ejecuta el codigo principal del proceso
void jefe_run(tipo_jefe *jefe);
// Finaliza la ejecucion del proceso
void jefe_end(tipo_jefe *jefe);
// Destruye un "tipo_nave"
void jefe_destroy(tipo_jefe *jefe);

// Inicializa las pipes a las naves hijas
void jefe_init_pipes_naves(tipo_jefe * jefe);
// Ejecuta (fork) las naves del equipo
void jefe_run_naves(tipo_jefe *jefe);
// Espera a que finalicen las naves hijas
void jefe_esperar_naves();
// Recibe un mensaje del proceso simulador
char * jefe_recibir_msg_sim(tipo_jefe *jefe);
// Envia un mensaje a un proceso nave
void jefe_mandar_msg_nave(tipo_jefe *jefe, int num_nave, char * msg);
// Bucle de actuacion del jefe, (recibe mensajes y los envia)
int jefe_actua (tipo_jefe * jefe, int accion_jefe, char * extra);

#endif