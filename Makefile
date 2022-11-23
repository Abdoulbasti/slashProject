#slash : slash.c
#	gcc slash.c  -o slash -L/usr/local/lib -I/usr/local/include -Iinclude -lreadline

#clean: 
#	rm -f slash
#	rm -fr .sy5-2022-2023-projet-slash-autotests.nosyncmukailaa@lulu:~/Projet$ 


#CC=gcc
#INCLUDE=./include/
#BIN=./bin/
#SRC=./src/
#SEFLAGS=-I$(INCLUDE) -L/usr/local/lib -I/usr/local/include -o

#init: clean bin compileserver

#bin:
#	mkdir $(BIN)

#clean:
#	rm -rf $(BIN)*
#	rm -rf $(BIN)

#compileserver:
#	$(CC) $(SEFLAGS) $(BIN)slash slash.c