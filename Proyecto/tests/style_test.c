// Definiciones de colores para salida en la terminal
// Fuente: https://stackoverflow.com/questions/3219393/stdlib-and-colored-output-in-c

#include <stdio.h>
#include <string.h>
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
  printf(SIM_MC,          OK_SC, "Esto es [SIM_MC], OK");
  printf(JEFE_MC,         "a", OK_SC, "Esto es [JEFE_MC] del equipo ""a"", OK");
  printf(NAVE_MC,         "a", "2", OK_SC, "Esto es [NAVE_MC] del equipo ""a"" con numero ""2"", OK");
  printf(SIM_MC,          ERROR_SC, "Esto es [SIM_MC], ERR");
  printf(JEFE_MC,         "a", ERROR_SC, "Esto es [JEFE_MC] del equipo ""a"", ERR");
  printf(NAVE_MC,         "a", "2", ERROR_SC, "Esto es [NAVE_MC] del equipo ""a"" con numero ""2"", ERR");
  
  printf(SIM_M,            OK_SC, "Esto es [SIM_M], OK");
  printf(JEFE_M,           "a", OK_SC, "Esto es [JEFE_M] del equipo ""a"", OK");
  printf(NAVE_M,           "a", "2", OK_SC, "Esto es [NAVE_M] del equipo ""a"" con numero ""2"", OK");
  printf(SIM_M,            ERROR_SC, "Esto es [SIM_M], ERR");
  printf(JEFE_M,           "a", ERROR_SC, "Esto es [JEFE_M] del equipo ""a"", ERR");
  printf(NAVE_M,           "a", "2", ERROR_SC, "Esto es [NAVE_M] del equipo ""a"" con numero ""2"", ERR");


  
  



  printf("\n[EJEMPLO]\n");
  
  char cadena[200];
  strcpy(cadena, NAVE_MC);
  printf(cadena, "a", "2", "OK", "Mensaje");


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