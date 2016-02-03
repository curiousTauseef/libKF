OS=$(shell uname)

CXX=gcc
CXX_FLAGS=-std=c99

MAIN=./..

KF_INC = -I./../src
KF_SRC = ./../src/*.c 

LINK = -lm -lc -lkf

TST_SRC=./src/*.c

ifeq ($(OS),Linux)
CXX_FLAGS+= -D_XOPEN_SOURCE=500
endif

all: $(TST_SRC)
	@echo "Building tests..."
	@for source in $(TST_SRC); do\
		($(CXX) $(KF_INC) $(CXX_FLAGS) $(KF_SRC) $$source  -o $${source%.*}.bin) || (exit 1);\
		mv ./src/*.bin ./bin;\
	done
	@python runner.py

clean:
	find . -type f -name '*.bin' -exec rm {} +
	
