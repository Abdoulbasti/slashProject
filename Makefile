slash : slash.c pwd/pwd.c cd/cd.c joker/joker.c commandesExternes/commandesExternes.c 
	gcc slash.c pwd/pwd.c cd/cd.c joker/joker.c commandesExternes/commandesExternes.c -o slash -L/usr/local/lib -I/usr/local/include -I/commandesExternes/ -I/pwd/ -lreadline

clean: 
	rm -f slash