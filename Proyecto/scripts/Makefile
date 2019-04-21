TARGET = ../bin

CC = gcc
CFLAGS = -g -Wall -pthread -I.
LDLIBS = -lrt -lncurses

BOLD=\e[1m
NC=\e[0m

all: style_test

reset_sems: 
	mkdir -p $(TARGET)
	$(CC) reset_sems.c -o $(TARGET)/reset_sems $(LDLIBS) -lm

clean: 
	rm -r -f $(TARGET)
	