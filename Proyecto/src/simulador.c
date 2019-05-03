#include "simulador.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <math.h>
#include <stdbool.h>
#include <semaphore.h>
#include <errno.h>
#include <unistd.h>

#include "jefe.h"
#include "nave.h"

tipo_sim  * sim_global;  // Creada de forma global para usarla en los manejadores de señal
sem_t *sem_sim;          // semaforo monitor-simulador	

// Manejador de la señal Ctrl+C (SIGINT), con mensajes
void sim_manejador_SIGINT(int sig) {
    fprintf(stdout, "\n");
    // msg_OK(stdout, "SIGINT SIM"); da error por variables globales
    msg_simOK(stdout, "Finalizando ejecucion...");
    sleep(1);
    exit(EXIT_SUCCESS);
}

// Manejador de la señal Ctrl+C (SIGINT)
void sim_manejador_SIGALRM(int sig) {
    char out_buff[BUFF_MAX];
    sprintf(out_buff, "Nuevo %s", estilo.turno_tag);
    msg_simOK(fpo, out_buff);
    sprintf(out_buff, "END %s", estilo.turno_tag);
      
}


void sim_launch() {
    sim_global = sim_create();
    sim_init(sim_global);
    sem_post(sem_sim);   // avisa al monitor
    sim_run(sim_global);
    sim_end(sim_global);
    sim_destroy(sim_global);
}

tipo_sim * sim_create() {
    tipo_sim * new_sim;
    char out_buff[BUFF_MAX];


    new_sim = (tipo_sim *)malloc(sizeof(new_sim[0]));
    load_sim_tag(new_sim->tag);
    
    sprintf(out_buff, "Creando %s", new_sim->tag);
    msg_simOK(fpo, out_buff);
    return new_sim;
}

void sim_init(tipo_sim * sim) {
    struct sigaction act_sigint, act_sigalrm;
    char out_buff[BUFF_MAX];

    msg_simOK(fpo, "Inicializando");
    
    // Inicializacion de semaforo simulador
    if((sem_sim = sem_open(SEM_SIMULADOR, O_CREAT, S_IRUSR | S_IWUSR, 0)) == SEM_FAILED){
        msg_simERR(fpo, "sem_open de ""sem_sim""");
		exit(EXIT_FAILURE);
	}  
    

    // Inicializacion del manejador SIGINT
    act_sigint.sa_handler = sim_manejador_SIGINT;
    sigemptyset(&(act_sigint.sa_mask));
    sigaddset(&act_sigint.sa_mask, SIGALRM);
    act_sigint.sa_flags = 0;
    if (sigaction(SIGINT, &act_sigint, NULL) < 0) {
        msg_simERR(fpo, "sigaction de SIGINT");
        exit(EXIT_FAILURE);
    }

    // Inicializacion del manejador SIGALRM
    act_sigalrm.sa_handler = sim_manejador_SIGALRM;
    sigemptyset(&(act_sigalrm.sa_mask));
    act_sigalrm.sa_flags = 0;
    if (sigaction(SIGALRM, &act_sigalrm, NULL) < 0) {
        sprintf(out_buff, "sigaction de SIGALRM: %s", strerror(errno));
        msg_simERR(fpo, out_buff);
        exit(EXIT_FAILURE);
    }  

    sim_init_pipes_jefes(sim);
}

void sim_run(tipo_sim * sim) {
    // Comienzo simulador
    msg_simOK(fpo, "Comenzando");
    sim_run_jefes(sim);
    sleep(2);
    for (int i = 0; i < N_EQUIPOS; i++)
        sim_mandar_msg_jefe(sim, i);
    for(int i = 0; i < 4; i++){
        alarm(1); 
        pause(); 
        
    }

}

void sim_end(tipo_sim *sim) {
    sim_esperar_jefes();
    
}

void sim_destroy(tipo_sim * sim) {
    char out_buff[BUFF_MAX];
    sprintf(out_buff, "Destruyendo %s", sim->tag);
    msg_simOK(fpo, out_buff);    

    // Elimina el manejador sigint antes de liberar
    signal(SIGINT, SIG_DFL); // CAMBIAR !!!
    free(sim);
    sem_close(sem_sim);
    sem_unlink(SEM_SIMULADOR); // !!! funciona si se cierra antes que monitor?
}

// Inicializa pipes a jefes
void sim_init_pipes_jefes(tipo_sim * sim) { 
    msg_simOK(fpo, "Inicializando pipes a jefes");
    int pipe_status;
    for (int i = 0; i < N_EQUIPOS; i++){
        pipe_status = pipe(sim->pipes_jefes[i]);
        if (pipe_status == -1){
            msg_simERR(fpo, "sim_init_pipes_jefes");
		    exit(EXIT_FAILURE);
        }
    }
}

// Ejecuta los jefes
void sim_run_jefes(tipo_sim *sim) {      
    int pid = -1;
    int i;
    msg_simOK(fpo, "Ejecutando jefes");
    // creacion de jefes
    for (i = 0; i < N_EQUIPOS; i++) {
        pid = fork();
        if (pid == 0) {  // jefe
            signal(SIGALRM, SIG_DFL);
            signal(SIGINT, SIG_DFL);
            //free(sim); //!!!!!!!!!!!
            sem_close(sem_sim);     //!!!!!!!!!!!!
            jefe_launch(i, sim->pipes_jefes[i]);
            break;  
        }
        else if (pid < 0) {
            msg_simERR(fpo, "sim_run_jefes");
            exit(EXIT_FAILURE);
        }   
    }
}

void sim_esperar_jefes() {
    msg_simOK(fpo, "Esperando jefes");
    for (int i = 0; i < N_EQUIPOS; i++)
        wait(NULL);
}


void sim_mandar_msg_jefe(tipo_sim *sim, int equipo) {
    char tag[TAG_MAX];
    char out_buff[BUFF_MAX];
    char msg_buffer[MSG_MAX];
    int * fd; // pipe

    load_jefe_tag(equipo, tag);
    sprintf(out_buff, "Mandando mensaje a %s", tag);
    msg_simOK(fpo, out_buff);
    fd = sim->pipes_jefes[equipo];
    // cierra el descriptor de entrada en el jefe
    close(fd[0]); 
    int len = sprintf(msg_buffer, "HOLA %s", tag);
    write(fd[1], msg_buffer, len); // !!! quiza msg_max+1. pero al leer podría fallar por pasarse de tamaño
}



void sim_init_cola_nave(tipo_sim * sim) {
    struct mq_attr attributes;
     msg_simOK(fpo, "Inicializando cola de mensajes a simulador");

	attributes.mq_flags = O_NONBLOCK;       // !!! OJO Para que los mensajes devuelvan error en vez de bloquearse, capturar
	attributes.mq_maxmsg = MAX_QUEUE_MSGS;
	attributes.mq_curmsgs = 0;
	attributes.mq_msgsize = MSG_MAX;
    
    sim->cola_msg_naves = mq_open(COLA_SIM,
                O_CREAT | O_RDONLY,  // This process is only going to receive messages 
                S_IRUSR | S_IWUSR,  // The user can read and write 
                &attributes); 

	if(sim->cola_msg_naves == (mqd_t)-1) {
        msg_simERR(fpo, "mq_open");
		exit(EXIT_FAILURE);
	}

}

void sim_recibir_msg_nave(tipo_sim * sim) {
    //char tag[TAG_MAX];
    char out_buff[BUFF_MAX];
    char msg_buffer[MSG_MAX] = "";  // !!! solucciona invalid read creo
    
    msg_simOK(fpo, "Esperando mensaje de nave");
    if(mq_receive(sim->cola_msg_naves, (char *)&msg_buffer, sizeof(msg_buffer), NULL) == -1) {
        msg_simERR(fpo, "mq_receive");
        exit(EXIT_FAILURE); 
    }

    sprintf(out_buff, "Recibido mensaje: %s", msg_buffer);
    msg_simOK(fpo, out_buff);
}
