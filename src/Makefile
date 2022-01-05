TARGET = ../bin

CC = gcc
CFLAGS =  -fcommon -std=gnu11 -g -Wall -pthread -I.

LDLIBS = -lrt -lncurses

BOLD=\e[1m
NC=\e[0m


all: launcher monitor 

clean: 
	@echo "Building style_test"
	@rm -r -f $(TARGET)

launcher:
	@echo "Building launcher"
	@mkdir -p $(TARGET)
	@$(CC) $(CFLAGS) launcher.c simulador.c nave.c jefe.c info_nave.c mapa.c msg.c -o $(TARGET)/launcher $(LDLIBS) -lm
	
monitor:
	@echo "Building monitor"
	@mkdir -p $(TARGET)
	@$(CC) $(CFLAGS) gamescreen.c info_nave.c   mapa.c monitor.c -o $(TARGET)/monitor $(LDLIBS) -lm
		
