#include "nave.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <math.h>

#include "mapa.h"
#include "msg.h"



extern FILE * fpo;



/*
// Manejador de la señal Ctrl+C (SIGINT)
void nave_manejador_SIGINT(int sig) {
    // msg_OK(stdout, "SIGINT SIM"); da error por variables globales
    msg_naveOK(stdout, nave_global, "Finalizando ejecucion...");
    // nave_end(nave_global);
    nave_destroy(nave_global);
    fflush(fpo);
    exit(EXIT_SUCCESS);
} */

void nave_launch(int equipo, int num, int *pipe_jefe) {
    tipo_nave * nave;   
    nave = nave_create(equipo, num, pipe_jefe);
    nave_init(nave);
    nave_run(nave);

    // Elimina el manejador sigint antes de liberar !!!
    //signal(SIGINT, SIG_DFL); // CAMBIAR !!!
    
    nave_end(nave);
    nave_destroy(nave);
    exit(EXIT_SUCCESS);
}
tipo_nave * nave_create(int equipo, int num, int *pipe_jefe) {
    
    tipo_nave *new_nave;
    char out_buff[BUFF_MAX];

    new_nave = (tipo_nave *)malloc(sizeof(new_nave[0]));
   
    new_nave->equipo = equipo;
    new_nave->num = num;
    new_nave->vida = VIDA_MAX;
    new_nave->alcance = ATAQUE_ALCANCE;

    int pos[2];
    if (mapa_generate_pos_nave(equipo, num, pos) == -1) {
        msg_naveERR(fpo, new_nave, "generate_pos_nave");
        exit(EXIT_FAILURE);
    }

    new_nave->posx = pos[0];
    new_nave->posy = pos[1];

    new_nave->pipe_jefe = pipe_jefe;
    load_nave_tag(equipo, num, new_nave->tag);
    
    sprintf(out_buff, "Creando %s con posicion: X:%d/Y:%d", new_nave->tag, new_nave->posx, new_nave->posy);
    msg_naveOK(fpo, new_nave, out_buff);
    
    return new_nave;
}
void nave_init(tipo_nave * nave){
    msg_naveOK(fpo, nave, "Inicializando");
    nave_init_cola_sim(nave);
}

void nave_run(tipo_nave *nave){
    bool fin = false;
    char * msg_recibido;

    msg_naveOK(fpo, nave, "Comenzando");
    sleep(1);
    while (!fin) {
        int action_code = -1;
        char extra_buff[BUFF_MAX] = "";
        char main_buff[BUFF_MAX] = "";
        msg_recibido = nave_recibir_msg_jefe(nave);
        dividir_msg(msg_recibido, main_buff, extra_buff);
        action_code = parse_accion(main_buff);
        fin = nave_actua(nave, action_code, extra_buff);
        free(msg_recibido);

    }
}

void nave_end(tipo_nave * nave){
    msg_naveOK(fpo, nave, "Finalizando");
}

void nave_destroy(tipo_nave *nave){
    char out_buff[BUFF_MAX];
    sprintf(out_buff, "Destruyendo %s", nave->tag);
    msg_naveOK(fpo, nave, out_buff);
    mq_close(nave->cola_sim);
    free(nave);
}


char * nave_get_tag(tipo_nave *nave) {
    return nave->tag;
}

void nave_set_tag(tipo_nave *nave, char *tag) {
    strcpy(nave->tag, tag);
}

int nave_get_dmg(tipo_nave *nave) {
    return nave->dmg;
}


void nave_set_dmg(tipo_nave *nave, int dmg) {
    nave->dmg = dmg;
}


int nave_get_alcance(tipo_nave *nave) {
    return nave->alcance;
}


void nave_set_alcance(tipo_nave *nave, int alcance) {
    nave->alcance = alcance;
}


int nave_get_vida(tipo_nave *nave) {
    return nave->vida;
}
void nave_set_vida(tipo_nave *nave, int vida) {
    nave->vida = vida;
}

int nave_get_posx(tipo_nave *nave) {
    return nave->posx;
}
void nave_set_posx(tipo_nave *nave, int posx) {
    nave->posx = posx;
}

int nave_get_posy(tipo_nave *nave) {
    return nave->posy;
}
void nave_set_posy(tipo_nave *nave, int posy) {
    nave->posy = posy;
}

int nave_get_equipo(tipo_nave *nave) {
    return nave->equipo;
}
void nave_set_equipo(tipo_nave *nave, int equipo) {
    nave->equipo = equipo;
}

int nave_get_num(tipo_nave *nave) {
    return nave->num;
}
void nave_set_num(tipo_nave *nave, int num) {
    nave->num = num;
}



char * nave_recibir_msg_jefe(tipo_nave *nave) {
    char tag[TAG_MAX];
    char out_buff[BUFF_MAX];
    char * msg_buffer;
    int * fd; // pipe

    msg_buffer = (char *)malloc(sizeof(char) * MSG_MAX);
    strcpy(msg_buffer, "");

    load_jefe_tag(nave->equipo, tag);
    sprintf(out_buff, "Esperando mensaje de %s", tag);
    msg_naveOK(fpo, nave, out_buff);
    fd = nave->pipe_jefe;
    // cierra el descriptor de salida en el sim
    close(fd[1]); 
    read(fd[0], msg_buffer, MSG_MAX);
    sprintf(out_buff, "Recibido mensaje: %s", msg_buffer);
    msg_naveOK(fpo, nave, out_buff);
    return msg_buffer;
}





void nave_init_cola_sim(tipo_nave * nave) {
    
    struct mq_attr attributes;
     msg_naveOK(fpo, nave,"Inicializando cola de mensajes a simulador");

	attributes.mq_flags = O_NONBLOCK;       // !!! OJO Para que los mensajes devuelvan error en vez de bloquearse, capturar
	attributes.mq_maxmsg = MAX_QUEUE_MSGS;
	attributes.mq_curmsgs = 0;
	attributes.mq_msgsize = MSG_MAX;
    
    nave->cola_sim = mq_open("/cola_sim",
                O_CREAT | O_WRONLY,  // This process is only going to send messages 
                S_IRUSR | S_IWUSR,  // The user can read and write 
                &attributes); 

	if(nave->cola_sim == (mqd_t)-1) {
        msg_naveERR(fpo, nave, "mq_open");
		exit(EXIT_FAILURE);
	}

    

}


void nave_mandar_msg_sim(tipo_nave * nave, char * msg) {
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         
        char sim_tag[TAG_MAX];
        char out_buff[BUFF_MAX];
        char msg_buffer[MSG_MAX] = "";

        sprintf(msg_buffer, "MENSAJE DE %s", nave->tag);

        load_sim_tag(sim_tag);

        sprintf(out_buff, "Mandando mensaje a %s", sim_tag);
        msg_naveOK(fpo, nave, out_buff);
        if(mq_send(nave->cola_sim, (char *)&msg_buffer, sizeof(msg_buffer), 1) == -1) {
            msg_naveERR(fpo, nave, "mq_send");
            exit(EXIT_FAILURE); 
        }

    
}




// retorna bool "fin"
int nave_actua (tipo_nave * nave, int action_code, char * extra) {
    switch (action_code){
        case MOVER_ALEATORIO:
            nave_mandar_msg_sim(nave, "MOVER");  // !!!  que reciba un argumento mas
            break;

        case ATACAR: 
            nave_mandar_msg_sim(nave, "ATACAR");  // !!!  que reciba un argumento mas
            break;

        default:
            return 1; 
            
    }
    return 0;
}


