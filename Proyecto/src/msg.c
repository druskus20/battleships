#include "msg.h"

#include <stdio.h>
#include <string.h>


tipo_estilo estilo;

void estilo_set_default(){
    strcpy(estilo.std_msg, STD_MSG);
    strcpy(estilo.status_msg, STATUS_MSG);
    strcpy(estilo.ok_status, OK_S);
    strcpy(estilo.err_status, ERROR_S);
    strcpy(estilo.nave_tag, NAVE);
    strcpy(estilo.jefe_tag, JEFE);
    strcpy(estilo.sim_tag, SIM);
    strcpy(estilo.turno_tag, TURNO);
}
void estilo_set_colorful(){
    strcpy(estilo.std_msg, STD_MSG);
    strcpy(estilo.status_msg, STATUS_MSG);
    strcpy(estilo.ok_status, OK_SC);
    strcpy(estilo.err_status, ERROR_SC);
    strcpy(estilo.nave_tag, NAVE_C);
    strcpy(estilo.jefe_tag, JEFE_C);
    strcpy(estilo.sim_tag, SIM_C);   
    strcpy(estilo.turno_tag, TURNO_C);   
}


void msg_OK(FILE * fpo, char * msg){
    fprintf(fpo, estilo.ok_status, estilo.status_msg, msg);  
}

void msg_ERR(FILE * fpo, char * msg){
    fprintf(fpo, estilo.err_status, estilo.status_msg, msg);  
}



void load_sim_tag(char tag[TAG_MAX]) {
    strcpy(tag,  estilo.sim_tag); 
}

void msg_simOK(FILE * fpo, char * msg){
    fprintf(fpo, estilo.std_msg, estilo.ok_status, estilo.sim_tag,  msg);
}

void msg_simERR(FILE * fpo,  char * msg){
    fprintf(fpo, estilo.std_msg, estilo.err_status, estilo.sim_tag,  msg);
}



void load_jefe_tag(int equipo, char tag[TAG_MAX]) {
    sprintf(tag,  estilo.jefe_tag, equipo); 
}

void msg_jefeOK(FILE * fpo, tipo_jefe * jefe, char * msg){
    fprintf(fpo, estilo.std_msg, estilo.ok_status, jefe->tag, msg);
}

void msg_jefeERR(FILE * fpo, tipo_jefe * jefe, char * msg){
    fprintf(fpo, estilo.std_msg, estilo.err_status, jefe->tag, msg);
}




void load_nave_tag(int equipo, int numero, char tag[TAG_MAX]) {
    sprintf(tag,  estilo.nave_tag, equipo, numero);     
}

void msg_naveOK(FILE * fpo, tipo_nave * nave, char * msg){
    fprintf(fpo, estilo.std_msg, estilo.ok_status, nave->tag, msg);
}

void msg_naveERR(FILE * fpo, tipo_nave * nave, char * msg){
    fprintf(fpo, estilo.std_msg, estilo.err_status, nave->tag, msg);
}


void msg_signal(char * msg) {
    char out_buffer[STRING_MAX];    
    sprintf(out_buffer, STATUS_MSG, OK_S, msg);
    fprintf(stdout, out_buffer);
}