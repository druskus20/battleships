# To Do List (Proyecto de Sistemas Operativos)
* Implementar filtro de colores en los logs (o crear script) DONE
* Capturar Ctrl+C tanto en el monitor como en el simulado DONE
* Modificar estilo para que cada entidad tenga una propiedad "tag" DONE
* Modificar estilo para usar "tag" en los mensajes DONE
* Hacer el orden de ejecucion monitor-simulador independiente DONE
* Hacer funciones de "print_msg" -> CAMBIADO POR: Cambiar formato de impresion DONE
* Quitar imprimir semaforo DONE
* msgs.c / .h  sim_print, sim_perror + mover estilo -> msgs.c DONE
	* nave_create_tag.... DONE

* setters y getters
* sem_unlink en los errores? 
* OJO! hacer que simulador, se cierre correctamente, esperando a los procesos cuando se pulsa ctrl + c
* README.md
NO TENER FUNC PRIVADAS
* OJO QUE PASA CUANDO SE CREA LA COLA DEL SIMULADOR ANTES QUE LA DE LA NAVE?

* Setters y getters

## Quizas
* types.h DONE
* error enums 
* Multiples algoritmos de deteccion de naves, dependiendo del equipo (puntero a funcion en jefe)
* Funciones "print" para debuggear, + argumento -d (-debug)

## Nave
* Añadir alcance y daño DONE
* Implementar cola de mensajes nave-simulador: 
	* Añadir funcion de evaluar accion (manda el mensaje al simulador)
* init

## Jefe
* Añadir funcion de comunicar accion a nave (pipes)
* Init

## Simulador
* tag
* Init 
* Acciones de las naves

## Monitor
* Hacer funcionar el mapa

## Preguntas 
* Como funciona la señal de alarma del turno ?
* Como funcionaba la memoria dinamica con los semáforos??
* !!!!!!!!! OJO SOBREESCRIBIR SIGPIPE POR SI JEFE ACABA ANTES Q SIM? creo q nos
!!!!!!!!!!!!!!!!!!!!!!
ERROR LIBERACION DE MEMORIA, MEMORIA RESERVADA SE COPIA!?
!!! Liberar memoria al final?
* PUEDO BORRAR LOS PARAMETROS "SIGACTION" DESPUES DE EJECUTAR "SIGACTION()"?
* PUEDO BORRAR LOS PARAMETROS "attributes" DESPUES DE EJECUTAR "queue_open()"?


## Planning: 
martes 30:
miercoles 1: 
juves 2: 
viernes 3: cola de msg, setters/getters, inits, valgrind

sabado-domingo 4, 5 : memoria compartida (mapa), acciones de las naves

## Entrega:
Jueves 9 de mayo


## Memoria
* Hablar de como se manejan las excepciones -> "FIN" en lugar de manejadores
* !!!!!! HABLAR DE LOS FREES Y SEM_CLOSE ANTES DE FORKS

SECUENCIA DE TURNO: 

*Simulador*
while(true) {
	leer_mensaje_nave() // cola
	reaccionar_mensaje_nave
}	

*Jefe* 
while (true) {
	leer_mensaje_simulador
	reaccionar_mensaje_simulador
}

*Nave* 
while (true) {
	leer_mensaje_jefe
	reaccionar_mensaje_jefe
}


*ALARM*  (simulador) 
Para cada jefe
	mandar_mensaje_jefes (TURNO)

*SIGINT* (simulador)
Para cada jefe
	mandar_mensaje_jefes (END)
esperar_jefes

*SIGINT* (Jefe)
Para cada nave
	mandar_mensaje_nave (END)
esperar_naves


// En sus init correspondientes
void nave_crear_cola_msg() {

}
void jefe_abrir_cola_msg() {
    
}

