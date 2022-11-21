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