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
        return 0;
    }
 
    int nb_arg_ajout = 0;
    struct dirent * entry;
	while((entry = readdir(dir)) != NULL){
        if((strcmp(suffixe, (entry->d_name + strlen(entry->d_name) - strlen(suffixe))) == 0 
        || strlen(suffixe) == 0 || entry->d_type == DT_DIR) && strcmp(".", entry->d_name) != 0 && strcmp("..", entry->d_name)){
            char *arg2 = malloc(sizeof(char) * MAX_ARGS_STRLEN );
            strcpy(arg2, arg);
            strcat(arg2, entry->d_name);
            int place = -1;
            if(entry->d_type == DT_DIR){
                strcat(arg2, suffixe);
                place = is_etoile_simple(arg2);
            }
            if(entry->d_type != DT_DIR && place == -1){
                argv[num_arg + nb_arg_ajout] = arg2;
                nb_arg_ajout++;
            }else if(place != -1){
                char copyArg[MAX_ARGS_STRLEN];
                strcpy(copyArg, arg2);
                nb_arg_ajout += cherche_prefixe(argv, copyArg, place, num_arg + nb_arg_ajout);
            }            
        }
    }
 
    return nb_arg_ajout;
}

int joker(int argc, char** argv){
    char* argv2[MAX_ARGS_NUMBER];       //copie argv
    for (size_t i = 0; i < argc; i++){
        argv2[i] = argv[i];
    }
    char* argv3[MAX_ARGS_NUMBER];       //copie argv
    for (size_t i = 0; i < argc; i++){
        argv3[i] = argv[i];
    }

    int args_ajout = 0; //nbr d'arguments rajoutés
    int place = -1;
    for (size_t i = 0; i < argc; i++){
        place = is_etoile_simple(argv2[i]);
        if(place != -1){
            args_ajout += cherche_prefixe(argv, argv2[i], place, i + args_ajout);
        }else{
            char* tmp = (char*) malloc(sizeof(char)* MAX_ARGS_STRLEN);
            strcpy(tmp, argv2[i]);
            argv[i + args_ajout] = (char*) argv2[i];
        }
    }

    if(args_ajout <= 0){
       for (size_t i = 0; i < argc; i++){
            char* tmp = (char*) malloc(sizeof(char)* MAX_ARGS_STRLEN);
            strcpy(tmp, argv2[i]);
            argv[i] = tmp;
            return argc;
       } 
    }
    
    return args_ajout;
}