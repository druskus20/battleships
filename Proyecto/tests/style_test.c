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

  sprintf(tag_sim, T_SIM);
  sprintf(tag_jefe, T_JEFE, 1);
  sprintf(tag_nave, T_NAVE, 1, 2);  
  sprintf(tag_sim_c, T_SIM_C);
  sprintf(tag_jefe_c, T_JEFE_C, 1);
  sprintf(tag_nave_c, T_NAVE_C, 1, 2);
  
  
  printf("\n[TAGS]\n");
  printf("%s %s", tag_sim_c,  "Esto es [T_SIM_C]\n");
  printf("%s %s", tag_jefe_c, "Esto es [T_JEFE_C] del equipo ""1""\n");
  printf("%s %s", tag_nave_c, "Esto es [T_NAVE_C] del equipo ""1"" con numero ""2""\n");
  printf(S_OK_C      "          Esto es [S_OK_C]\n");
  printf(S_ERROR_C   "          Esto es [S_ERROR_C]\n");

  printf("%s %s", tag_sim,     "Esto es [T_SIM]\n");
  printf("%s %s", tag_jefe,    "Esto es [T_JEFE] del equipo ""1""\n");
  printf("%s %s", tag_nave,    "Esto es [T_NAVE] del equipo ""1"" con numero ""2""\n");
  printf(S_OK          "        Esto es [S_OK]\n");
  printf(S_ERROR       "        Esto es [S_ERROR]\n");


  printf("\n[MENSAJES]\n");
  printf(STD_MSG,         tag_sim_c, S_OK_C,     "Esto es [STD_MSG], del simulador, con status [S_OK_C]");
  printf(STD_MSG,         tag_jefe_c, S_OK_C,    "Esto es [STD_MSG], del jefe, del equipo ""1"", con status [S_OK_C]");
  printf(STD_MSG,         tag_nave_c, S_OK_C,    "Esto es [STD_MSG], de la nave, del equipo ""1"" con numero ""2"", con status [S_OK_C]");
  printf(STD_MSG,         tag_sim_c, S_ERROR_C,  "Esto es [STD_MSG], del simulador, con status [S_ERROR_C]");
  printf(STD_MSG,         tag_jefe_c, S_ERROR_C, "Esto es [STD_MSG], del jefe, del equipo ""1"", con status [S_ERROR_C]");
  printf(STD_MSG,         tag_nave_c, S_ERROR_C, "Esto es [STD_MSG], de la nave, del equipo ""1"" con numero ""2"", con status [S_ERROR_C]");
  printf(STATUS_MSG,      S_OK_C,                "Esto es [STATUS_MSG], con status [OK_SC]");
  printf(STATUS_MSG,      S_ERROR_C,             "Esto es [STATUS_MSG], con status [ERR_SC]");

  printf("\n");
  
  printf(STD_MSG,           tag_sim, S_OK,       "Esto es [SIM_M], del simulador, con status [S_OK]");
  printf(STD_MSG,           tag_jefe, S_OK,      "Esto es [JEFE_M], del jefe, del equipo ""1"", con status [S_OK]");
  printf(STD_MSG,           tag_nave, S_OK,      "Esto es [NAVE_M], de la nave, del equipo ""1"" con numero ""2"", con status [S_OK]");
  printf(STD_MSG,           tag_sim, S_ERROR,    "Esto es [SIM_M], del simulador, con status [S_ERROR]");
  printf(STD_MSG,           tag_jefe, S_ERROR,   "Esto es [JEFE_M], del jefe, del equipo ""1"", con status [S_ERROR]");
  printf(STD_MSG,           tag_nave, S_ERROR,   "Esto es [NAVE_M], de la nave, del equipo ""1"" con numero ""2"", con status [S_ERROR]");
  printf(STATUS_MSG,        S_OK,                "Esto es [STATUS_MSG], con status [S_OK]");
  printf(STATUS_MSG,        S_ERROR,             "Esto es [STATUS_MSG], con status [S_ERROR]");



  printf("\n"
         RED     "A" COLOR_RESET 
         GREEN   "R" COLOR_RESET 
         YELLOW  "C" COLOR_RESET 
         BLUE    "O" COLOR_RESET 
         MAGENTA "I" COLOR_RESET 
         CYAN    "R" COLOR_RESET 
         RED     "I" COLOR_RESET 
         GREEN   "S" COLOR_RESET 
                 "\n\n"           );

  int equipo;
  int num_nave;
  

  extractv_jefe_tag("JEFE E:1", &equipo);
  printf("JEFE E:1, equipo: %d\n", equipo);
  extractv_nave_tag("NAVE E:1/N:2", &equipo, &num_nave);
  printf("NAVE E:1/N:2, equipo: %d, num_nave: %d\n", equipo, num_nave);

  return 0;
}