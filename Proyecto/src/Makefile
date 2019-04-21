TARGET = ../bin

CC = gcc
CFLAGS = -g -Wall -pthread -I.
LDLIBS = -lrt -lncurses

BOLD=\e[1m
NC=\e[0m

DEP = mapa.c nave.c 
all: simulador monitor

clean: 
	rm -r -f $(TARGET)

simulador:
	mkdir -p $(TARGET)
	$(CC) $(CFLAGS) mapa.c simulador.c  nave.c -o $(TARGET)/simulador $(LDLIBS) -lm
	
monitor:
	mkdir -p $(TARGET)
	$(CC) $(CFLAGS) gamescreen.c  nave.c  mapa.c monitor.c -o $(TARGET)/monitor $(LDLIBS) -lm
		

nave: 
	mkdir -p $(TARGET)
	$(CC) $(CFLAGS) nave.c simulador.c -o $(TARGET)/monitor $(LDLIBS) -lm