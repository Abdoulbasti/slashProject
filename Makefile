<<<<<<< HEAD
slash : slash.c pwd/pwd.c cd/cd.c joker/joker.c
	gcc slash.c pwd/pwd.c cd/cd.c joker/joker.c -o slash -L/usr/local/lib -I/usr/local/include -I/pwd/ -lreadline
=======
slash : slash.c pwd/pwd.c cd/cd.c joker/joker.c commandsExterns/commandesExterns.c
	gcc slash.c pwd/pwd.c cd/cd.c joker/joker.c commandsExterns/commandesExterns.c -o slash -L/usr/local/lib -I/usr/local/include -I/commandsExterns/ -I/pwd/ -lreadline 
#	gcc slash.c pwd/pwd.c cd/cd.c -o slash -L/usr/local/lib -I/usr/local/include -I/pwd/ -lreadline 
>>>>>>> b1ed5fbf0d2e4648eae8f4001be1f0df0d7be5d9

clean: 
	rm -f slash