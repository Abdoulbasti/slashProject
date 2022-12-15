slash : slash.c pwd/pwd.c cd/cd.c commandsExterns/commandesExterns.c
	gcc slash.c pwd/pwd.c cd/cd.c commandsExterns/commandesExterns.c -o slash -L/usr/local/lib -I/usr/local/include -I/commandsExterns/ -I/pwd/ -lreadline 
#	gcc slash.c pwd/pwd.c cd/cd.c -o slash -L/usr/local/lib -I/usr/local/include -I/pwd/ -lreadline 

clean: 
	rm -f slash