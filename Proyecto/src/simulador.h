#ifndef SRC_SIMULADOR_H_
#define SRC_SIMULADOR_H_

#include <stdbool.h>
#include <stdio.h>
#include <semaphore.h>
#include "style.h"
#include "types.h"

// variables globales 
// !!! por que solo funcionan aqui??
tipo_argumentos args;
tipo_estilo estilo;
FILE * fpo;


void sim_run_jefes(tipo_sim *sim);
void sim_esperar_jefes(tipo_sim *sim);

#endif 
