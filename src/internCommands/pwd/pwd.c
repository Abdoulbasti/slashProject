#include "include/headers.h"
#include "include/internCommands/pwd.h"

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



int pwd(char* args)
{
	int last_return_value = 1;
    
	//Commande pwd
    if (strcmp(args, "-P") == 0)
    {
        last_return_value = pwdForP();
        //return last_return_value;
    }
    //Cas par defaut, cas d'un lien logique
    else if (strcmp(args, "-L") == 0 || strcmp(args, "")==0)
    {
        last_return_value = pwdForL();
        //return last_return_value;
    }
    else
    {
        printError("pwd: wrong argument");
        last_return_value = 1;
    }
    return last_return_value;
}