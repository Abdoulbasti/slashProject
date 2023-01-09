slash : slash.c pwd/pwd.c cd/cd.c joker/joker.c commandesExternes/commandesExternes.c redirections/redirections.c
	gcc slash.c pwd/pwd.c cd/cd.c joker/joker.c commandesExternes/commandesExternes.c redirections/redirections.c -g -o slash -L/usr/local/lib -I/usr/local/include -I/commandesExternes/ -I/pwd/ -I/redirections/ -lreadline

clean: 
	rm -f slash