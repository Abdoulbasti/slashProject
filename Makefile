slash : slash.c pwd/pwd.c cd/cd.c
	gcc slash.c pwd/pwd.c cd/cd.c -o slash -L/usr/local/lib -I/usr/local/include -I/pwd/ -lreadline

clean: 
	rm -f slash