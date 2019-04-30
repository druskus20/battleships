// Definiciones de colores para salida en la terminal
// Fuente: https://stackoverflow.com/questions/3219393/stdlib-and-colored-output-in-c

#include <stdio.h>
#include <string.h>
#include "../src/style.h"

#define CHILD 0
#define FATHER // TODO !!!

int main (int argc, char const *argv[]) {

  printf("\nSTYLE_TEST\n\n");
  
  printf("[COLORES]\n");

  printf(RED     "Esto es [RED]"     COLOR_RESET "\n");
  printf(GREEN   "Esto es [GREEN]"   COLOR_RESET "\n");
  printf(YELLOW  "Esto es [YELLOW]"  COLOR_RESET "\n");
  printf(BLUE    "Esto es [BLUE]"    COLOR_RESET "\n");
  printf(MAGENTA "Esto es [MAGENTA]" COLOR_RESET "\n");
  printf(CYAN    "Esto es [CYAN]"    COLOR_RESET "\n");



  char tag_sim[200];
  char tag_jefe[200];
  char tag_nave[200];
  char tag_sim_c[200];
  char tag_jefe_c[200];
  char tag_nave_c[200];

  sprintf(tag_sim, SIM);
  sprintf(tag_jefe, JEFE, 1);
  sprintf(tag_nave, NAVE, 1, 2);  
  

  sprintf(tag_sim_c, SIM_C);
  sprintf(tag_jefe_c, JEFE_C, 1);
  sprintf(tag_nave_c, NAVE_C, 1, 2);
  
  
  printf("\n[LEYENDA]\n");
  printf("%s %s", tag_sim_c,  "Esto es [SIM_C]\n");
  printf("%s %s", tag_jefe_c, "Esto es [JEFE_C] del equipo ""1""\n");
  printf("%s %s", tag_nave_c, "Esto es [NAVE_C] del equipo ""1"" con numero ""2""\n");
  printf(OK_SC      "          Esto es [OK_MSG_C]\n");
  printf(ERROR_SC   "          Esto es [ERROR_MSG_C]\n");

  printf("%s %s", tag_sim,  "Esto es [SIM]\n");
  printf("%s %s", tag_jefe, "Esto es [JEFE] del equipo ""1""\n");
  printf("%s %s", tag_nave, "Esto es [NAVE] del equipo ""1"" con numero ""2""\n");
  printf(OK_S          "          Esto es [OK_MSG]\n");
  printf(ERROR_S       "          Esto es [ERROR_MSG]\n");


  printf("\n[MENSAJES]\n");
  printf(SIM_MC,          tag_sim_c, OK_SC, "Esto es [SIM_MC], OK");
  printf(JEFE_MC,         tag_jefe_c, OK_SC, "Esto es [JEFE_MC] del equipo ""1"", OK");
  printf(NAVE_MC,         tag_nave_c, OK_SC, "Esto es [NAVE_MC] del equipo ""1"" con numero ""2"", OK");
  printf(SIM_MC,          tag_sim_c, ERROR_SC, "Esto es [SIM_MC], ERR");
  printf(JEFE_MC,         tag_jefe_c, ERROR_SC, "Esto es [JEFE_MC] del equipo ""1"", ERR");
  printf(NAVE_MC,         tag_nave_c, ERROR_SC, "Esto es [NAVE_MC] del equipo ""1"" con numero ""2"", ERR");
  printf(OK_MC,            "Esto es [OK_MC]");
  printf(ERROR_MC,         "Esto es [ERROR_MC]");

  printf("\n");
  
  printf(SIM_M,            tag_sim, OK_S, "Esto es [SIM_M], OK");
  printf(JEFE_M,           tag_jefe, OK_S, "Esto es [JEFE_M] del equipo ""1"", OK");
  printf(NAVE_M,           tag_nave, OK_S, "Esto es [NAVE_M] del equipo ""1"" con numero ""2"", OK");
  printf(SIM_M,            tag_sim, ERROR_S, "Esto es [SIM_M], ERR");
  printf(JEFE_M,           tag_jefe, ERROR_S, "Esto es [JEFE_M] del equipo ""1"", ERR");
  printf(NAVE_M,           tag_nave, ERROR_S, "Esto es [NAVE_M] del equipo ""1"" con numero ""2"", ERR");
  printf(OK_M,             "Esto es [OK_M]");
  printf(ERROR_M,          "Esto es [ERROR_M]");



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

/*

  if (0 == CHILD) printf("A");
  if (1 == FATHER) printf("D"); */
  
  return 0;
}