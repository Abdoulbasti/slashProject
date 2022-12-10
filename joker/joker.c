#include "joker.h"

int is_etoile_simple(char* arg){
    for (size_t i = 0; i < strlen(arg); i++){
        if(arg[i] == '*'){
            return i;
        }
    }
    return -1;    
}

int cherche_prefixe(char** argv2, char* arg, int place){
    char path[PATH_MAX];
    strcpy(path, getenv("PWD"));
    char suffixe[MAX_ARGS_STRLEN];
    strcpy(suffixe, arg + place + 1);
    arg[place] = '\0';
    strcat(path, "/");
    strcat(path, arg);
    path[strlen(path) - 1] = '\0';

    printf("%s : %s\n", path, suffixe);

    DIR * dir = opendir(path);
    if(dir = NULL){
        return -1;
    }
    struct dirent * entry;
    printf("TEST\n");
	while((entry = readdir(dir))){
        printf("TEST2\n");
        //printf("%s\n", entry->d_name);
    }

    return -1;
}

int joker(int argc, char** argv){
    char* argv2[MAX_ARGS_NUMBER];       //copie argv
    for (size_t i = 0; i < argc; i++){
        argv2[i] = argv[i];
    }
    
    int args_ajout = 0; //nbr d'arguments rajoutés
    int place = -1;
    for (size_t i = 0; i < argc; i++){
        place = is_etoile_simple(argv2[i]);
        if(place != -1){
            args_ajout += cherche_prefixe(argv, argv2[i], place);
        }else{
            argv[i + args_ajout] = (char*) argv2[i];
        }
    }
    return -1;
}

int main(int argc, char** argv){
    joker(argc, argv);
}