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


* !!! Necesito un manejador de sigint especifico para naves? o vale con el de jefes?

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


## Planning: 
martes 30: simulador / jefes, pipes
miercoles 1: jefes / naves, pipes 
juves 2: naves / simulador, cola de msg

sabado-domingo 4, 5 : memoria compartida (mapa), acciones de las naves

### Entrega:
Jueves 9 de mayo