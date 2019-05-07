#ifndef SRC_TYPES_H_
#define SRC_TYPES_H_

#include <semaphore.h>
#include <stdbool.h>
#include <signal.h>
#include <mqueue.h> 

#define BUFF_MAX 1000
#define MSG_MAX 750	// Tiene que ser menor que string max, o los prints de 
					// 	"out_buff[BUFF_MAX] = recibido msng + buff[MSG_MAX] fallarán"

#define TAG_MAX 50      // Tiene que ser bastante menor que BUFF_LEN para que
                        // 	los sprintfs no den error. 
#define MAX_FICHERO_OUT 100
#define STYLE_STRING_L 100


// Semaforos
#define SEM_SIMULADOR "/sem_simulador"
#define SEM_NAVES_READY "/sem_naves_ready"

#define SHM_MAP_NAME "/shm_naves"

// Colas
#define COLA_SIM "/cola_sim"


/*** SIMULACION ***/
#define N_EQUIPOS 1 	  // Número de equipos
#define N_NAVES 1   	  // Número de naves por equipo
#define TURNO_INTERVAL 1
#define MAX_NAVES 4		 // No cambiar
#define MAX_EQUIPOS 4	 // No cambiar

#define MAX_QUEUE_MSGS 10  			 // Maximo numero de mensajes en la cola


#define VIDA_MAX 50       // Vida inicial de una nave
#define ATAQUE_ALCANCE 20 // Distancia máxima de un ataque
#define ATAQUE_DANO 10    // Daño de un ataque
#define MOVER_ALCANCE 1   // Máximo de casillas a mover
#define TURNO_SECS 5      // Segundos que dura un turno

/*** MAPA ***/
#define MAPA_MAXX 20         // Número de columnas del mapa
#define MAPA_MAXY 20         // Número de filas del mapa

/*** SCREEN ***/
#define SCREEN_REFRESH 10000 // !!! TURNO_INTERVAL // Frequencia de refresco del mapa en el monitor
#define SYMB_VACIO '.'       // Símbolo para casilla vacia
#define SYMB_TOCADO '%'      // Símbolo para tocado
#define SYMB_DESTRUIDO 'X'   // Símbolo para destruido
#define SYMB_AGUA 'W'        // Símbolo para agua

// Cadenas de opcion
#define M_FIN 		  		  "FIN"		     // sim y jefe
#define M_MOVER  			  "MOVER"
#define M_ATACAR       	  	  "ATACAR"		 // nave
#define M_DESTRUIR 	 	  	  "DESTRUIR"     // jefe y sim
#define M_TURNO     	  	  "TURNO"  		 // sim y jefe
#define M_ACCION		 	  "ACCION"
// !!! la nave no tiene "DESTRUIR" como pone en el enunciado

// Direcciones
#define COORDENADA "X:%d/Y:%d"
#define NORTE	   "NORTE"
#define SUR		   "SUR"
#define ESTE	   "ESTE"
#define OESTE	   "OESTE"



// Acciones
typedef enum {
	FIN = 0,
	MOVER_ALEATORIO = 1,
	ATACAR = 2,
	DESTRUIR = 3,
	TURNO = 4,
	MOVER = 5
} ACCIONES;





/*** ARGUMENTOS ***/
// Argumentos de ejecucion
typedef struct {
	bool F_color;
	bool F_fichero_out;
	char fichero_out[MAX_FICHERO_OUT];
} tipo_argumentos;
// Se usará con una variable "extern" si es necesario

/*** ESTILO ***/
// Formato para los mensajes en la terminal
typedef struct {
	// messages
	char std_msg[STYLE_STRING_L];	
	char status_msg[STYLE_STRING_L];		
	// status
	char ok_status[STYLE_STRING_L];
	char err_status[STYLE_STRING_L];
	// tags
	char turno_tag[TAG_MAX];
	char nave_tag[TAG_MAX];
	char jefe_tag[TAG_MAX];
	char sim_tag[TAG_MAX];	
	int tag_offset;
} tipo_estiloMSG;

/*** NAVE ***/
// Información de nave
typedef struct {
	int num; 	   // Numero de la nave en el equipo (id)
	int vida; 	   // Vida que le queda a la nave
	int dmg;	   // El daño que inflinge
	int alcance;
	int posx; 	   // Columna en el mapa
	int posy;  	   // Fila en el mapa
	int equipo;    // Equipo de la nave
	
	int * pipe_jefe;
	char tag[TAG_MAX];   
	mqd_t cola_sim;
} tipo_nave;


/*** JEFE ***/
typedef struct {
	int equipo;		// equipo del jefe (id)
	// int pid_naves[N_NAVES]; !!! NO
	// !!! quizas pipes de naves hijas
	int pipes_naves[N_NAVES][2];
	int * pipe_sim;
	char tag[TAG_MAX]; 	
	int pid_naves[N_NAVES];
	int naves_res;	// naves restantes
} tipo_jefe;

/*** SIM ***/
typedef struct {
	// int equipos_rest; // <- se puede? con los waits...
	
	// int pid_jefes[N_EQUIPOS]; !!! NO
	// !!! array de pipes a jefes
	int pipes_jefes[N_EQUIPOS][2];
	char tag[TAG_MAX];
	int equipos_res;		// equipos (jefes) restantes
	mqd_t cola_msg_naves;
	sem_t *sem_naves_ready;
	sem_t *sem_sim;		// semaforo para avisar al monitor
} tipo_sim;

/*** MAPA ***/
// Información de una casilla en el mapa
typedef struct {
	char simbolo; // Símbolo que se mostrará en la pantalla para esta casilla
	int equipo;   // Si está vacia = -1. Si no, número de equipo de la nave que está en la casilla
	int num_nave; // Número de nave en el equipo de la nave que está en la casilla
 } tipo_casilla;

// Información del mapa
typedef struct {
	tipo_nave info_naves[N_EQUIPOS][N_NAVES];
	tipo_casilla casillas[MAPA_MAXY][MAPA_MAXX];
	int num_naves[N_EQUIPOS]; 					  // Número de naves vivas en un equipo
} tipo_mapa;



#endif
