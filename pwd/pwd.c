#include "pwd.h"

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


char courant[MAX_PATH] =".";
char chemin_physique[MAX_PATH]="";

int est_racine(){
	struct stat st;
	char tmp[MAX_PATH + 4];
	if(sprintf(tmp, "%s/..", courant) == -1){
		perror("sprintf");
	}

	if(stat(courant, &st) == -1){
		perror("stat");
	}

	struct stat st2;
	if(stat(tmp, &st2) == -1){
		perror("stat");
	}

	if(st.st_ino == st2.st_ino && st.st_dev == st2.st_dev){
		return 1;
	}
	else{
		return 0;
	}

}

int construit_chemin(char* lien_symbolique, int setEnv){
	int n;
	int d;
	strcpy(courant, lien_symbolique);
	while(!est_racine()){
		struct stat st;
		if(stat(courant, &st) == -1){
			return -1;
		}
		n = st.st_ino;
		d = st.st_dev;

		char suivant[MAX_PATH + 4];
		sprintf(suivant, "%s/..", courant);

		DIR * dir_parent = opendir(suivant);

		struct dirent * entry;
		while((entry = readdir(dir_parent))){
			struct stat st2;

			char chemin[MAX_PATH + sizeof(entry->d_name) + 4];
			sprintf(chemin, "%s/%s", suivant, entry->d_name);

			lstat(chemin, &st2);

			if(st2.st_ino == st.st_ino && st2.st_dev == st.st_dev){
				//memmove(chemin_physique+strlen(chemin_physique), strcat(entry->d_name, "/"), strlen(entry->d_name)+1);

				sprintf(courant, "%s", suivant);
				sprintf(suivant, "%s/%s", entry->d_name, chemin_physique);
				sprintf(chemin_physique, "%s", suivant);
				sprintf(suivant, "%s", courant);
				//printf("%s\n", suivant);
			}
		}
	}
	if(setEnv){
		setenv("PWD", chemin_physique, 1);
	}
	return 0;
}

int pwdForP(){
	int return_value = construit_chemin(getenv("PWD"), 0);
	if(return_value == -1){
		printError("pwd -P : Error");
		return -1;
	}
	write(STDIN_FILENO, "/", 1);
	write(STDIN_FILENO, chemin_physique, strlen(chemin_physique) - 1);
	write(STDIN_FILENO, "\n", 1);
	return return_value;
}

/*
Un chemin faisant intervenir eventuellemnt des liens symboliques
*/
int pwdForL()
{
	char* pwd = getenv("PWD");
	if(pwd==NULL)
	{
		printError("getenv error ");
		return 1;
	}
	else 
	{
		write(STDOUT_FILENO, pwd, strlen(pwd));
		write(STDOUT_FILENO, "\n", 1);
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