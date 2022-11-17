#include <string.h>
#include <fcntl.h>
#include <limits.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char dernier_sym[PATH_MAX] = "~";
char courant_sym[PATH_MAX] = "~";
char pwd[PATH_MAX];

/*int inoeud_dossier(char * repertoire){
	//en fonction des arguments, on devra chercher des dossiers où des liens symboliques 
	//TODO: comment suivre un lien symbolique? 
}*/

/*char * prochain_dossier(char * chemin){
	//renvoie le répertoire au début de chemin
	
}*/

void cd(int argc, char **argv){
	printf("argc: %i\n", argc);
	for(int i = 0; i<argc; i++){
		printf("argv[%i]: %s\n", i, argv[i]);
	}
	if(argc == 0){
		DIR * dir = opendir(getenv("HOME"));
		if(dir == NULL){
			perror("erreur ouverture dossier ~");
		}
		sprintf(courant_sym, "%s", "~");
	}
	else if(argc == 1 && strcmp(argv[0],"-")==0){
		//ouvre le dernier répertoire puis échange les deux chemins symboliques
		DIR * courant = opendir(dernier_sym);
		char tmp[PATH_MAX];
		sprintf(tmp, "%s", dernier_sym);
		sprintf(dernier_sym, "%s", courant_sym);
		sprintf(courant_sym, "%s", tmp);
	}
	//on a le droit d'utiliser strcmp?
	else if(argc == 1 || (argc == 2 && strcmp(argv[0],"-L")==0)){
		char param[PATH_MAX];
		if(argc == 1){
			sprintf(param, "%s", argv[0]);
		}
		else{
			sprintf(param, "%s", argv[1]);
		}
		//chemin interprété de manière logique
		//si la référence logique n'a pas de sens, l'interpréter de manière logique
		if(param[0]=='/'){
			DIR * d = opendir(param);
			if(d == NULL){
				perror("le chemin n'existe pas");
			}
			sprintf(dernier_sym, "%s", courant_sym);
			sprintf(courant_sym, "%s", param);
		}
		/*else{
			while(strlen(argv[2])!=0){
				char[PATH_MAX] tmp;
				//malloc?
				nom_dossier = prochain_dossier(argv[2]);
				if(strcmp(nom_dossier, "..")==0){
					if(courant_sym == "/"){
						perror("chemin incorrect");
					}
					else
						//enlever le dernier répertoire de tmp
				}
				else{
					//ajouter le nom du répertoire courant à la fin de tmp
					DIR * d = opendir(tmp);
					if(d == NULL){
						perror("chemin incorrect");
					}
				}
			}
			sprintf(courant, "%s", tmp);
			//décomposer le chemin en répertoires
			//itérer entre les répertoires
			//si le répertoire est "..", enlever le répertoire à la fin de chemin_sym
			//sinon, concaténer le répertoire à la fin de chemin_sym
		}*/

	}
	else if(argc == 2 && strcmp(argv[0],"-P")==0){
		if(argv[1][0]=='/'){
			DIR * d = opendir(argv[2]);
			if(d == NULL){
				perror("le chemin n'existe pas");
			}
			sprintf(dernier_sym, "%s", courant_sym);
			sprintf(courant_sym, "%s", argv[1]);
		}
		/*else{
			//inoeud_dossier pour trouver l'on
		}*/
	}
	else{
		//afficher les instruction d'utilisation
		//exit(1);
	}

}

int main(int argc, char **argv){
	//ouvrir le dossier "~"
	//cd();
	printf("courant_sym: %s\n", courant_sym);
	printf("dernier_sym: %s\n", dernier_sym);
	cd(argc-1,&argv[1]);
	printf("courant_sym: %s\n", courant_sym);
	printf("dernier_sym: %s\n", dernier_sym);
}