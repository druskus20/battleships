TARGET = ../bin

CC = gcc
CFLAGS = -g -Wall -pthread -I.
LDLIBS = -lrt

BOLD=\e[1m
NC=\e[0m

all: reset_sems

reset_sems: 
	mkdir -p $(TARGET)
	$(CC) $(CFLAGS) reset_sems.c -o $(TARGET)/reset_sems $(LDLIBS) -lm

clean: 
	rm -r -f $(TARGET)
	