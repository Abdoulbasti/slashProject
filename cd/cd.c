#include <string.h>
#include <fcntl.h>
#include <limits.h>
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "cd.h"
#include "../pwd/pwd.h"

char dernier_sym[PATH_MAX] = "\0";

//insère dans "res" un string contenant les caractères jusqu'au premier "/" inclus de "chemin"
void prochain_dossier(char * nom_dossier, char * chemin){
	nom_dossier[0] = '\0';
	int len = strlen(chemin);
	int i;
	for(i = 0; i<len && chemin[i]!='/'; i++){
		nom_dossier[i] = chemin[i];
	}
	nom_dossier[i] = '\0';
}

//enlève les caractères jusqu'au premier "/" inclus
void enlever_premier_dossier(char * chemin){
	char tmp[PATH_MAX];
	tmp[0] = '\0';
	int len = strlen(chemin);
	int i;
	for(i = 0; i<len && chemin[i]!='/'; i++){}
	sprintf(tmp, "%s", chemin);
	if(i == len){
		chemin[0] = '\0';
	}
	else{
		sprintf(chemin, "%s", tmp+i+1);
	}
}

//enlève les charactères à partir du dernier "/"
void enlever_dernier_dossier(char * chemin){
	int len = strlen(chemin);
	int i;
	for(i = len; i>0 && chemin[i]!='/'; i--){}
	chemin[i]='\0';
}

int cd(int argc, char **argv){
	if(strcmp(dernier_sym, "\0") == 0){
		sprintf(dernier_sym, "%s", getenv("PWD"));
	}
	if(argc == 0){
		int fd = open(getenv("HOME"), O_RDONLY|O_DIRECTORY, 0666);
		if(fd < 0){
			perror(NULL);
			return 1;
		}
		close(fd);
		setenv("PWD", getenv("HOME"), 1);
	}
	else if(argc == 1 && strcmp(argv[0],"-")==0){
		//ouvre le dernier répertoire puis échange les deux chemins symboliques
		int fd = open(dernier_sym, O_RDONLY|O_DIRECTORY, 0666);
		if(fd < 0){
			perror(NULL);
			return 1;
		}
		close(fd);
		char tmp[PATH_MAX];
		sprintf(tmp, "%s", dernier_sym);
		sprintf(dernier_sym, "%s", getenv("PWD"));
		setenv("PWD", tmp, 1);
	}
	//chemin interprété de manière logique
	/*
	else if(argc == 1 || (argc == 2 && strcmp(argv[0],"-L")==0)){
		char param[PATH_MAX];
		if(argc == 1){
			sprintf(param, "%s", argv[0]);
		}
		else{
			sprintf(param, "%s", argv[1]);
		}
		//stocke le nouveau chemin symbolique. Il sera écrit dans la variable "pwd" si il est valide
		char tmp[PATH_MAX];
		tmp[0] = '\0';

		//ouverture du dossier à partir duquel le parcours sera effectué
		int fd;
		//si c'est une référence relative, le parcours commence par le répertoire de travail
		if(param[0] == '/'){
			fd = open("/", O_RDONLY|O_DIRECTORY, 0666);
			if(fd < 0){
				perror(NULL);
				return 1;
			}
			//on enlève le premier "/" de param
			enlever_premier_dossier(param);
		}
		else{
			fd = open(getenv("PWD"), O_RDONLY|O_DIRECTORY, 0666);
			if(fd < 0){
				perror(NULL);
				return 1;
			}
			//comme le parcours commence par le répertoire de travail courant, c'est la valeur de $PWD qui est copiée dans tmp
			sprintf(tmp, "%s", getenv("PWD"));
		}
		
		//nom du premier dossier dans le chemin à parcourir
		char nom_dossier[PATH_MAX];

		//boucle de parcours
		while(param[0]!='\0'){
			prochain_dossier(nom_dossier, param);

			int fd_sous = openat(fd, nom_dossier, O_RDONLY|O_DIRECTORY,  0666);
			if(fd_sous < 0){
				char * nargv[] = {"-P", param};
				return cd(2, nargv);
			}
			else{
				close(fd);
				fd = fd_sous;
			}

			if(strcmp(nom_dossier, "..") == 0){
				enlever_dernier_dossier(tmp);
			}
			else if(strcmp(nom_dossier, ".") != 0){
				strcat(tmp, "/");
				strcat(tmp, nom_dossier);
			}
			enlever_premier_dossier(param);
		}
		close(fd);
		sprintf(dernier_sym, "%s", getenv("PWD"));
		setenv("PWD", tmp, 1);
	}
	//chemin interprété de manière physique
	else if(argc == 2 && strcmp(argv[0],"-P")==0){
		char param[PATH_MAX];
		sprintf(param, "%s", argv[1]);

		int fd;
		//ouverture du dossier à partir duquel le parcours sera effectué
		//si c'est une référence relative, le parcours commence par le répertoire de travail
		if(param[0] == '/'){
			fd = open("/", O_RDONLY|O_DIRECTORY, 0666);
			if(fd < 0){
				perror(NULL);
				return 1;
			}
			//on enlève le premier "/" de param
			enlever_premier_dossier(param);
		}
		else{
			fd = open(getenv("PWD"), O_RDONLY|O_DIRECTORY, 0666);
			if(fd < 0){
				perror(NULL);
				return 1;
			}
		}
		
		//nom du premier dossier dans le chemin à parcourir
		char nom_dossier[PATH_MAX];

		//boucle de parcours
		while(param[0]!='\0'){
			prochain_dossier(nom_dossier, param);
			
			int fd_sous = openat(fd, nom_dossier, O_RDONLY|O_NOFOLLOW|O_DIRECTORY,  0666);
			if(fd_sous < 0){
				perror(NULL);
				return 1;
			}
			else{
				close(fd);
				fd = fd_sous;
			}
			enlever_premier_dossier(param);
		}
		sprintf(dernier_sym, "%s", getenv("PWD"));
		//une fois le dossier ouvert, appel de la fonction "pwd" qui mettra à jour la variable d'environement
		
		//cette fonction n'existe pas. Elle prendra en paramètre le descripteur de fichier du dossier actuel?
		//TODO: ECRIRE CETTE FONCTION
		//pwd(fd);
		close(fd);

	}
	*/

	else if(argc == 1 || 
		(argc == 2 && 
			(strcmp(argv[0], "-L") == 0 || strcmp(argv[0], "-P") == 0) )){
		char param[PATH_MAX];
		if(argc == 1){
			sprintf(param, "%s", argv[0]);
		}
		else{
			sprintf(param, "%s", argv[1]);
		}
		//stocke le nouveau chemin symbolique. Il sera écrit dans la variable "pwd" si il est valide
		char tmp[PATH_MAX];
		tmp[0] = '\0';

		//ouverture du dossier à partir duquel le parcours sera effectué
		int fd;
		//si c'est une référence relative, le parcours commence par le répertoire de travail
		if(param[0] == '/'){
			fd = open("/", O_RDONLY|O_DIRECTORY, 0666);
			if(fd < 0){
				perror(NULL);
				return 1;
			}
			//on enlève le premier "/" de param
			enlever_premier_dossier(param);
		}
		else{
			fd = open(getenv("PWD"), O_RDONLY|O_DIRECTORY, 0666);
			if(fd < 0){
				perror(NULL);
				return 1;
			}
			//comme le parcours commence par le répertoire de travail courant, c'est la valeur de $PWD qui est copiée dans tmp
			sprintf(tmp, "%s", getenv("PWD"));
		}
		
		//nom du premier dossier dans le chemin à parcourir
		char nom_dossier[PATH_MAX];

		//boucle de parcours
		while(param[0]!='\0'){
			prochain_dossier(nom_dossier, param);

			int fd_sous;
			if(argc == 1 || (argc == 2 && strcmp(argv[0], "-L") == 0)){
				fd_sous = openat(fd, nom_dossier, O_RDONLY|O_DIRECTORY,  0666);
			}
			else{
				fd_sous = openat(fd, nom_dossier, O_RDONLY|O_NOFOLLOW|O_DIRECTORY,  0666);
			}
			if(fd_sous < 0){
				if(argc == 1 || (argc == 2 && strcmp(argv[0], "-L") == 0)){
					char * nargv[] = {"-P", param};
					return cd(2, nargv);
				}
				else{
					perror(NULL);
					return 1;
				}
			}
			else{
				close(fd);
				fd = fd_sous;
			}

			if(strcmp(nom_dossier, "..") == 0){
				if(argc == 1 || (argc == 2 && strcmp(argv[0], "-L") == 0)){
					enlever_dernier_dossier(tmp);
				}
				else{
					strcat(tmp, "/..");
					if(construit_chemin(tmp, 1) !=0){
						return 1;
					}
					sprintf(tmp, "%s", getenv("PWD"));
				}
			}
			else if(strcmp(nom_dossier, ".") != 0){
				strcat(tmp, "/");
				strcat(tmp, nom_dossier);
			}
			enlever_premier_dossier(param);
		}
		close(fd);

		//il faut stocker le chemin symbolique dans "$PWD" tel quel
		if(argc == 1 || (argc == 2 && strcmp(argv[0],"-L")==0)){
			sprintf(dernier_sym, "%s", getenv("PWD"));
			setenv("PWD", tmp, 1);
		}
		//il faut stocker le chemin physique dans "$PWD"
		//pour cela faire, appel de ../pwd/pwd.c/construit_chemin
		else{
			return construit_chemin(tmp, 1);
		}
	}

	else{
		//afficher les instruction d'utilisation
		return 1;
	}
	return 0;

}
