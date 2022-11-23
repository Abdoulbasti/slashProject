#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include "../constant.h"


int pwd(int agrv, char argv[MAX_ARGS_NUMBER][MAX_ARGS_STRLEN]);

int pwdForP();

int pwdForL();

char* printError(char* error_msg);