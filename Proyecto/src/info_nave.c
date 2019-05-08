#include "info_nave.h"
#include <stdio.h>
#include <stdlib.h>

#include "mapa.h"

info_nave * info_nave_create(int equipo, int num) {
    info_nave * new_info = (info_nave *)malloc(sizeof(info_nave));
    
    new_info->equipo = equipo;
    new_info->num = num;
    new_info->vida = VIDA_MAX;
    new_info->alcance = ATAQUE_ALCANCE;
    new_info->dmg = ATAQUE_DANO;

    int pos[2];
    if (mapa_generate_pos_nave(equipo, num, pos) == -1) {
        exit(EXIT_FAILURE);
    }

    new_info->posx = pos[0];
    new_info->posy = pos[1];


    return new_info;
}


void info_nave_destroy(info_nave * info) {
    free(info);
}






