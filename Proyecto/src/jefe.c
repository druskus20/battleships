#include "jefe.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>

#include "nave.h"
#include "msg.h" 


extern FILE * fpo;


void jefe_launch(int equipo, int *pipe_sim) {     
        tipo_jefe * jefe;
        srand(getpid());
        jefe = jefe_create(equipo, pipe_sim);
        jefe_init(jefe);
        jefe_run(jefe);

        jefe_end(jefe); 

        jefe_destroy(jefe);
        exit(EXIT_SUCCESS);
}


tipo_jefe * jefe_create(int equipo, int *pipe_sim) {
    
    tipo_jefe *new_jefe;

    new_jefe = (tipo_jefe *)malloc(sizeof(new_jefe[0]));

    new_jefe->equipo = equipo;
    new_jefe->naves_res = N_NAVES;    
    new_jefe->pipe_sim = pipe_sim;
    
    for (int i = 0; i < N_NAVES; i++){ 
        new_jefe->pid_naves[i] = -1;
        new_jefe->naves_vivas[i] = true;
    }
   

    load_jefe_tag(equipo, new_jefe->tag);


    char out_buff[BUFF_MAX];
    sprintf(out_buff, "Creando %s", new_jefe->tag);
    msg_jefeOK(fpo, new_jefe, out_buff);
    
    return new_jefe;
}

void jefe_init(tipo_jefe *jefe) {
    msg_jefeOK(fpo, jefe, "Inicializando");
    jefe_init_pipes_naves(jefe);
}

void jefe_run(tipo_jefe *jefe){


    bool fin = false;
    char * msg_recibido;
    
    msg_jefeOK(fpo, jefe, "Comenzando");
    jefe_run_naves(jefe); 

    while(!fin  /*&& jefe->naves_res > 0*/) {
        int action_code = -1;
        char extra_buff[BUFF_MAX] = "";
        char main_buff[BUFF_MAX] = "";
        msg_recibido = jefe_recibir_msg_sim(jefe);
        


        dividir_msg(msg_recibido, main_buff, extra_buff);
        action_code = parse_accion(main_buff);
        fin = jefe_actua(jefe, action_code, extra_buff);
        free(msg_recibido);
    }   
} 

void jefe_end(tipo_jefe *jefe) {
    msg_jefeOK(fpo, jefe, "Finalizando");
    jefe_esperar_naves(jefe);
    
    
}
void jefe_destroy(tipo_jefe *jefe){
    char out_buff[BUFF_MAX];
    sprintf(out_buff, "Destruyendo %s", jefe->tag);
    msg_jefeOK(fpo, jefe, out_buff);
    free(jefe);
}

void jefe_run_naves(tipo_jefe *jefe){
    int pid = -1;
    int equipo = jefe->equipo;
    msg_jefeOK(fpo, jefe, "Ejecutando naves");
    // creacion de naves
    for (int i = 0; i < N_NAVES; i++) {
        pid = fork();
        if (pid == 0) { 
            
            signal(SIGINT, SIG_IGN); 
            
            int fd[2];
            fd[0] = jefe->pipes_naves[i][0];
            fd[1] = jefe->pipes_naves[i][1];
            free(jefe);
            nave_launch(equipo, i, fd);
            break;
        }
        else if (pid > 0) { // jefe
            jefe->pid_naves[i] = pid;
        }
        else  {
            msg_jefeERR(fpo, jefe, "jefe_run_naves");
            exit(EXIT_FAILURE);
        }
    }
}

void jefe_esperar_naves(tipo_jefe *jefe) {
    msg_jefeOK(fpo, jefe, "Esperando naves");
    for (int i = 0; i < N_NAVES; i++)
        wait(NULL);
}

void jefe_init_pipes_naves(tipo_jefe * jefe) { 
    msg_jefeOK(fpo, jefe,"Inicializando pipes a naves");
    int pipe_status;
    for (int i = 0; i < N_NAVES; i++){
        pipe_status = pipe(jefe->pipes_naves[i]);
        if (pipe_status == -1){
            msg_jefeERR(fpo, jefe, "jefe_init_pipes_naves");
		    exit(EXIT_FAILURE);
        }
    }
}


char * jefe_recibir_msg_sim(tipo_jefe *jefe) {
    char tag[TAG_MAX];
    char out_buff[BUFF_MAX];
    char * msg_buffer;
    int * fd; // pipe

    msg_buffer = (char *)malloc(sizeof(char) * MSG_MAX);
    strcpy(msg_buffer, "");

    load_sim_tag(tag);
    sprintf(out_buff, "Esperando mensaje de %s", tag);
    msg_jefeOK(fpo, jefe, out_buff);
    fd = jefe->pipe_sim;

    // cierra el descriptor de salida en el sim
    close(fd[1]); 
    read(fd[0], msg_buffer, MSG_MAX);
    sprintf(out_buff, "Recibido mensaje: %s", msg_buffer);
    msg_jefeOK(fpo, jefe, out_buff);
    return msg_buffer;

}

void jefe_mandar_msg_nave(tipo_jefe *jefe, int num_nave, char * msg) {
    char tag[TAG_MAX];
    char out_buff[BUFF_MAX];

    int * fd; 
    load_nave_tag(jefe->equipo, num_nave, tag); 
    if (jefe->naves_vivas[num_nave] == false) {
        sprintf(out_buff, "La nave %s ha sido destruida, no se envia mensaje", tag);
        msg_jefeOK(fpo, jefe, out_buff);
    }

   
    sprintf(out_buff, "Mandando mensaje a %s", tag);
    msg_jefeOK(fpo, jefe, out_buff);
    fd = jefe->pipes_naves[num_nave];

    // cierra el descriptor de entrada en el jefe
    close(fd[0]); 
    write(fd[1], msg, MSG_MAX); 
}




int jefe_actua (tipo_jefe * jefe, int accion_jefe, char * extra) {
    int num_nave, equipo;

    switch (accion_jefe){   
        case DESTRUIR:
            extractv_nave_tag(extra, &equipo, &num_nave);
            jefe_mandar_msg_nave(jefe, num_nave, M_DESTRUIR);
            jefe->naves_res--;
            jefe->naves_vivas[num_nave] = false;
            if (jefe->naves_res <= 0)  {
                return 1;
            }
            break;

        case TURNO: 
                for (int i = 0; i < N_NAVES; i++) {
                    for (int j = 0; j < N_ACCIONES_TURNO; j++) {
                        if ((rand() % 2) != 1 && jefe->naves_vivas[i] == true) 
                            jefe_mandar_msg_nave(jefe, i, M_MOVER_ALEATORIO);
                        else 
                            jefe_mandar_msg_nave(jefe, i, M_ATACAR);
                    }
                }
            break;
        
        case FIN:
        default:
            for (int i = 0; i < N_NAVES; i++) {
                if (jefe->naves_vivas[i] == true) 
                    kill(jefe->pid_naves[i],SIGTERM);
            }
            
            return 1;
    }
    return 0;
}