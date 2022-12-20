#include "joker.h"

int is_etoile_simple(char* arg){
    for (size_t i = 0; i < strlen(arg); i++){
        if(arg[i] == '*'){
            return i;
        }
    }
    return -1;
}

int next_slash(char* arg){
    for (size_t i = 0; i < strlen(arg); i++){
        if(arg[i] == '/'){
            return i;
        }
    }
    return -1;
}

int cherche_prefixe(char** argv, char* arg, int place, int num_arg){
    char path[PATH_MAX];
    strcpy(path, getenv("PWD"));
    char suffixe[MAX_ARGS_STRLEN];
    char suite[PATH_MAX] = "\0";
    int place_next_slash = next_slash(arg + place + 1);
    if( place_next_slash != -1){
        strcpy(suite, arg + place_next_slash + 1);
        arg[place_next_slash + 1] = '\0';
        strcpy(suffixe, arg + place + 1);
    }else{
        strcpy(suffixe, arg + place + 1);
    }
    arg[place] = '\0';
    strcat(path, "/");
    strcat(path, arg);
    path[strlen(path) - 1] = '\0';
    printf("suite = %s\n", suite);
    printf("suffixe = %s\n", suffixe);
    printf("arg = %s\n", arg);
    
    DIR * dir = opendir(path);
    if(dir == NULL){
        perror("");
        return 0;
    }


    char * arg2;
    int nb_arg_ajout = 0;
    struct dirent * entry;
	while((entry = readdir(dir)) != NULL){
        if((strcmp(suffixe, (entry->d_name + strlen(entry->d_name) - strlen(suffixe))) == 0 
        || strlen(suffixe) == 0 || entry->d_type == DT_DIR) && strcmp(".", entry->d_name) != 0 && strcmp("..", entry->d_name)){
            //char *arg2 = malloc(sizeof(char) * MAX_ARGS_STRLEN );
            strcpy(arg2, arg);
            printf("arg2 = %s\n", arg2);
            strcat(arg2, entry->d_name);
            int place = -1;
            if(entry->d_type == DT_DIR){
                printf("test\n");
                strcat(arg2, suite);
                place = is_etoile_simple(arg2);
            }
            if(entry->d_type != DT_DIR && place == -1){
                argv[num_arg + nb_arg_ajout] = arg2;
                nb_arg_ajout++;
            }else if(place != -1){
                printf("test2\n");
                char copyArg[MAX_ARGS_STRLEN];
                strcpy(copyArg, arg2);
                nb_arg_ajout += cherche_prefixe(argv, copyArg, place, num_arg + nb_arg_ajout);
            }

        }
    }
    free(arg2);
 
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
    int nbEtoiles = 0;
    int args_ajout = 0; //nbr d'arguments rajoutés
    int place = -1;
    for (size_t i = 0; i < argc; i++){
        place = is_etoile_simple(argv2[i]);
        if(place != -1){
            nbEtoiles++;
            args_ajout += cherche_prefixe(argv, argv2[i], place, i + args_ajout);
        }else{
            argv[i + args_ajout] = (char*) argv2[i];
        }
    }
    if(!nbEtoiles){
        return -1;
    }

    if(args_ajout <= 0 && argc != 0){
       for (size_t i = 0; i < argc; i++){
            char* tmp = (char*) malloc(sizeof(char)* MAX_ARGS_STRLEN);
            strcpy(tmp, argv2[i]);
            argv[i] = tmp;
            free(tmp);
            return argc;
       } 
    }
    
    return args_ajout;
}