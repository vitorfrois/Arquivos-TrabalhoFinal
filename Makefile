CC = gcc
INCLUDES = includes
SOURCES = sources/*.c
BINARY = ./programaTrab
PROG = main.c
FLAGS = -g -lm -Wall -Werror
VFLAGS = --show-leak-kinds=all --track-origins=yes --leak-check=full -s

all:
	@$(CC) -o $(BINARY) $(PROG) $(SOURCES) -I$(INCLUDES) $(FLAGS)

run:
	@$(BINARY)

val: all clear 
	valgrind $(VFLAGS) $(BINARY) 

zip:
	zip -r TerceiroTrabalho.zip main.c includes sources Makefile

clean:
	rm $(BINARY) *.bin vgcore.* -f

clear:
	clear

teste: clean all
	cp arquivos/antes/*.bin .
	
