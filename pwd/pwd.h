#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include "../constant.h"


int pwd(int agrv, char** argv);

//affiche le lien physique du répertoire courant
int pwdForP();

//affiche le lien du répertoire courant (peut être symbolique)
int pwdForL();

char* printError(char* error_msg);

//revoi si le dossier courant est la racine
int est_racine();

/*
    construit le chemin physique à partir d'un lien symbolique
    setEnv(0:false, 1:true) : défini si le chemin construit doit 
    remplacer le lien de la variable d'environement PWD
*/
int construit_chemin(char* lien_symbolique, int setEnv);