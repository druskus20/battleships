// Definiciones de colores para salida en la terminal
// Fuente: https://stackoverflow.com/questions/3219393/stdlib-and-colored-output-in-c

#include <stdio.h>
#include "../src/style.h"

int main (int argc, char const *argv[]) {

  printf("\nSTYLE_TEST\n\n");
  printf("[COLORES]\n");
  printf(RED     "Esto es [RED]"     COLOR_RESET "\n");
  printf(GREEN   "Esto es [GREEN]"   COLOR_RESET "\n");
  printf(YELLOW  "Esto es [YELLOW]"  COLOR_RESET "\n");
  printf(BLUE    "Esto es [BLUE]"    COLOR_RESET "\n");
  printf(MAGENTA "Esto es [MAGENTA]" COLOR_RESET "\n");
  printf(CYAN    "Esto es [CYAN]"    COLOR_RESET "\n");

  printf("\n[LEYENDA]\n");
  printf(NAVE        "Esto es [NAVE]\n");
  printf(JEFE        "Esto es [JEFE]\n");
  printf(SIMULADOR   "Esto es [SIMULADOR]\n");
  printf(ERROR_MSG   "Esto es [ERROR_MSG]\n");
  printf(SUCCESS_MSG "Esto es [SUCCESS_MSG]\n");
  
  printf("\n"
         RED     "A" COLOR_RESET 
         GREEN   "R" COLOR_RESET 
         YELLOW  "C" COLOR_RESET 
         BLUE    "O" COLOR_RESET 
         MAGENTA "I" COLOR_RESET 
         CYAN    "R" COLOR_RESET 
         RED     "I" COLOR_RESET 
         GREEN   "S" COLOR_RESET 
         "\n");

  return 0;
}