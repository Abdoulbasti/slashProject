slash : slash.c src/internCommands/pwd/pwd.c
	gcc slash.c src/internCommands/pwd/pwd.c -o slash -L/usr/local/lib -I/usr/local/include -Iinclude/internCommands/pwd -Iinclude -lreadline

clean: 
	rm -f slash
	rm -fr .sy5-2022-2023-projet-slash-autotests.nosyncmukailaa@lulu:~/Projet