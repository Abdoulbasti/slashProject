#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>
#include "../constant.h"

int joker(int argc, char** argv);


/*
 * prend une chaine de caractère et la position de la première étoile si il y en a une
 * et renvoi -1 sinon
*/
int is_etoile_simple(char* argv);