#include <string.h>
#include <fcntl.h>
#include <limits.h>
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "cd.h"
#include "../pwd/pwd.h"

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
	char * save_pwd = getenv("PWD");
	// 1. Calculer un path en fonction des arguemnets
	// 1. a. si -L, rendre cannonique
	// 1. b. sinon, -P, realpath()
	// 2. chdir(path), setenv, ...

	char * chemin = malloc(PATH_MAX);
	if(argc == 0){
		sprintf(chemin, "%s", getenv("HOME"));
	}
	else if(argc == 1 && strcmp(argv[0],"-")==0){		
		sprintf(chemin, "%s", getenv("OLDPWD"));
	}
	else if(argc == 2 && strcmp(argv[0], "-P") == 0){
		if(argv[1][0] != '/'){
			char * tmp = malloc(PATH_MAX);
			sprintf(tmp, "%s/%s", getenv("PWD"), chemin);
			free(chemin);
			chemin = tmp;
		}
		realpath(argv[1], chemin);
		if(chemin == NULL){
			perror(NULL);
			return 1;
		}
	}
	/*else if(argc == 1 || (argc == 2 && (strcmp(argv[0], "-L") == 0))){

	}*/

	if(chdir(chemin) != 0){
		perror(NULL);
		return 1;
	}
	else{
		setenv("OLDPWD", getenv("PWD"), 1);
		setenv("PWD", chemin, 1);
	}
	return 0;
}
