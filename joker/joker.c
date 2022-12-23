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

int last_slash(char* arg){
    for (int i = strlen(arg) - 1; i >= 0 ; i--){
        if(arg[i] == '/'){
            return i;
        }
    }
    return -1;
}

char *  del_double_slash(char* arg){
    int len = strlen(arg);
    int len2 = 0;
    char* arg2 = (char*) malloc(sizeof(char) * PATH_MAX);
    for (size_t i = 0; i < len; i++){
        if(i < len - 1 && arg[i] == '/'){
            if(arg[i + 1] != '/'){
                arg2[len2] = arg[i];
                len2++;
            }
        }else{
                arg2[len2] = arg[i];
                len2++;
        }
    }
    arg2[len2] = '\0';
    free(arg);
    return arg2;
}

int check_file_exist(char* arg){
    int place = last_slash(arg);
    if(arg[strlen(arg) - 1] == '/'){
        arg[strlen(arg) - 1] = '\0';
        place = last_slash(arg);
    }

    char filename[MAX_ARGS_STRLEN];
    char path[PATH_MAX];
    strcpy(filename, place + arg + 1);

    if(arg[0] != '/'){
        strcpy(path, getenv("PWD"));
    }else{
        strcpy(path, "");
    }

    if(place != -1){
      strcat(path, "/");
        strncat(path, arg, place);  
    }

    //On essaye d'ouvrir le dossier correspondant à path
    DIR * dir = opendir(path);
    if(dir == NULL){
        return 0;
    }

    struct dirent * entry;
    while((entry = readdir(dir)) != NULL){
        if(strcmp(entry->d_name, filename) == 0){
            closedir(dir);
            return 1;
        }
    }
    closedir(dir);
    return 0;
}

int cherche_prefixe(char** argv, char* arg, int place, int num_arg){
    char path[PATH_MAX];    //Chemin absolu
    if(arg[0] != '/'){
        strcpy(path, getenv("PWD"));
    }else{
        strcpy(path, "");
    }
    
    char suffixe[MAX_ARGS_STRLEN];
    char suite[PATH_MAX] = "\0";
    //On recherche si il y a un slash après l'étoile
    int place_next_slash = next_slash(arg + place);
    int type = 0;   //Si type = DT_DIR alors on recherche un dossier sinon un fichier

    //On recherche un dossier si il y a un slash après l'étoile
    if( place_next_slash != -1){
        //On garde la partie à partir du slash dans suite
        strcpy(suite, arg + place_next_slash + 1 + place);
        //On coupe l'arg au niveau du slash pour garder le suffixe
        arg[place_next_slash + place] = '\0';
        //On garde le suffix du nom du dossier
        strcpy(suffixe, arg + place + 1);
        //On précise qu'on cherche un dossier
        type = DT_DIR;
    }else{
        //On garde le suffix du nom du fichier
        strcpy(suffixe, arg + place + 1);
    }
    //On garde dans arg toute la partie avant l'étoile et l'ajoute au chemin
    arg[place] = '\0';
    strcat(path, "/");
    strcat(path, arg);
    //On enlève le slash à la fin
    path[strlen(path) - 1] = '\0';

    //Si il y a une étoile dans le suffixe on renvoi 0
    if(is_etoile_simple(suffixe) != -1){
        return 0;
    }
    

    //On essaye d'ouvrir le dossier correspondant à path
    DIR * dir = opendir(path);
    if(dir == NULL){
        return 0;
    }

    char * arg2;
    int nb_arg_ajout = 0;
    struct dirent * entry;

    //Pour chaque element de dir
	while((entry = readdir(dir)) != NULL){

        //Si le suffixe et le type correspondent et qu'il nest pas dir ou son père
        if((strcmp(suffixe, (entry->d_name + strlen(entry->d_name) - strlen(suffixe))) == 0 
        || strlen(suffixe) == 0) && strcmp(".", entry->d_name) != 0 && strcmp("..", entry->d_name)
        && entry->d_name[0] != '.'){
            char *arg2 = malloc(sizeof(char) * MAX_ARGS_STRLEN );
            strcpy(arg2, arg);
            strcat(arg2, entry->d_name);
            int place = -1; //Place de la première étoile de l'arg

            //Si l'entrée est un dossier on ajoute la suite à l'arg et vérifi si il n'y a pas d'autres étoiles
            if(type == DT_DIR){
                strcat(arg2, "/");
                strcat(arg2, suite);
                place = is_etoile_simple(arg2);
            }

            //Si l'entrée n'est pas un dossier on ajoute le fichier aux arguments
            if(entry->d_type != DT_DIR && place == -1){
                argv[num_arg + nb_arg_ajout] = arg2;
                nb_arg_ajout++;


            //Si l'entrée est un fichier et qu'il y a une autre étoile on continu de chercher
            }else if(place != -1){
                char copyArg[MAX_ARGS_STRLEN];
                strcpy(copyArg, arg2);
                free(arg2);
                nb_arg_ajout += cherche_prefixe(argv, copyArg, place, num_arg + nb_arg_ajout);

            //Si l'entrée est un fichier et qu'il n'y a pas d'étoile
            }else{
                if(check_file_exist(arg2) == 1){
                    argv[num_arg + nb_arg_ajout] = arg2;
                    nb_arg_ajout++;
                }else{
                    free(arg2);
                }
            }
        }
    }
    
    closedir(dir);
    return nb_arg_ajout;
}

int joker(int argc, char** argv){
    //copie argv pour les utilisés dans cherche_prefixe
    char* argv2[MAX_ARGS_NUMBER];
    for (size_t i = 0; i < argc; i++){
        argv2[i] = argv[i];
    }
    //copie argv pour les remettre dans argv si il n'y a pas d'argument à la fin
    char* argv3[MAX_ARGS_NUMBER];
    for (size_t i = 0; i < argc; i++){
        argv3[i] = argv[i];
    }
    int nbEtoiles = 0;
    int args_ajout = 0; //nbr d'arguments rajoutés
    int place = 0;     //place de la première étoile de l'arg i
    for (size_t i = 0; i < argc; i++){
        argv2[i] = del_double_slash(argv2[i]);
        place = is_etoile_simple(argv2[i]);
        //Il y a une étoile on applique `cherche_prefixe` sur l'arg
        if(place != -1){
            nbEtoiles++;
            char * a_supp = argv2[i];
            args_ajout += cherche_prefixe(argv, argv2[i], place, args_ajout);
            free(a_supp);
        //Il n'y a pas d'étoile on ajoute simplement l'argument
        }else{
            argv[i + args_ajout] = (char*) argv2[i];
        }
    }

    //Aucun argument n'a été modifié
    if(!nbEtoiles){
        return -1;
    }

    //On remet les arguments de base si il n'y a plus d'arguments à la fin si il y en avait au début
    if(args_ajout <= 0 && argc != 0){
       for (size_t i = 0; i < argc; i++){
            char* tmp = (char*) malloc(sizeof(char)* MAX_ARGS_STRLEN);
            strcpy(tmp, argv2[i]);
            argv[i] = tmp;
            return argc;
       } 
    }
    
    // for (size_t i = 0; i < args_ajout; i++)
    // {
    //     printf("%ld = %s\n", i, argv[i]);
    // }
    

    return args_ajout;
}