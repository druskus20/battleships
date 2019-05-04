#include "msg.h"

#include <stdio.h>
#include <string.h>


tipo_estiloMSG estiloMSG;

void estiloMSG_set_default(){
    strcpy(estiloMSG.std_msg, STD_MSG);
    strcpy(estiloMSG.status_msg, STATUS_MSG);
    strcpy(estiloMSG.ok_status, OK_S);
    strcpy(estiloMSG.err_status, ERROR_S);
    strcpy(estiloMSG.nave_tag, NAVE);
    strcpy(estiloMSG.jefe_tag, JEFE);
    strcpy(estiloMSG.sim_tag, SIM);
    strcpy(estiloMSG.turno_tag, TURNO);
    estiloMSG.tag_offset = NO_TAG_L;
}
void estiloMSG_set_colorful(){
    strcpy(estiloMSG.std_msg, STD_MSG);
    strcpy(estiloMSG.status_msg, STATUS_MSG);
    strcpy(estiloMSG.ok_status, OK_SC);
    strcpy(estiloMSG.err_status, ERROR_SC);
    strcpy(estiloMSG.nave_tag, NAVE_C);
    strcpy(estiloMSG.jefe_tag, JEFE_C);
    strcpy(estiloMSG.sim_tag, SIM_C);   
    strcpy(estiloMSG.turno_tag, TURNO_C);   
    estiloMSG.tag_offset = COLOR_TAG_L;
}


void msg_OK(FILE * fpo, char * msg){
    fprintf(fpo, estiloMSG.status_msg, estiloMSG.ok_status, msg);  
}

void msg_ERR(FILE * fpo, char * msg){
    fprintf(fpo, estiloMSG.status_msg, estiloMSG.err_status, msg);  
}



void load_sim_tag(char tag[TAG_MAX]) {
    strcpy(tag,  estiloMSG.sim_tag); 
}

void msg_simOK(FILE * fpo, char * msg){
    fprintf(fpo, estiloMSG.std_msg, estiloMSG.ok_status, estiloMSG.sim_tag,  msg);
}

void msg_simERR(FILE * fpo,  char * msg){
    fprintf(fpo, estiloMSG.std_msg, estiloMSG.err_status, estiloMSG.sim_tag,  msg);
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
}

void msg_jefeERR(FILE * fpo, tipo_jefe * jefe, char * msg){
    fprintf(fpo, estiloMSG.std_msg, estiloMSG.err_status, jefe->tag, msg);
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
}

void msg_naveERR(FILE * fpo, tipo_nave * nave, char * msg){
    fprintf(fpo, estiloMSG.std_msg, estiloMSG.err_status, nave->tag, msg);
}

