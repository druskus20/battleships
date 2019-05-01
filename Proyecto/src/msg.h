#ifndef SRC_MSG_H
#define SRC_MSG_H



#include "types.h"
#include <stdio.h>
// Definiciones de colores para salida en la terminal
// Fuente: https://stackoverflow.com/questions/3219393/stdlib-and-colored-output-in-c
// Uso: ver ../tests/style_test.c




#define RED         "\x1b[0;31m"
#define GREEN       "\x1b[0;32m" 
#define YELLOW      "\x1b[0;33m"
#define BLUE        "\x1b[0;34m"
#define MAGENTA     "\x1b[0;35m"
#define CYAN        "\x1b[0;36m"
#define BOLD_RED    "\x1b[1;31m" 
#define BOLD_GREEN  "\x1b[1;32m" 
#define COLOR_RESET "\x1b[0;0m"


// Espaciados y lineas
#define ESP                     "         "
#define LINE                    "------------"

// Status
#define ERROR_SC   "["BOLD_RED     "ERR"       COLOR_RESET "]"
#define OK_SC      "["BOLD_GREEN   "OK"        COLOR_RESET " ]"
#define ERROR_S   "[ERR]" 
#define OK_S      "[OK ]" 

// Tags
#define SIM_C         MAGENTA   "SIM"               COLOR_RESET "         " 
#define JEFE_C        YELLOW    "JEFE E:%d"         COLOR_RESET "    "   
#define NAVE_C        BLUE      "NAVE E:%d/N:%.1d"  COLOR_RESET  
#define TURNO_C       CYAN      "TURNO"             COLOR_RESET "       "
#define SIM                     "SIM         " 
#define JEFE                    "JEFE E:%d    " 
#define NAVE                    "NAVE E:%d/N:%.1d" 
#define TURNO                   "TURNO       "

// Mensajes
#define STD_MSG      "%s %s > %s\n"
#define STATUS_MSG   "%s " LINE " > %s\n"        




void msg_OK(FILE * fpo, char * msg);
void msg_ERR(FILE * fpo, char * msg);

void load_sim_tag(char tag[TAG_MAX]);
void msg_simOK(FILE * fpo, char * msg);
void msg_simERR(FILE * fpo, char * msg);

void load_jefe_tag(int equipo, char tag[TAG_MAX]);
void msg_jefeOK(FILE * fpo, tipo_jefe * jefe, char * msg);
void msg_jefeERR(FILE * fpo, tipo_jefe * jefe, char * msg);

void load_nave_tag(int equipo, int numero, char tag[TAG_MAX]);
void msg_naveOK(FILE * fpo, tipo_nave * nave, char * msg);
void msg_naveERR(FILE * fpo, tipo_nave * nave, char * msg);

void estilo_set_default();
void estilo_set_colorful();

void msg_signal(FILE * fpo, char * msg) ;
#endif 