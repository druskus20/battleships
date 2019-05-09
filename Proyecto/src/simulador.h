#ifndef SRC_SIMULADOR_H_
#define SRC_SIMULADOR_H_

#include <stdbool.h>
#include <stdio.h>
#include <semaphore.h>
#include "msg.h"
#include "types.h"


tipo_argumentos args;
FILE * fpo;      
extern tipo_estiloMSG estiloMSG;         // Ver msg.c            
extern char symbol_equipos[MAX_EQUIPOS]; // Ver mapa.c

// Lanza un proceso simulador
void sim_launch();

// Funciones generales de proceso
// Crea una estructura para almacenar los datos del proceso
tipo_sim * sim_create();
// Inicializa las conexiones entre procesos del simulador
void sim_init(tipo_sim * sim);
// Ejecuta el codigo principal del proceso
void sim_run(tipo_sim * sim); 
// Finaliza la ejecucion del proceso
void sim_end(tipo_sim * sim); 
// Destruye el rpoceso simulador
void sim_destroy(tipo_sim * sim); 

// Libera los recursos de tipo cola, memoria compartida etc
void sim_free_resources(tipo_sim * sim);
// Ejecuta el codigo correspondiente a los jefes
void sim_run_jefes(tipo_sim *sim);
// Espera que terminen los jefes para recogerlos y terminar correctamente
void sim_esperar_jefes();
// Inicializa los pipes de cominicacion con los jefes
void sim_init_pipes_jefes(tipo_sim *sim);
// Manda un mensaje al proceso jefe
void sim_mandar_msg_jefe(tipo_sim *sim, int equipo, char * msg);
// Recibe la respuesta de las naves a traves de cola de mensaje
char * sim_recibir_msg_nave(tipo_sim * sim);
// Inicializa la cola de mensajes con la nave
void sim_init_cola_nave(tipo_sim * sim);
// Comprueba si la ejecucion debe finalizar
bool sim_evaluar_fin(tipo_sim * sim);
// Inicializa los manejadores
void sim_init_signal_handlers();
// Bucle de actuacion del simulador, (recibe mensajes y los envia)
int sim_actua(tipo_sim * sim, int accion_sim, char * nave_tag, char * coord_dir);
// Inicializa los semaforos 
void sim_init_semaforos(tipo_sim * sim);
// A partir de una string "accion", devuelve un id (enum ACCIONES)
int parse_accion(char * accion);
// Espera a que las naves estén preparadas
void sim_esperar_naves_ready(tipo_sim * sim);
// Libera todos los recursos ocupados por sumulador
void sim_free_resources(tipo_sim * sim);
// Inicializa el mapa en la memoria compartida
void sim_init_mapa_shm(tipo_sim * sim);


#endif 
