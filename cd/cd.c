#include <string.h>
#include <fcntl.h>
#include <limits.h>
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "cd.h"
#include "../pwd/pwd.h"

//insère dans "nom_dossier" un string contenant les caractères jusqu'au premier "/" de "chemin" 
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

void enlever_premier_charactere(char * string){
	if(strlen(string)>1){
		char * tmp = malloc(PATH_MAX);
		strcpy(tmp, string+1);
		strcpy(string, tmp);
		free(tmp);
	}
}

//prend en paramètre un réference absolue (commençant par '/'), et renvoie sa forme canonique (sans "." et "..")
void forme_canonique(char * chemin){
	char * res = malloc(PATH_MAX);
	char * nom_dossier = malloc(PATH_MAX);
	strcpy(res, "");
	strcpy(nom_dossier, "");
	enlever_premier_charactere(chemin);
	while(chemin[0] != '\0'){
		prochain_dossier(nom_dossier, chemin);
		//disjonction des cas en fontion du premier dossier
		//".": ne rien faire
		//"..": enlever les dernier dossier de "res", sauf si "res" est la chaîne "/". Dans ce cas, ne rien faire
		//autre: l'ajouter à "res"
		if(strcmp(nom_dossier, ".") == 0){
		}
		else if(strcmp(nom_dossier, "..") == 0){
			enlever_dernier_dossier(res);
		}
		else{
			strcat(res, "/");
			strcat(res, nom_dossier);
		}
		enlever_premier_dossier(chemin);

	}
	free(nom_dossier);
	strcpy(chemin, res);
	free(res);
}

int cd(int argc, char **argv){
	// 1. Calculer un path en fonction des arguemnets
	// 1. a. si -L, rendre cannonique
	// 1. b. sinon, -P, realpath()
	// 2. chdir(path), setenv, ...

	//booléens indiquant l'interprétation souhaitée selon les paramètres
	int interpretation_physique = (argc == 2 && strcmp(argv[0], "-P") == 0);
	int interpretation_logique = (argc == 1 || (argc == 2 && (strcmp(argv[0], "-L") == 0)));

	char * chemin = malloc(PATH_MAX);
	//variable sotckant le chemin passé en paramètre
	char * param = malloc(PATH_MAX);
	//"cd" tout court
	if(argc == 0){
		sprintf(chemin, "%s", getenv("HOME"));
	}
	//"cd -"
	else if(argc == 1 && strcmp(argv[0],"-")==0){		
		sprintf(chemin, "%s", getenv("OLDPWD"));
	}
	//"cd [-L|-P] chemin"
	else{
		//si il y a deux arguments, le chemin se trouve dans le deuxième
		if(argc == 2){
			sprintf(param, "%s", argv[1]);
		}
		//si il y un argument, c'est le chemin
		else if(argc == 1){
			sprintf(param, "%s", argv[0]);
		}
		//si le chemin passé en paramètre est une référence relative, ajouter "pwd" devant
		if(param[0] != '/'){
			sprintf(chemin, "%s/%s", getenv("PWD"), param);
		}
		else{
			sprintf(chemin, "%s", param);
		}

		//interprétation physique
		if(interpretation_physique){
			realpath(param, chemin);
			if(chemin == NULL){
				perror(NULL);
				free(chemin);
				free(param);
				return 1;
			}
		}
		//interprétation logique
		else if(interpretation_logique){
			forme_canonique(chemin);
		}
		else{
			free(chemin);
			free(param);
			exit(1);
		}
	}

	if(chdir(chemin) != 0){
		if(interpretation_logique){
			char * arg2[2];
			arg2[0] = malloc(PATH_MAX);
			arg2[1] = malloc(PATH_MAX);
			strcpy(arg2[0], "-P");
			strcpy(arg2[1], param);
			int retour = cd(2, arg2);
			free(arg2[0]);
			free(arg2[1]);
			free(chemin);
			free(param);
			return retour;
		}
		else{
			free(chemin);
			free(param);
			perror(NULL);
			return 1;
		}
	}
	else{
		setenv("OLDPWD", getenv("PWD"), 1);
		setenv("PWD", chemin, 1);
	}
	free(chemin);
	free(param);

	return 0;
}
