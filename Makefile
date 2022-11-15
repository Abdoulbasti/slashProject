#slash : slash.o pwd.o
#	gcc -Wall -o slash.o pwd.o 

#slash.o : slash.c 
#	gcc -Wall -c slash.c

#pwd.o : pwd/pwd.c pwd/pwd.h
#	gcc -Wall -c pwd/pwd.c -I pwd/pwd.h

#clean : slash slash.o pwd.o
#	rm -f slash *.o 

slash : slash.c /pwd/pwd.c /pwd/pwd.h
	gcc -Wall -o slash.c /pwd/pwd.c -I pwd/pwd.h