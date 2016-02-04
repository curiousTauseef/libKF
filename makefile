CC=gcc
INC=
LIB=
COMP_HDR=./src/kf.h ./src/kfMath.h
FLAGS=-std=c99 -Wall -Werror -fpic
SRC=$(wildcard ./src/*.c)
LINK=
OBJS=$(SRC:.c=.o)

all: library

install: library
	cp ./libKF.so /usr/local/lib/
	echo "#define KALMAN_BIG_HEADER" > kf.h
	cat $(COMP_HDR) >> kf.h
	mv ./kf.h /usr/local/include/

library: $(OBJS)
	gcc -shared -o libKF.so $(OBJS)

.PHONY: header
header:
	echo "#ifndef H_KALMAN_FILTER" > kf.h
	echo "#define H_KALMAN_FILTER" >> kf.h

	cat ./src/kfData.h ./src/kfMath.h >> kf.h
	cat $(SRC) >> kf.h

	echo "#endif" >> kf.h

.c.o:
	$(CC) $(FLAGS) $(INC) $(LIB) -c $< -o $@

clean:
	rm ./src/*.o
	rm ./kf.h
