#ifndef STYLE_H_
#define STYLE_H_

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

#define SIM_C         MAGENTA "SIM          "       COLOR_RESET "-> "
#define JEFE_C        YELLOW  "JEFE E:%.1s     "    COLOR_RESET "-> "
#define NAVE_C        BLUE    "NAVE E:%.1s/N:%.1s " COLOR_RESET "-> "
#define ERROR_MSG_C   RED     "ERR          "       COLOR_RESET "-> "
#define OK_MSG_C      GREEN   "OK           "       COLOR_RESET "-> "


#define SIM         "SIM          -> "
#define JEFE        "JEFE E:%.1s     -> "
#define NAVE        "NAVE E:%.1s/N:%.1s -> "
#define ERROR_MSG   "ERR          -> "
#define OK_MSG      "OK           -> "

#endif 