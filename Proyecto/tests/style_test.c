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

  printf(SIM_C         " Esto es [SIM_C]\n");
  printf(JEFE_C        " Esto es [JEFE_C] del equipo ""a""\n", "a");
  printf(NAVE_C        " Esto es [NAVE_C] del equipo ""a"" con numero ""2""\n", "a", "2");

  printf(OK_SC      "          Esto es [OK_MSG_C]\n");
  printf(ERROR_SC   "          Esto es [ERROR_MSG_C]\n");



  printf(SIM           " Esto es [SIM]\n");
  printf(JEFE          " Esto es [JEFE] del equipo ""a""\n", "a");
  printf(NAVE          " Esto es [NAVE] del equipo ""a"" con numero ""2""\n", "a", "2");

  printf(OK_S          "          Esto es [OK_MSG]\n");
  printf(ERROR_S       "          Esto es [ERROR_MSG]\n");




  printf("\n[MENSAJES]\n");

  printf(SIM_MC          " Esto es [SIM_MC], OK\n", OK_SC);
  printf(JEFE_MC         " Esto es [JEFE_MC] del equipo ""a"", OK\n", "a", OK_SC);
  printf(NAVE_MC         " Esto es [NAVE_MC] del equipo ""a"" con numero ""2"", OK\n", "a", "2", OK_SC);

  printf(SIM_MC          " Esto es [SIM_MC], ERR\n", ERROR_SC);
  printf(JEFE_MC         " Esto es [JEFE_MC] del equipo ""a"", ERR\n", "a", ERROR_SC);
  printf(NAVE_MC         " Esto es [NAVE_MC] del equipo ""a"" con numero ""2"", ERR\n", "a", "2", ERROR_SC);


  printf(SIM_M           " Esto es [SIM_M] OK\n", OK_S);
  printf(JEFE_M          " Esto es [JEFE_M] del equipo ""a"", OK\n", "a", OK_S);
  printf(NAVE_M          " Esto es [NAVE_M] del equipo ""a"" con numero ""2"", OK\n", "a", "2", OK_S);
  printf(SIM_M           " Esto es [SIM_M] ERR\n", ERROR_S);
  printf(JEFE_M          " Esto es [JEFE_M] del equipo ""a"", ERR\n", "a", ERROR_S);
  printf(NAVE_M          " Esto es [NAVE_M] del equipo ""a"" con numero ""2"", ERR\n", "a", "2", ERROR_S);


  
  printf("\n[EJEMPLO]\n");
  printf(NAVE_MC "Mensaje", "a", "2", OK_SC);

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