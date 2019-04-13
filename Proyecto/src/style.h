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

#define NAVE        BLUE    "[NAVE]      " COLOR_RESET
#define JEFE        YELLOW  "[JEFE]      " COLOR_RESET
#define SIMULADOR   MAGENTA "[SIMULADOR] " COLOR_RESET
#define ERROR_MSG   RED     "[ERROR]     " COLOR_RESET
#define SUCCESS_MSG GREEN   "[SUCCESS]   " COLOR_RESET 


#endif 