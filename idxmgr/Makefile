DIR_SRC_IDXMGR = .
DIR_OBJ_IDXMGR = ./obj/idxmgr
DIR_BIN = ./bin
MAIN = ./main.cc

SRC_IDXMGR = $(wildcard ${DIR_SRC_IDXMGR}/*.cc) 
OBJ_IDXMGR = $(patsubst %.cc,${DIR_OBJ_IDXMGR}/%.o,$(notdir ${SRC_IDXMGR}))

TARGET = main
BIN_TARGET = ${DIR_BIN}/${TARGET}

CC = g++
CFLAGS = -g -Wall -nostdlib

${BIN_TARGET}: ${OBJ_IDXMGR} 
	$(CC) $(OBJ_IDXMGR) -o $@ 

${DIR_OBJ_IDXMGR}/%.o:${DIR_SRC_IDXMGR}/%.cc
	$(CC) $(CFLAGS) -c  $< -o $@

.PHONY:clean run
clean:
	rm -rf obj/**/*.o
run:
	make
	./bin/main
