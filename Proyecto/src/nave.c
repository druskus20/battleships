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
#include <errno.h>
#include <sys/mman.h>
#include <fcntl.h>

#include "mapa.h"
#include "msg.h"



extern FILE * fpo;
tipo_nave * nave_global;



// Manejador de la señal (SIGTERM)
void nave_manejador_SIGTERM(int sig) {    
    //nave_end(nave_global);
    nave_free_resources(nave_global);
    nave_destroy(nave_global);
    
    exit(EXIT_SUCCESS);
} 

void nave_launch(int equipo, int num, int *pipe_jefe) {
    
    nave_global = nave_create(equipo, num, pipe_jefe);
    nave_init(nave_global);
    nave_run(nave_global);

    // Elimina el manejador sigint antes de liberar !!!
    //signal(SIGINT, SIG_DFL); // CAMBIAR !!!
    
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
    nave_init_semaforos(nave);
    nave_init_cola_sim(nave);
    nave_init_signal_handlers(nave);
    nave_init_shm_mapa(nave);
    nave_init_shm_readers_count(nave);
    nave_ready(nave);
}

void nave_run(tipo_nave *nave){
    bool fin = false;
    char * msg_recibido;

    msg_naveOK(fpo, nave, "Comenzando");
    
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
    nave_free_resources(nave);
}
void nave_free_resources(tipo_nave * nave) {
    mq_close(nave->cola_sim);
    sem_close(nave->sem_lecmapa);
    sem_close(nave->sem_escmapa);
    sem_close(nave->sem_mutex1);
    sem_close(nave->sem_mutex3);
    munmap(nave->mapa, sizeof(*nave->mapa));
    munmap(nave->readers_count, sizeof(*nave->readers_count));
}

void nave_destroy(tipo_nave *nave){
    char out_buff[BUFF_MAX];
    sprintf(out_buff, "Destruyendo %s", nave->tag);
    msg_naveOK(fpo, nave, out_buff);

    sem_unlink(MUTEX_LE3);

    free(nave);
}


char * nave_get_tag(tipo_nave *nave) {
    return nave->tag;
}

void nave_set_tag(tipo_nave *nave, char *tag) {
    strcpy(nave->tag, tag);
}




/*
int nave_get_vida(tipo_nave *nave) {
    return nave->vida;
}
void nave_set_vida(tipo_nave *nave, int vida) {
    nave->vida = vida;
}*/


int nave_get_equipo(tipo_nave *nave) {
    return nave->equipo;
}
void nave_set_equipo(tipo_nave *nave, int equipo) {
    nave->equipo = equipo;
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
    
    read(fd[0], msg_buffer, MSG_MAX);
    sprintf(out_buff, "Recibido mensaje: %s", msg_buffer);
    msg_naveOK(fpo, nave, out_buff);
    return msg_buffer;
}





void nave_init_cola_sim(tipo_nave * nave) {
    
    struct mq_attr attributes;
     msg_naveOK(fpo, nave,"Inicializando cola de mensajes a simulador");

	attributes.mq_flags = 0;       // !!! OJO Para que los mensajes devuelvan error en vez de bloquearse, capturar
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


void nave_init_signal_handlers (tipo_nave * nave) {
    struct sigaction act_sigterm;

    msg_naveOK(fpo, nave, "Inicializando manejadores de señal");

    // Inicializacion del manejador SIGINT
    act_sigterm.sa_handler = nave_manejador_SIGTERM;
    sigemptyset(&(act_sigterm.sa_mask));
    sigaddset(&act_sigterm.sa_mask, SIGALRM);
    sigaddset(&act_sigterm.sa_mask, SIGINT);
    sigaddset(&act_sigterm.sa_mask, SIGPIPE);
    act_sigterm.sa_flags = 0;

    if (sigaction(SIGTERM, &act_sigterm, NULL) < 0) {
        msg_naveERR(fpo, nave, "sigaction de SIGTERM");
        exit(EXIT_FAILURE);
    }
}

void nave_mandar_msg_sim(tipo_nave * nave, char * msg) {
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         
        char sim_tag[TAG_MAX];
        char out_buff[BUFF_MAX];
        char msg_buffer[MSG_MAX] = "";   // !!! puede fallar al concatenar la tag dentro        
        
     

        load_sim_tag(sim_tag);
        sprintf(msg_buffer, "%s %s", msg, nave->tag);
        sprintf(out_buff, "Mandando mensaje a %s", sim_tag);
        msg_naveOK(fpo, nave, out_buff);

        
        if(mq_send(nave->cola_sim, (char *)&msg_buffer, MSG_MAX, 1) == -1) {
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
            printf("nave_mapa_cosa %s -\n", nave->mapa->PRUEBA);
            break;

        case DESTRUIR:
        
        default:
            return 1; 
            
    }
    return 0;
}


// Avisa al simulador de que la nave esta preparada
void nave_ready(tipo_nave * nave) {
    sem_t * sem_naves_ready;
    msg_naveOK(fpo, nave, "Nave preparada");
    if((sem_naves_ready = sem_open(SEM_NAVES_READY, O_CREAT, S_IRUSR | S_IWUSR, 0)) == SEM_FAILED){
        msg_naveERR(fpo, nave, "sem_open de ""sem_naves_ready""");
		exit(EXIT_FAILURE);
	}  
    sem_post(sem_naves_ready);

    sem_close(sem_naves_ready);
}

void nave_init_semaforos(tipo_nave * nave) {
    msg_naveOK(fpo, nave, "Inicializando semaforos");
    if((nave->sem_lecmapa = sem_open(SEM_LECMAPA, O_CREAT, S_IRUSR | S_IWUSR, 0)) == SEM_FAILED){
        msg_naveERR(fpo, nave, "sem_open de ""sem_lecmapa""");
		exit(EXIT_FAILURE);
	}  

    if((nave->sem_escmapa = sem_open(SEM_ESCMAPA, O_CREAT, S_IRUSR | S_IWUSR, 0)) == SEM_FAILED){
        msg_naveERR(fpo, nave, "sem_open de ""sem_escmapa""");
		exit(EXIT_FAILURE);
	}  

    if((nave->sem_mutex1 = sem_open(MUTEX_LE1, O_CREAT, S_IRUSR | S_IWUSR, 0)) == SEM_FAILED){
        msg_naveERR(fpo, nave, "sem_open de ""sem_mutex1""");
		exit(EXIT_FAILURE);
	}  

    if((nave->sem_mutex3 = sem_open(MUTEX_LE3, O_CREAT, S_IRUSR | S_IWUSR, 0)) == SEM_FAILED){
        msg_naveERR(fpo, nave, "sem_open de ""sem_mutex3""");
		exit(EXIT_FAILURE);
	}  

}

void nave_init_shm_mapa(tipo_nave * nave)  {
    msg_naveOK(fpo, nave, "Inicializando mapa (shm)");
    int fd_shm = shm_open(SHM_MAP_NAME,
                          O_RDONLY, 0);
    if(fd_shm == -1) {
        msg_naveERR(fpo, nave, "shm_open de ""nave_init_shm_mapa""");
        exit(EXIT_FAILURE);
    }
    
    nave->mapa  = mmap(NULL, sizeof(*nave->mapa),
                        PROT_READ, MAP_SHARED, fd_shm, 0);
    if(nave->mapa == MAP_FAILED){
        msg_naveERR(fpo, nave, "mmap de ""nave_init_shm_mapa""");
         exit(EXIT_FAILURE);
    }
    printf("NAVE_MAPA_COSA: %s\n", nave->mapa->PRUEBA);
    
}
void nave_init_shm_readers_count(tipo_nave * nave)  {
    msg_naveOK(fpo, nave, "Inicializando contador de lectores (shm)");
    int fd_shm = shm_open(SHM_READERS_COUNT,
                            O_RDWR,
                            S_IRUSR | S_IWUSR); 
    if(fd_shm == -1) {
        msg_naveERR(fpo, nave, "shm_open de ""nave_init_shm_readers_count""");
        exit(EXIT_FAILURE);
    }
    
    nave->readers_count  = mmap(NULL, sizeof(*nave->readers_count),
                                PROT_READ | PROT_WRITE, MAP_SHARED, fd_shm, 0);
    if(nave->readers_count == MAP_FAILED){
        msg_naveERR(fpo, nave, "mmap de ""nave_init_shm_readers_count""");
        exit(EXIT_FAILURE);
    }
}





void nave_down_mapa(tipo_nave * nave) {
    
    do {
        sem_wait(nave->sem_mutex3);
        printf("ERRNO_SEM1: %d", errno);
    } while (errno == EINTR);
    do {
        sem_wait(nave->sem_lecmapa);
        printf("ERRNO_SEM2: %d", errno);
    } while (errno == EINTR);
    do {
        sem_wait(nave->sem_mutex1);
        printf("ERRNO_SEM3: %d", errno);
    } while (errno == EINTR);

    nave->readers_count++;
    if (*nave->readers_count == 1){
        do {
            sem_wait(nave->sem_escmapa);
            printf("ERRNO_SEM4: %d", errno);
        } while (errno == EINTR);
    }
    sem_post(nave->sem_mutex1);
    sem_post(nave->sem_lecmapa);
    sem_post(nave->sem_mutex3);

}


void nave_up_mapa(tipo_nave * nave) {

    do {
        sem_wait(nave->sem_mutex1);
        printf("ERRNO_SEM3: %d", errno);
    } while (errno == EINTR);
    nave->readers_count--;
    
    
    if (*nave->readers_count == 0)
        sem_post(nave->sem_escmapa);
    
    sem_post(nave->sem_mutex1);

}