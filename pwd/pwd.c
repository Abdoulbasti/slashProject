#include "pwd.h"

/*
Fonctions et autres choses utiles :
    getcwd()
    getenv()
    setenv()
    Variable d'environnement
    getenv() utile pour la commande cd
	avec la variable d'environnement PWD
*/

int est_racine(){
	struct stat st;
	char tmp[PATH_MAX];
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

int construit_chemin(){
	while(!est_racine()){
		struct stat st;
		if(stat(courant, &st) == -1){
			//exit(1);
			perror("stat1 error ");
			return 1;
		}

		char tmp[PATH_MAX];
		sprintf(tmp, "%s/..", courant);

		DIR * dir_parent = opendir(tmp);

		struct dirent * entry;
		while((entry = readdir(dir_parent))){
			struct stat st2;

			char chemin[PATH_MAX];
			sprintf(chemin, "%s/%s", tmp, entry->d_name);

			if(stat(chemin, &st2) == -1)
			{
				perror("stat2 error ");
				return 1;
			}

			if(st2.st_ino == st.st_ino && st2.st_dev == st.st_dev){
				//memmove(pwd+strlen(pwd), strcat(entry->d_name, "/"), strlen(entry->d_name)+1);

				sprintf(courant, "%s", tmp);
				sprintf(tmp, "%s/%s", entry->d_name, pwd);
				sprintf(pwd, "%s", tmp);
				sprintf(tmp, "%s", courant);
			}
		}
		printf("s", pwd);
		return 0;
	}
}


/*
0 -> succes 
1 -> echec
*/
int pwdForP()
{
	if (getcwd(pwd, sizeof(pwd)) == NULL)
	{
		perror("error getwcd ");
		return 1;
	}
	else 
	{
		printf("%s\n", pwd);
		//Mise à jour de la variable d'env PWD par pwd
		int env = setenv( "PWD" , pwd, 1);
		if (env==-1)
		{
			perror("setenv error ");
			return 1;
		}
		return 0;
	}
}

/*
default cas
*/
int pwdForL()
{
	return construit_chemin();
}