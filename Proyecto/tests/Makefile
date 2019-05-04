TARGET = ../bin

CC = gcc
CFLAGS = -g -Wall -pthread -I.
LDLIBS = -lrt -lncurses

BOLD=\e[1m
NC=\e[0m

all: style_test

style_test:
	@echo "Building style_test"
	@mkdir -p $(TARGET)
	@$(CC) style_test.c ../src/msg.c -o $(TARGET)/style_test $(LDLIBS) -lm

clean: 
	rm -r -f $(TARGET)
	