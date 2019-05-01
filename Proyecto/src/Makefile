TARGET = ../bin

CC = gcc
CFLAGS = -g -Wall -pthread -I.
LDLIBS = -lrt -lncurses

BOLD=\e[1m
NC=\e[0m


all: simulador monitor 

clean: 
	@echo "Building style_test"
	@rm -r -f $(TARGET)

simulador:
	@echo "Building simulador"
	@mkdir -p $(TARGET)
	@$(CC) $(CFLAGS)  simulador.c nave.c jefe.c mapa.c msg.c -o $(TARGET)/simulador $(LDLIBS) -lm
	
monitor:
	@echo "Building monitor"
	@mkdir -p $(TARGET)
	@$(CC) $(CFLAGS) gamescreen.c  mapa.c monitor.c -o $(TARGET)/monitor $(LDLIBS) -lm
		
