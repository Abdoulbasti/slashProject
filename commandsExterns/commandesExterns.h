#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>
#include "../constant.h"


void recupererCommandeEtArguments(char line[]);

void gestionErreur(char* nomFonction);

int executionCommandesExternesAutres(char* arguments[MAX_ARGS_NUMBER]);

int executionCommandeExternes(char* arguments[MAX_ARGS_NUMBER]);

int commandesExternes(char* commandes[MAX_ARGS_NUMBER]);