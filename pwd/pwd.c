#include "pwd.h"
#include <limits.h>
#include <sys/stat.h>

#define MAX_ARGS_NUMBER 4096
#define MAX_ARGS_STRLEN 4096

/*
Fonctions et autres choses utiles :
    getcwd()
    getenv()
    setenv()
    Variable d'environnement
    getenv() utile pour la commande cd
	Mise à jour de la varibale d'environnement PWD à chaque appele cd
	avec la variable d'environnement PWD
*/

/*
0 -> succes 
1 -> echec
Evite les liens symbolique, donc s'il y'a un lien symbolique recuperer
le chemin logique de lien symbolique
Un chemin ne faisant intervenir aucun lien symblique
*/


char* printError(char* error_msg){
    write(STDERR_FILENO, (const void*) error_msg, strlen(error_msg));
    write(STDERR_FILENO, (const void*) "\n", 1);
}


int pwdForP()
{
	char pwd[MAX_ARGS_STRLEN];
	if (getcwd(pwd, sizeof(pwd)) == NULL)
	{
		perror("error getwcd ");
		return 1;
	}
	else 
	{
		printf("%s\n", pwd);

		return 0;
	}
}

/*
Un chemin faisant intervenir eventuellemnt des liens symboliques
*/
int pwdForL()
{
	char* pwd = getenv("PWD");
	if(pwd==NULL)
	{
		perror("getenv error ");
		return 1;
	}
	else 
	{
		printf("%s\n", pwd);
		return 0;
	}
}



int pwd(int argc, char** argv)
{
	int last_return_value = -1;
    switch (argc){
	case 0:
		//Sans arguments
		last_return_value = pwdForL();
		break;
	case 1:
		//argument '-p'
		if(strcmp((const char*)argv[0], (const char*)"-P") == 0){
        	last_return_value = pwdForP();
        	//return last_return_value;
    	}else if (strcmp((const char*)argv[0], (const char*)"-L") == 0){	//argument '-L'
			last_return_value = pwdForL();
			//return last_return_value;
		}
		else{	//argument invalide
			printError("pwd: wrong argument");
			last_return_value = -1;
		}
		break;
	default:
		//trop d'arguments
		printError("pwd: too many arguments");
		return -1;
	}

    return last_return_value;
}