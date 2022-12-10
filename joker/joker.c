#include "joker.h"

int is_etoile_simple(char* argv){
    for (size_t i = 0; i < strlen(argv); i++){
        if(argv[i] == '*'){
            return i;
        }
    }
    return -1;    
}

char** joker(int argc, char** argv){
    char** argv2;       //argv à renvoyer
    int args_ajout = 0; //nbr d'arguments rajoutés
    for (size_t i = 0; i < argc; i++){
        int place = is_etoile_simple(argv[i]);
        if(place != -1){

        }else{
            argv2[i + args_ajout] = argv[i];
        }
    }
}

int main(int argc, char** argv){
    for (size_t i = 0; i < argc; i++){
        printf("%ld: %d\n", i, is_etoile_simple(argv[i]));
    }
    
    
}