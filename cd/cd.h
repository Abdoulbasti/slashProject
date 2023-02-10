#include <string.h>
#include <fcntl.h>
#include <limits.h>
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include "../constant.h"

void prochain_dossier(char * nom_dossier, char * chemin);

void enlever_premier_dossier(char * chemin);

void enlever_dernier_dossier(char * chemin);

void forme_canonique(char * chemin);

int cd(int argc, char **argv);