CC=gcc
INC=
LIB=
FLAGS=-std=c99
SRC=$(wildcard ./src/*.c)
LINK=
OBJS=$(SRC:.c=.o)

all: $(OBJS)

.c.o:
	$(CC) $(FLAGS) $(INC) $(LIB) -c $< -o $@
