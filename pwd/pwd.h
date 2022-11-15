#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ARGS_NUMBER 4096
#define MAX_ARGS_STRLEN 4096

char courant[MAX_ARGS_STRLEN] = ".";
char pwd[MAX_ARGS_STRLEN]="";


int est_racine();

int construit_chemin();

int pwdForP();

int pwdForL();
