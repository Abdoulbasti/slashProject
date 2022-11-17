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

//insère dans "res" un string contenant les caractères jusqu'au premier "/" de "chemin"
void prochain_dossier(char * res, char * chemin){
	res[0] = '\0';
	int len = strlen(chemin);
	int i;
	for(i = 0; i<len && chemin[i]!='/'; i++){
		res[i] = chemin[i];
	}
	res[i] = '\0';
}

//enlève les caractères jusqu'au premier "/"
void enlever_dossier(char * chemin){
	char tmp[PATH_MAX];
	tmp[0] = '\0';
	int len = strlen(chemin);
	int i;
	for(i = 0; i<len && chemin[i]!='/'; i++){}
	sprintf(tmp, "%s", chemin);
	sprintf(chemin, "%s", tmp+i+1);
}

void cd(int argc, char **argv){
	/*printf("argc: %i\n", argc);
	for(int i = 0; i<argc; i++){
		printf("argv[%i]: %s\n", i, argv[i]);
	}*/
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
		else{
			char tmp[PATH_MAX];
			char * nom_dossier[PATH_MAX];
			for(int i = 0; i < strlen(param); i++){
				sprintf(tmp, "%s", courant_sym);
				char nom_dossier[PATH_MAX];
				prochain_dossier(nom_dossier, param);
				if(strcmp(nom_dossier, "..")==0){
					if(strcmp(tmp, "/")==0){
						perror("chemin incorrect");
					}
					else{
						//enlever_dernier(tmp);
					}
				}
				else{
					//ajouter le nom du répertoire courant à la fin de tmp

					DIR * d = opendir(tmp);
					if(d == NULL){
						perror("répertoire inexistant");
					}
				}
			}
			sprintf(dernier_sym, "%s", courant_sym);
			sprintf(courant_sym, "%s", tmp);
		}

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
	}
	else{
		//afficher les instruction d'utilisation
		//exit(1);
	}

}

int main(int argc, char **argv){
	//cd();
	char chemin[PATH_MAX] = "fiubbukvd/dfdtstbdtebtb/drdtrbtstbbtd/sdtdtstdstb";
	char nom_dossier[PATH_MAX];
	printf("chemin: %s\n", chemin);
	prochain_dossier(nom_dossier, chemin);
	printf("prochain_dossier: %s\n", nom_dossier);
	enlever_dossier(chemin);
	printf("enlever_dossier: %s\n", chemin);
}