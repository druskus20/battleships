#include "nave.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include "msg.h"

extern tipo_argumentos args;
extern tipo_estilo estilo;
extern FILE * fpo;

tipo_nave * nave_global;   // Creada de forma global para usarla en los manejadores de señal


void nave_launch(int equipo, int num, int *pipe_jefe) {
    nave_global = nave_create(equipo, num, pipe_jefe);
    nave_init(nave_global);
    nave_run(nave_global);
    nave_end(nave_global);
    nave_destroy(nave_global);
    exit(EXIT_SUCCESS);
}
tipo_nave * nave_create(int equipo, int num, int *pipe_jefe) {
    
    tipo_nave *new_nave;
    char out_buff[BUFF_MAX];

    new_nave = (tipo_nave *)malloc(sizeof(new_nave[0]));
   
    new_nave->equipo = equipo;
    new_nave->num = num;
    new_nave->pipe_jefe = pipe_jefe;
    load_nave_tag(equipo, num, new_nave->tag);
    
    sprintf(out_buff, "Creando %s", new_nave->tag);
    msg_naveOK(fpo, new_nave, out_buff);
    
    return new_nave;
}
void nave_init(tipo_nave * nave){
    msg_naveOK(fpo, nave, "Inicializando");
    nave_init_cola_sim(nave);
}

void nave_run(tipo_nave *nave){
    msg_naveOK(fpo, nave, "Comenzando");
    sleep(1);
    nave_recibir_msg_jefe(nave);
    nave_mandar_msg_sim(nave);  // !!!  que reciba un argumento mas
}

void nave_end(tipo_nave * nave){
    msg_naveOK(fpo, nave, "Finalizando");
}

void nave_destroy(tipo_nave *nave){
    char out_buff[BUFF_MAX];
    sprintf(out_buff, "Destruyendo %s", nave->tag);
    msg_naveOK(fpo, nave, out_buff);
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

bool nave_is_viva(tipo_nave *nave) {
    return nave->viva;
}
void nave_set_viva(tipo_nave *nave, bool viva){
    nave->viva = viva;
}


void nave_recibir_msg_jefe(tipo_nave *nave) {
    char tag[TAG_MAX];
    char out_buff[BUFF_MAX];
    char msg_buffer[MSG_MAX] = ""; // !!! solucciona "error uninitialised value"
    int * fd; // pipe

    load_jefe_tag(nave->equipo, tag);
    sprintf(out_buff, "Esperando mensaje de %s", tag);
    msg_naveOK(fpo, nave, out_buff);
    fd = nave->pipe_jefe;
    // cierra el descriptor de salida en el sim
    close(fd[1]); 
    read(fd[0], msg_buffer, MSG_MAX);
    sprintf(out_buff, "Recibido mensaje: %s", msg_buffer);
    msg_naveOK(fpo, nave, out_buff);
}




/*

	queue1 = mq_open(mq_name,
                O_CREAT | O_WRONLY,  // This process is only going to send messages 
                S_IRUSR | S_IWUSR,  // The user can read and write 
                &attributes); 

	if(queue1 == (mqd_t)-1) {
		fprintf (stderr, "[A] Error opening the queue: %s\n", mq_name); 
		return EXIT_FAILURE;
	}
*/

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


void nave_mandar_msg_sim(tipo_nave * nave) {
        char sim_tag[TAG_MAX];
        char out_buff[BUFF_MAX];
        char msg_buffer[MSG_MAX];

        sprintf(msg_buffer, "MENSAJE DE %s", nave->tag);

        load_sim_tag(sim_tag);

        sprintf(out_buff, "Mandando mensaje a %s", sim_tag);
        msg_naveOK(fpo, nave, out_buff);
        if(mq_send(nave->cola_sim, (char *)&msg_buffer, sizeof(msg_buffer), 1) == -1) {
            msg_naveERR(fpo, nave, "mq_send");
            exit(EXIT_FAILURE); 
        }
}


