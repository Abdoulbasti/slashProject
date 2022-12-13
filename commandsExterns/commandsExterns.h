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




char** allocation();

void gestionErreur(char* nomFonction);

char** recupererCommandeEtArguments(char chainePrompt[MAX_ARGS_STRLEN]);

void executionCommandesExternesAutres(char** arguments);

void executionCommandeExternes(char** arguments);

void commandesExternes(char* chainePrompt);