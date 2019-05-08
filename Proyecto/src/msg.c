#include "msg.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


tipo_estiloMSG estiloMSG;

void estiloMSG_set_default(){
    strcpy(estiloMSG.std_msg, STD_MSG);
    strcpy(estiloMSG.status_msg, STATUS_MSG);
    strcpy(estiloMSG.ok_status, S_OK);
    strcpy(estiloMSG.err_status, S_ERROR);
    strcpy(estiloMSG.nave_tag, T_NAVE);
    strcpy(estiloMSG.jefe_tag, T_JEFE);
    strcpy(estiloMSG.sim_tag, T_SIM);
    strcpy(estiloMSG.turno_tag, T_TURNO);
    estiloMSG.tag_offset = NO_TAG_L;
}
void estiloMSG_set_colorful(){
    strcpy(estiloMSG.std_msg, STD_MSG);
    strcpy(estiloMSG.status_msg, STATUS_MSG);
    strcpy(estiloMSG.ok_status, S_OK_C);
    strcpy(estiloMSG.err_status, S_ERROR_C);
    strcpy(estiloMSG.nave_tag, T_NAVE_C);
    strcpy(estiloMSG.jefe_tag, T_JEFE_C);
    strcpy(estiloMSG.sim_tag, T_SIM_C);   
    strcpy(estiloMSG.turno_tag, T_TURNO_C);   
    estiloMSG.tag_offset = COLOR_TAG_L;
}   


void msg_OK(FILE * fpo, char * msg){
    fprintf(fpo, estiloMSG.status_msg, estiloMSG.ok_status, msg);  
    fflush(fpo);
}

void msg_ERR(FILE * fpo, char * msg){
    fprintf(fpo, estiloMSG.status_msg, estiloMSG.err_status, msg);  
    fflush(fpo);
}



void load_sim_tag(char tag[TAG_MAX]) {
    strcpy(tag,  estiloMSG.sim_tag); 
}

void msg_simOK(FILE * fpo, char * msg){
    fprintf(fpo, estiloMSG.std_msg, estiloMSG.ok_status, estiloMSG.sim_tag,  msg);
    fflush(fpo);
}

void msg_simERR(FILE * fpo,  char * msg){
    fprintf(fpo, estiloMSG.std_msg, estiloMSG.err_status, estiloMSG.sim_tag,  msg);
    fflush(fpo);
}



void load_jefe_tag(int equipo, char tag[TAG_MAX]) {
    sprintf(tag,  estiloMSG.jefe_tag, equipo); 
}

void extractv_jefe_tag(char tag[TAG_MAX], int * equipo) { 
    *equipo = tag[7 + estiloMSG.tag_offset] - '0';  // JEFE E:%d
    // como es un "char", basta con restar el offset numeracion en ascii ('0')
}

void msg_jefeOK(FILE * fpo, tipo_jefe * jefe, char * msg){
    fprintf(fpo, estiloMSG.std_msg, estiloMSG.ok_status, jefe->tag, msg);
    fflush(fpo);
}

void msg_jefeERR(FILE * fpo, tipo_jefe * jefe, char * msg){
    fprintf(fpo, estiloMSG.std_msg, estiloMSG.err_status, jefe->tag, msg);
    fflush(fpo);
}




void load_nave_tag(int equipo, int numero, char tag[TAG_MAX]) {
    sprintf(tag,  estiloMSG.nave_tag, equipo, numero);     
}

void extractv_nave_tag(char tag[TAG_MAX], int * equipo, int * num_nave) { 
    *equipo = tag[7 + estiloMSG.tag_offset] - '0';  // NAVE E:%d/N:%d
    *num_nave = tag[11 + estiloMSG.tag_offset] - '0';
}

void msg_naveOK(FILE * fpo, tipo_nave * nave, char * msg){
    fprintf(fpo, estiloMSG.std_msg, estiloMSG.ok_status, nave->tag, msg);
    fflush(fpo);
}

void msg_naveERR(FILE * fpo, tipo_nave * nave, char * msg){
    fprintf(fpo, estiloMSG.std_msg, estiloMSG.err_status, nave->tag, msg);
    fflush(fpo);
}


// divide el mensaje "msg" en msg_1 y msg_2 por el token ' '
// msg_1 y msg_2 son buffers vacios, argumentos de salida
void dividir_msg(char * msg, char * msg_1, char * msg_2) { 
    int i = 0;
    char msg_temp[strlen(msg)];  // para evitar erroes en declaraciones implicitas !!!
    strcpy (msg_temp, msg);
    strcpy (msg_1, msg_temp);
    int len  = strlen(msg_temp);
    for (i = 0; i < len; i++) {
        if (msg_temp[i] == ' '){
            msg_temp[i] = '\0';
            strcpy(msg_1, msg_temp);
            strcpy(msg_2, &(msg_temp[i+1])); 
            break;
        }
    }
    
    // si es un mensaje de accion, tiene 3 partes
    if (strcmp(msg_1, "ACCION") == 0) {
        strcpy (msg_temp, msg_2);
        int len  = strlen(msg_temp);
        for (i = 0; i < len; i++) {
            if (msg_temp[i] == ' '){
                msg_temp[i] = '\0';
                strcpy(msg_1, msg_temp);
                strcpy(msg_2, &(msg_temp[i+1])); 
                break;
            }
        }
    }

}

// Extrae el valor de unas coordenadas de la forma X:__/Y:__
void extractv_coordenadas(char * coord, int * x, int * y){
    char cx[3], cy[3];

    cx[0] = coord[2];
    cx[1] = coord[3];
    cx[2] = '\0';
    cy[0] = coord[7];
    cy[1] = coord[8];
    cy[2] = '\0';

    *x = atoi(cx);
    *y = atoi(cy);
    

    

}

