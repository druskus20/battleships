// Definiciones de colores para salida en la terminal
// Fuente: https://stackoverflow.com/questions/3219393/stdlib-and-colored-output-in-c

#include <stdio.h>
#include <string.h>
#include "../src/msg.h"

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
  
  
  printf("\n[TAGS]\n");
  printf("%s %s", tag_sim_c,  "Esto es [SIM_C]\n");
  printf("%s %s", tag_jefe_c, "Esto es [JEFE_C] del equipo ""1""\n");
  printf("%s %s", tag_nave_c, "Esto es [NAVE_C] del equipo ""1"" con numero ""2""\n");
  printf(OK_SC      "          Esto es [OK_SC]\n");
  printf(ERROR_SC   "          Esto es [ERROR_SC]\n");

  printf("%s %s", tag_sim,     "Esto es [SIM]\n");
  printf("%s %s", tag_jefe,    "Esto es [JEFE] del equipo ""1""\n");
  printf("%s %s", tag_nave,    "Esto es [NAVE] del equipo ""1"" con numero ""2""\n");
  printf(OK_S          "        Esto es [OK_S]\n");
  printf(ERROR_S       "        Esto es [ERROR_S]\n");


  printf("\n[MENSAJES]\n");
  printf(STD_MSG,         tag_sim_c, OK_SC,     "Esto es [STD_MSG], del simulador, con status [OK_SC]");
  printf(STD_MSG,         tag_jefe_c, OK_SC,    "Esto es [STD_MSG], del jefe, del equipo ""1"", con status [OK_SC]");
  printf(STD_MSG,         tag_nave_c, OK_SC,    "Esto es [STD_MSG], de la nave, del equipo ""1"" con numero ""2"", con status [OK_SC]");
  printf(STD_MSG,         tag_sim_c, ERROR_SC,  "Esto es [STD_MSG], del simulador, con status [ERR_SC]");
  printf(STD_MSG,         tag_jefe_c, ERROR_SC, "Esto es [STD_MSG], del jefe, del equipo ""1"", con status [ERR_SC]");
  printf(STD_MSG,         tag_nave_c, ERROR_SC, "Esto es [STD_MSG], de la nave, del equipo ""1"" con numero ""2"", con status [ERR_SC]");
  printf(STATUS_MSG,        OK_SC,              "Esto es [STATUS_MSG], con status [OK_SC]");
  printf(STATUS_MSG,        ERROR_SC,           "Esto es [STATUS_MSG], con status [ERR_SC]");

  printf("\n");
  
  printf(STD_MSG,            tag_sim, OK_S,     "Esto es [SIM_M], del simulador, con status [OK_S]");
  printf(STD_MSG,           tag_jefe, OK_S,     "Esto es [JEFE_M], del jefe, del equipo ""1"", con status [OK_S]");
  printf(STD_MSG,           tag_nave, OK_S,     "Esto es [NAVE_M], de la nave, del equipo ""1"" con numero ""2"", con status [OK_S]");
  printf(STD_MSG,            tag_sim, ERROR_S,  "Esto es [SIM_M], del simulador, con status [ERR_S]");
  printf(STD_MSG,           tag_jefe, ERROR_S,  "Esto es [JEFE_M], del jefe, del equipo ""1"", con status [ERR_S]");
  printf(STD_MSG,           tag_nave, ERROR_S,  "Esto es [NAVE_M], de la nave, del equipo ""1"" con numero ""2"", con status [ERR_S]");
  printf(STATUS_MSG,          OK_S,             "Esto es [STATUS_MSG], con status [OK_S]");
  printf(STATUS_MSG,          ERROR_S,          "Esto es [STATUS_MSG], con status [ERR_S]");



  printf("\n"
         RED     "A" COLOR_RESET 
         GREEN   "R" COLOR_RESET 
         YELLOW  "C" COLOR_RESET 
         BLUE    "O" COLOR_RESET 
         MAGENTA "I" COLOR_RESET 
         CYAN    "R" COLOR_RESET 
         RED     "I" COLOR_RESET 
         GREEN   "S" COLOR_RESET 
                 "\n"           );

  
  return 0;
}