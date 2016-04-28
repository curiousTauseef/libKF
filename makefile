CC=gcc
INC=
LIB=
COMP_HDR=./src/kf.h ./src/kfMath.h
FLAGS=-std=c99 -Wall -Werror -fpic -g
SRC=$(wildcard ./src/*.c)
LINK=
OBJS=$(SRC:.c=.o)

OS := $(shell uname)

all: library

install: library
	echo "#define KALMAN_BIG_HEADER" > ./include/kf.h
	cat $(COMP_HDR) >> ./include/kf.h
	cp ./include/kf.h /usr/local/include/
	cp ./lib/libKF.so /usr/local/lib/
ifeq ($(OS),Darwin)
else
	cp ./lib/libKF.so /usr/lib/
endif

library: $(OBJS)
	gcc -shared -o ./lib/libKF.so $(OBJS)

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
	rm ./lib/*.so
	rm ./src/*.o
	rm ./include/kf.h
