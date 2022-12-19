slash : slash.c pwd/pwd.c cd/cd.c joker/joker.c commandsExterns/commandesExterns.c
	gcc slash.c pwd/pwd.c cd/cd.c commandsExterns/commandesExterns.c joker/joker.c -o slash -L/usr/local/lib -I/usr/local/include -I/commandsExterns/ -I/pwd/ -lreadline 

clean: 
	rm -f slash