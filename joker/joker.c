#include "joker.h"

int is_etoile_simple(char* arg){
    for (size_t i = 0; i < strlen(arg); i++){
        if(arg[i] == '*'){
            return i;
        }
    }
    return -1;    
}

int cherche_prefixe(char** argv, char* arg, int place, int num_arg){
    char path[PATH_MAX];
    strcpy(path, getenv("PWD"));
    char suffixe[MAX_ARGS_STRLEN];
    strcpy(suffixe, arg + place + 1);
    arg[place] = '\0';
    strcat(path, "/");
    strcat(path, arg);
    path[strlen(path) - 1] = '\0';

    DIR * dir = opendir(path);
    if(dir == NULL){
        perror("");
        return -1;
    }

    int nb_arg_ajout = 0;
    struct dirent * entry;
	while((entry = readdir(dir)) != NULL){
        if((strcmp(suffixe, (entry->d_name + strlen(entry->d_name) - strlen(suffixe))) == 0 
        || strlen(suffixe) == 0) && strcmp(".", entry->d_name) != 0 && strcmp("..", entry->d_name)){
            argv[num_arg + nb_arg_ajout] = entry->d_name;
            nb_arg_ajout++;
        }
    }

    return nb_arg_ajout;
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
            args_ajout += cherche_prefixe(argv, argv2[i], place, i + args_ajout);
        }else{    printf("%d\n", args_ajout);
            argv[i + args_ajout] = (char*) argv2[i];
        }
    }
    return args_ajout - 1;
}