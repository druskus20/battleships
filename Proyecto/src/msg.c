#include "msg.h"

#include <stdio.h>
#include <string.h>




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
    fprintf(fpo, estilo.std_msg,  estilo.ok_status, msg);
}

void msg_simERR(FILE * fpo,  char * msg){
    fprintf(fpo, estilo.std_msg,  estilo.err_status, msg);
}



void load_jefe_tag(int equipo, char tag[TAG_MAX]) {
    sprintf(tag,  estilo.jefe_tag, equipo); 
}

void msg_jefeOK(FILE * fpo, tipo_jefe * jefe, char * msg){
    fprintf(fpo, estilo.std_msg, jefe->tag, estilo.ok_status, msg);
}

void msg_jefeERR(FILE * fpo, tipo_jefe * jefe, char * msg){
    fprintf(fpo, estilo.std_msg, jefe->tag, estilo.err_status, msg);
}




void load_nave_tag(int equipo, int numero, char tag[TAG_MAX]) {
    sprintf(tag,  estilo.nave_tag, equipo, numero);     
}

void msg_naveOK(FILE * fpo, tipo_nave * nave, char * msg){
    fprintf(fpo, estilo.std_msg, nave->tag, estilo.ok_status, msg);
}

void msg_naveERR(FILE * fpo, tipo_nave * nave, char * msg){
    fprintf(fpo, estilo.std_msg, nave->tag, estilo.err_status, msg);
}
