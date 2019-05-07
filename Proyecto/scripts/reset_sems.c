#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include "../src/simulador.h"

int main(int argc, char *argv[]) {
    
    sem_unlink(SEM_SIMULADOR);

    return EXIT_SUCCESS;
}