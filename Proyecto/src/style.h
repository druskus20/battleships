#ifndef SRC_STYLE_H_
#define SRC_STYLE_H_

#include "types.h"
// Definiciones de colores para salida en la terminal
// Fuente: https://stackoverflow.com/questions/3219393/stdlib-and-colored-output-in-c
// Uso: ver ../tests/style_test.c

#define RED         "\x1b[31m"
#define GREEN       "\x1b[32m" 
#define YELLOW      "\x1b[33m"
#define BLUE        "\x1b[34m"
#define MAGENTA     "\x1b[35m"
#define CYAN        "\x1b[36m"
#define COLOR_RESET "\x1b[0m"



// Status
#define ERROR_SC   RED     "ERR"       COLOR_RESET 
#define OK_SC      GREEN   "OK "       COLOR_RESET
#define ERROR_S   "ERR" 
#define OK_S      "OK " 

// Nombres
#define ESP                     "         "
#define LINE                    "------------ "
#define SIM_C         MAGENTA   "SIM         "       COLOR_RESET 
#define JEFE_C        YELLOW    "JEFE E:%d    "      COLOR_RESET 
#define NAVE_C        BLUE      "NAVE E:%d/N:%.1d"   COLOR_RESET  
#define SIM                     "SIM         " 
#define JEFE                    "JEFE E:%d    " 
#define NAVE                    "NAVE E:%d/N:%.1d" 

// Mensajes
#define SIM_MC                     "%s %s -> %s\n"
#define JEFE_MC                    "%s %s -> %s\n"
#define NAVE_MC                    "%s %s -> %s\n"
#define OK_MC          LINE   OK_SC    " -> %s\n"
#define ERROR_MC       LINE   ERROR_SC " -> %s\n"

#define SIM_M                      "%s %s -> %s\n"
#define JEFE_M                     "%s %s -> %s\n"
#define NAVE_M                     "%s %s -> %s\n" 
#define OK_M           LINE   OK_S     " -> %s\n"
#define ERROR_M        LINE   ERROR_S  " -> %s\n"



#endif 