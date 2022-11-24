#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include "../constant.h"


int pwd(int agrv, char** argv);

int pwdForP();

int pwdForL();

char* printError(char* error_msg);