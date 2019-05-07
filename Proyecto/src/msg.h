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
#define COLOR_RESET "\x1b[0;00m" // !!!

#define NO_TAG_L 0
#define COLOR_TAG_L 10  // longitud de la tag de color

// Espaciados y lineas           
#define ESP                     "         "
#define LINE                    "------------"

// Status
#define S_ERROR_C   "["BOLD_RED     "ERR"       COLOR_RESET "]"
#define S_OK_C      "["BOLD_GREEN   "OK"        COLOR_RESET " ]"
#define S_ERROR     "[ERR]" 
#define S_OK        "[OK ]" 

// Tags
#define T_SIM_C         MAGENTA   "SIM"               COLOR_RESET "         " 
#define T_JEFE_C        YELLOW    "JEFE E:%d"         COLOR_RESET "    "   
#define T_NAVE_C        BLUE      "NAVE E:%d/N:%d"    COLOR_RESET  
#define T_TURNO_C       CYAN      "TURNO"             COLOR_RESET "       "
#define T_SIM                     "SIM         " 
#define T_JEFE                    "JEFE E:%d    " 
#define T_NAVE                    "NAVE E:%d/N:%d" 
#define T_TURNO                   "TURNO       "

// Mensajes
#define STD_MSG      "%s %s > %s\n"
#define STATUS_MSG   "%s " LINE " > %s\n"        




void msg_OK(FILE * fpo, char * msg);
void msg_ERR(FILE * fpo, char * msg);

// tag es parametro de salida
void load_sim_tag(char tag[TAG_MAX]);
void msg_simOK(FILE * fpo, char * msg);
void msg_simERR(FILE * fpo, char * msg);

// tag es parametro de salida
void load_jefe_tag(int equipo, char tag[TAG_MAX]);
// equipo es parametro de salida
void extractv_jefe_tag(char tag[TAG_MAX], int * equipo);
void msg_jefeOK(FILE * fpo, tipo_jefe * jefe, char * msg);
void msg_jefeERR(FILE * fpo, tipo_jefe * jefe, char * msg);

// tag es parametro de salida
void load_nave_tag(int equipo, int numero, char tag[TAG_MAX]);
// equipo y numero_nave son parametros de salida
void extractv_nave_tag(char tag[TAG_MAX], int * equipo, int * num_nave);
void msg_naveOK(FILE * fpo, tipo_nave * nave, char * msg);
void msg_naveERR(FILE * fpo, tipo_nave * nave, char * msg);

void estiloMSG_set_default();
void estiloMSG_set_colorful();
void dividir_msg(char * msg,  char * msg_1, char * msg_2);


#endif 