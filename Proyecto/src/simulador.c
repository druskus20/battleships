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
#include "mapa.h"

tipo_sim  * sim_global;  // Creada de forma global para usarla en los manejadores de señal
          // semaforo monitor-simulador	

// Manejador de la señal Ctrl+C (SIGINT)
void sim_manejador_SIGINT(int sig) {

    fprintf(fpo, "\n");
    fflush(fpo);
    msg_simOK(fpo,   "Finalizando ejecucion...");

    for (int i = 0; i < N_EQUIPOS; i++) 
        sim_mandar_msg_jefe(sim_global, i, M_FIN);

    sim_esperar_jefes();
    sim_free_resources(sim_global);
    sim_destroy(sim_global);
    
    exit(EXIT_SUCCESS);
}

// Manejador de la señal Ctrl+C (SIGINT)
void sim_manejador_SIGALRM(int sig) {
    char out_buff[BUFF_MAX];

    sprintf(out_buff, "Nuevo %s", estiloMSG.turno_tag);
    msg_simOK(fpo, out_buff);
    
    for (int i = 0; i < N_EQUIPOS; i++)
        sim_mandar_msg_jefe(sim_global, i, M_TURNO);
    alarm(TURNO_SECS);

}


void sim_launch() {
    sim_global = sim_create();
    sim_init(sim_global);
    sim_run(sim_global);
    // Elimina el manejador sigint antes de liberar
    
    sim_end(sim_global);
    sim_destroy(sim_global);
}

tipo_sim * sim_create() {
    tipo_sim * new_sim;
    char out_buff[BUFF_MAX];

    new_sim = (tipo_sim *)malloc(sizeof(new_sim[0]));
    new_sim->equipos_res = 0;
    new_sim->mapa = mapa_create();
    load_sim_tag(new_sim->tag);
    
    sprintf(out_buff, "Creando %s", new_sim->tag);
    msg_simOK(fpo, out_buff);
    return new_sim;
}

void sim_init(tipo_sim * sim) {

    msg_simOK(fpo, "Inicializando");


    sim_init_semaforos(sim);
    sim_init_pipes_jefes(sim);
    sim_init_cola_nave(sim);
    sim_init_signal_handlers();
    sim_init_mapa_shm(sim) ;


    sim_init_shm_readers_count(sim);
}

void sim_init_semaforos(tipo_sim * sim) {
    // Inicializacion de semaforo simulador
    msg_simOK(fpo, "Inicializando semaforos");
    if((sim->sem_sim = sem_open(SEM_SIMULADOR, O_CREAT, S_IRUSR | S_IWUSR, 0)) == SEM_FAILED){
        msg_simERR(fpo, "sem_open de ""sem_sim""");
		exit(EXIT_FAILURE);
	}  

    if((sim->sem_naves_ready = sem_open(SEM_NAVES_READY, O_CREAT, S_IRUSR | S_IWUSR, 0)) == SEM_FAILED){
        msg_simERR(fpo, "sem_open de ""sem_naves_ready""");
		exit(EXIT_FAILURE);
	}  

    if((sim->sem_lecmapa = sem_open(SEM_LECMAPA, O_CREAT, S_IRUSR | S_IWUSR, 0)) == SEM_FAILED){
        msg_simERR(fpo, "sem_open de ""sem_lecmapa""");
		exit(EXIT_FAILURE);
	}  

    if((sim->sem_escmapa = sem_open(SEM_ESCMAPA, O_CREAT, S_IRUSR | S_IWUSR, 0)) == SEM_FAILED){
        msg_simERR(fpo, "sem_open de ""sem_escmapa""");
		exit(EXIT_FAILURE);
	}  

    if((sim->sem_mutex1 = sem_open(MUTEX_LE1, O_CREAT, S_IRUSR | S_IWUSR, 0)) == SEM_FAILED){
        msg_simERR(fpo, "sem_open de ""sem_mutex1""");
		exit(EXIT_FAILURE);
	}  

    if((sim->sem_mutex2 = sem_open(MUTEX_LE2, O_CREAT, S_IRUSR | S_IWUSR, 0)) == SEM_FAILED){
        msg_simERR(fpo, "sem_open de ""sem_mutex3""");
		exit(EXIT_FAILURE);
	}  

}

void sim_run(tipo_sim * sim) {

    bool fin = false;
    char * msg_recibido;

    msg_simOK(fpo, "Avisando al proceso monitor");
    sem_post(sim->sem_sim);   // avisa al monitor
    // Comienzo simulador
    msg_simOK(fpo, "Comenzando");
    sim_run_jefes(sim);
    
    sim_esperar_naves_ready(sim);
  
    alarm(TURNO_SECS);

    while(!fin) {        
        int action_code = -1;
        char extra_buff[BUFF_MAX] = "";
        char main_buff[BUFF_MAX] = "";
        
        msg_recibido = sim_recibir_msg_nave(sim);
        dividir_msg(msg_recibido, main_buff, extra_buff);
        action_code = parse_accion(main_buff);
        fin = sim_actua(sim, action_code, extra_buff);
        free(msg_recibido);
    }   

    signal(SIGALRM, SIG_DFL);  
}


void sim_end(tipo_sim *sim) {
    msg_simOK(fpo, "Finalizando");
    signal(SIGINT, SIG_DFL); 
    
    for (int i = 0; i < N_EQUIPOS; i++) 
        sim_mandar_msg_jefe(sim_global, i, M_FIN);
    
    sim_esperar_jefes();

    sim_free_resources(sim);
}


void sim_destroy(tipo_sim * sim) {
    char out_buff[BUFF_MAX];
    sprintf(out_buff, "Destruyendo %s", sim->tag);
    msg_simOK(fpo, out_buff);    
    

    sem_unlink(SEM_NAVES_READY);
    sem_unlink(SEM_SIMULADOR); // !!! funciona si se cierra antes que monitor?
    sem_unlink(SEM_ESCMAPA); 
    sem_unlink(SEM_LECMAPA); 
    sem_unlink(MUTEX_LE1); 
    sem_unlink(MUTEX_LE2); 
    mq_unlink(COLA_SIM);
    shm_unlink(SHM_MAP_NAME);
    shm_unlink(SHM_READERS_COUNT);
    free(sim);
   
}

void sim_free_resources(tipo_sim * sim) {

    // MAPA? !!!
    mq_close(sim->cola_msg_naves);
    sem_close(sim->sem_naves_ready);
    sem_close(sim->sem_sim);
    sem_close(sim->sem_escmapa);
    sem_close(sim->sem_lecmapa);
    sem_close(sim->sem_mutex1);
    sem_close(sim->sem_mutex2);
    munmap(sim->mapa, sizeof(*sim->mapa));
    munmap(sim->readers_count, sizeof(*sim->readers_count));
    
    
    //mapa_destroy(sim->mapa); // !!!!!!!!!!! No se
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
            signal(SIGINT, SIG_IGN); 
            sem_close(sim->sem_sim);  
            int fd[2];
            fd[0] = sim->pipes_jefes[i][0];
            fd[1] = sim->pipes_jefes[i][1];
            sim_free_resources(sim);
            free(sim);
            jefe_launch(i, fd);
            
            break;  
        }
        else if (pid > 0) { // sim
            sim->equipos_res++;
        }
        else {
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


void sim_mandar_msg_jefe(tipo_sim *sim, int equipo, char msg[MSG_MAX]) {
    char tag[TAG_MAX];
    char out_buff[BUFF_MAX];
    int * fd; // pipe

    load_jefe_tag(equipo, tag);
    sprintf(out_buff, "Mandando mensaje a %s", tag);
    msg_simOK(fpo, out_buff);
    fd = sim->pipes_jefes[equipo];

    // cierra el descriptor de entrada en el jefe

    write(fd[1], msg, MSG_MAX); 
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

char * sim_recibir_msg_nave(tipo_sim * sim) {
    
    //char tag[TAG_MAX];
    char out_buff[BUFF_MAX];
    char * msg_buffer;  // !!! solucciona invalid read creo
    int err = 0;
    

    msg_buffer = (char *)malloc(sizeof(char) * MSG_MAX);
    strcpy(msg_buffer, "");


    msg_simOK(fpo, "Esperando mensaje de nave");
    
    
        errno = 0;
        err = mq_receive(sim->cola_msg_naves, msg_buffer, sizeof(char)*MSG_MAX, NULL);
        printf("ERRNO: %d\n", errno);
    
    if (err == -1) {
        msg_simERR(fpo, "mq_receive");
        exit(EXIT_FAILURE);
    }

    
    sprintf(out_buff, "Recibido mensaje: %s", msg_buffer);
    msg_simOK(fpo, out_buff);

    return msg_buffer;
    
}


bool sim_evaluar_fin(tipo_sim * sim) {
    if (sim->equipos_res <= 1)      // !!! Puede darse el caso de que ningun equipo gane?
        return true;
    return false;
}

void sim_init_signal_handlers() {
    struct sigaction act_sigint, act_sigalrm;

    msg_simOK(fpo, "Inicializando manejadores de señal");

    // Inicializacion del manejador SIGINT
    act_sigint.sa_handler = sim_manejador_SIGINT;
    sigemptyset(&(act_sigint.sa_mask));
    sigaddset(&act_sigint.sa_mask, SIGALRM);
    sigaddset(&act_sigint.sa_mask, SIGPIPE);
    act_sigint.sa_flags = 0;

    if (sigaction(SIGINT, &act_sigint, NULL) < 0) {
        msg_simERR(fpo, "sigaction de SIGINT");
        exit(EXIT_FAILURE);
    }

    // Inicializacion del manejador SIGALRM
    act_sigalrm.sa_handler = sim_manejador_SIGALRM;
    sigemptyset(&(act_sigalrm.sa_mask));
    act_sigalrm.sa_flags = SA_RESTART;
    
    if (sigaction(SIGALRM, &act_sigalrm, NULL) < 0) {
        msg_simERR(fpo, "sigaction de SIGALRM: %s");
        exit(EXIT_FAILURE);
    }  
}

int parse_accion(char * accion) {
    
    if (strcmp(accion, M_FIN) == 0)    
        return FIN;
    else if (strcmp(accion, M_MOVER) == 0) 
        return MOVER;
    else if (strcmp(accion, M_ATACAR) == 0) 
        return ATACAR;
    else if (strcmp(accion, M_DESTRUIR) == 0) 
        return DESTRUIR;
    else if (strcmp(accion, M_TURNO) == 0) 
        return TURNO;
    else if (strcmp(accion, M_ACCION) == 0) 
        return TURNO;
    return -1;
}

int sim_actua(tipo_sim * sim, int accion_sim, char * extra) {    switch (accion_sim){   
        case ATACAR:
        break;

        case MOVER: 
        break;
        
        default:
            return 1;
    }
    return 0;
}


void sim_esperar_naves_ready(tipo_sim * sim) {
    msg_simOK(fpo, "Esperando a naves");
    for (int i = 0; i < N_NAVES * N_EQUIPOS; i++) {
        sem_wait(sim->sem_naves_ready);
    }
}



void sim_init_mapa_shm(tipo_sim * sim) {
    
    msg_simOK(fpo, "Inicializando mapa (shm)");
    int fd_shm = shm_open(SHM_MAP_NAME,
                            O_RDWR | O_CREAT | O_EXCL,
                            S_IRUSR | S_IWUSR); 
    if(fd_shm == -1) {
        msg_simERR(fpo, """shm_open"" de ""sim_init_mapa_shm""");
        exit( EXIT_FAILURE);
    }


    /* Resize the memory segment */
    int error = ftruncate(fd_shm, sizeof(tipo_mapa)); // !???
    if(error == -1) {
        msg_simERR(fpo, """ftruncate"" de ""sim_init_mapa_shm""");
        shm_unlink(SHM_MAP_NAME);
        exit( EXIT_FAILURE);
    }

    sim->mapa = mmap(NULL, sizeof(*sim->mapa),
                            PROT_READ | PROT_WRITE, MAP_SHARED, fd_shm, 0);
    if(sim->mapa == MAP_FAILED)  {
        msg_simERR(fpo, """mmap"" de ""sim_init_mapa_shm""");
        shm_unlink(SHM_MAP_NAME);
        exit( EXIT_FAILURE);
    }

    sim->mapa = mapa_create();
}

void sim_init_shm_readers_count(tipo_sim * sim) {
    
    msg_simOK(fpo, "Inicializando contador de lectores (shm)");
    int fd_shm = shm_open(SHM_READERS_COUNT,
                            O_RDWR | O_CREAT | O_EXCL,
                            S_IRUSR | S_IWUSR); 
    if(fd_shm == -1) {
        msg_simERR(fpo, """shm_open"" de ""sim_init_readerscount_shm""");
        exit( EXIT_FAILURE);
    }


    /* Resize the memory segment */
    int error = ftruncate(fd_shm, sizeof(*sim->readers_count)); // !???
    if(error == -1) {
        msg_simERR(fpo, """ftruncate"" de ""sim_init_readerscount_shm""");
        shm_unlink(SHM_MAP_NAME);
        exit( EXIT_FAILURE);
    }

    sim->readers_count = mmap(NULL, sizeof(*sim->readers_count),
                            PROT_READ | PROT_WRITE, MAP_SHARED, fd_shm, 0);
    if(sim->readers_count == MAP_FAILED)  {
        msg_simERR(fpo, """mmap"" de ""sim_init_readerscount_shm""");
        shm_unlink(SHM_MAP_NAME);
        exit( EXIT_FAILURE);
    }

// !!!!!!!!!!!!!!!!!!!!!!!
    sim->readers_count = 0;

}


void sim_down_mapa(tipo_sim * sim) {
    do {
        sem_wait(sim->sem_lecmapa);
        printf("ERRNO_SEM5: %d", errno);
    } while (errno == EINTR);

    do {
        sem_wait(sim->sem_escmapa);
        printf("ERRNO_SEM6: %d", errno);
    } while (errno == EINTR);
}


void sim_up_mapa(tipo_sim * sim) {
    sem_post(sim->sem_escmapa);
    sem_post(sim->sem_lecmapa);
}