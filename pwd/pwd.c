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

int est_racine(int setChemin, char * courant){
	struct stat st;
	char tmp[PATH_MAX];
	if(sprintf(tmp, "%s/..", courant) == -1){
		perror("sprintf");
		return -1;
	}

	if(stat(courant, &st) == -1){
		if(setChemin == 0){
			perror("pwd");
		}
		return -1;
	}

	struct stat st2;
	if(stat(tmp, &st2) == -1){
		if(setChemin == 0){
			perror("pwd");
		}
		return -1;
	}

	if(st.st_ino == st2.st_ino && st.st_dev == st2.st_dev){
		return 1;
	}
	else{
		return 0;
	}

}


//fonction similarie à "realpath", construisant le chemin physique à partir d'un chemin symbolique passé en paramètre
char * construit_chemin(char* chemin_symbolique, int setChemin){
	int n;
	int d;

	char courant[PATH_MAX];
	char* chemin_physique = malloc(PATH_MAX);
	char current_file[PATH_MAX];

	//initialisation de "chemin_physique" au string vide
	chemin_physique[0] = '\0';

	//initialisation de "courant" au chemin passé en paramètre
	strcpy(courant, chemin_symbolique);

	//valeur retour de "est_racinesetChemin, courant)"
	int return_value_est_racine;
	while(!(return_value_est_racine = est_racine(setChemin, courant))){
		//on récupère l'ino, le périphérique et le nom du fichier courant
		struct stat st;

		//erreur lors de l'ouverture du dossier courant
		if(stat(courant, &st) == -1){
			perror(NULL);
			free(chemin_physique);
			return NULL;
		}

		//current_file est le chemin vers le dossier parent de courant
		sprintf(current_file, "%s/..", courant);

		//ouverture du dossier parent du dossier sourant
		DIR * dir_parent = opendir(current_file);

		//parcours du dossier parent pour trouver le nom du dossier actuel
		struct dirent * entry;
		int i = 0;
		while((entry = readdir(dir_parent))){
			i++;
			struct stat st2;

			char chemin[PATH_MAX + sizeof(entry->d_name)];
			sprintf(chemin, "%s/%s", current_file, entry->d_name);

			lstat(chemin, &st2);

			//on retrouve le dossier courant ("st2") dans le dossier parent. On utilise st2 pour obtenir le nom du dossier courant
			if(st2.st_ino == st.st_ino && st2.st_dev == st.st_dev){
				//memmove(chemin_physique+strlen(chemin_physique), strcat(entry->d_name, "/"), strlen(entry->d_name)+1);

				//on remonte l'arborescence (ajout de "../" à la fin de courant)
				sprintf(courant, "%s", current_file);


				//ajout du nom du dossier courant au début du chemmin physique
				//on utilise current_file comme string temporaire
				sprintf(current_file, "/%s%s", entry->d_name, chemin_physique);
				sprintf(chemin_physique, "%s", current_file);
				sprintf(current_file, "%s", courant);
			}
		}
		//libération de la mémoire allouée pour le pointeur "dir_parent"
		free(dir_parent);

		if(i == 0){
			free(chemin_physique);
			return NULL;
		}
	}
	if(return_value_est_racine == -1){
		free(chemin_physique);
		return NULL;
	}
	if(setChemin != 0){
		strcpy(chemin_symbolique, chemin_physique);
	}
	return chemin_physique;
}

int pwdForP(){
	char* chemin_physique = construit_chemin(getenv("PWD"), 0);
	if(chemin_physique == NULL){
		printError("pwd -P : Error");
		return -1;
	}

	write(STDIN_FILENO, chemin_physique, strlen(chemin_physique));
	write(STDIN_FILENO, "\n", 1);
	free(chemin_physique);
	return 0;
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