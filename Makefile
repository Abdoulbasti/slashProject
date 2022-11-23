slash : slash.c pwd/pwd.c
	gcc slash.c pwd/pwd.c -o slash -L/usr/local/lib -I/usr/local/include -I/pwd/ -lreadline

clean: 
	rm -f slash