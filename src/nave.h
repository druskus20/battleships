#ifndef SRC_NAVE_H_
#define SRC_NAVE_H_

#include "simulador.h"
#include <stdbool.h>



// Lanza un proceso nave
void nave_launch(int equipo, int num, int *pipe_jefe);

// Funciones generales de proceso
// Crea una estructura para almacenar los datos del proceso
tipo_nave * nave_create(int equipo, int num, int *pipe_jefe);
// Inicializa las conexiones entre procesos de la nave
void nave_init(tipo_nave * nave);
// Ejecuta el codigo principal del proceso
void nave_run(tipo_nave *nave);
// Finaliza la ejecucion del proceso
void nave_end(tipo_nave * nave);
// Destruye un "tipo_nave"
void nave_destroy(tipo_nave *nave);


// Libera los recursos de comunicacion entre procesos de un proceso nave
void nave_free_resources(tipo_nave * nave) ;
// Incicializa los manejadores de señal pertienentes
void nave_init_signal_handlers(tipo_nave * nave);
// Incializa la cola de mensajes al simulador
void nave_init_cola_sim(tipo_nave * nave);
// Envia un mensaje al simulador
void nave_mandar_msg_sim(tipo_nave * nave, char * msg);
// Bucle de actuacion de la nave, (recibe mensajes y los envia)
int nave_actua (tipo_nave * nave, int accion_nave, char * extra);
// Recibe un mensaje del jefe
char * nave_recibir_msg_jefe(tipo_nave *nave);
// Avisa al simulador de que la nave ha sido creada
void nave_ready(tipo_nave * nave);
// Inicializa la memoria compartida para el mapa
void nave_init_shm_mapa(tipo_nave * nave);


int nave_get_equipo(tipo_nave *nave);
void nave_set_equipo(tipo_nave *nave, int equipo);
int nave_get_num(tipo_nave *nave);
void nave_set_num(tipo_nave *nave, int num);
char * nave_get_tag(tipo_nave *nave);
void nave_set_tag(tipo_nave *nave, char *tag);

#endif