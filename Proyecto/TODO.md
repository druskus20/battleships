# To Do List (Proyecto de Sistemas Operativos)
* Implementar filtro de colores en los logs (o crear script) DONE
* Capturar Ctrl+C tanto en el monitor como en el simulado DONE
* Modificar estilo para que cada entidad tenga una propiedad "tag"
* Modificar estilo para usar "tag" en los mensajes DONE
* Funciones "print" para debuggear, + argumento -d (-debug)
* Hacer el orden de ejecucion monitor-simulador independiente DONE
* Hacer funciones de "print_msg" -> CAMBIADO POR: Cambiar formato de impresion DONE



## Quizas
* types.h
* error enums
* Multiples algoritmos de deteccion de naves, dependiendo del equipo (puntero a funcion en jefe)

## Nave
* !!! Como hacer para que la nave pueda comunicarse con el simulador
* Añadir precision, alcance y daño DONE
* Hacer alguna funcion para "recibir daño (set_daño + mensaje de nave) DONE
* Hacer funcion mover_aleatorio
* Añadir funcion destruir DONE
* Implementar cola de mensajes nave-simulador: 
	* Añadir funcion de evaluar accion (manda el mensaje al simulador)
* Completar nave_init

## Jefe
* Añadir funcion de comunicar accion a nave (pipes)
* Init

## Simulador
* tag
* Init 

## Monitor
* Hacer funcionar el mapa



## Preguntas 
* Como funciona la señal de alarma del turno ?
* Como funcionaba la memoria dinamica con los semáforos??