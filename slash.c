#include <unistd.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "cd/cd.h"
#include "pwd/pwd.h"
#include "constant.h"
#include "cd/cd.h"
#include "joker/joker.h"
#include <stdlib.h>
#include <string.h>
#include "commandsExterns/commandesExterns.h"

char prompt_msg[100];       //Message du prompt
int last_return_value = 0;  //valeur retour de la dernière commande
char* args[MAX_ARGS_NUMBER];    //arguments de la commande entrée dans le prompt
char command[MAX_ARGS_STRLEN];      //commande entrée dans le prompt     
char chemin_sym[PATH_MAX] = "/";    //Chemin relatif



/*
    char* prompt_format() :
    Créer le prompt sous le format : [0]/Documents$
        [0]:            retour de la dernière commande exécutée
        /Documents :    chemin relatif
    le prompt est écrit directement dans la variable prompt_msg que la fonction renvoie
*/
char* prompt_format(){

    //Dernière commande exécutée
    char last_return_value_str[4];
    sprintf(last_return_value_str, "%d", last_return_value);

    //Couleur (vert: succès / rouge échec)
    char* first_color = "[";
    if(last_return_value != 0){
        first_color = "\033[91m[";
    }else{
        first_color = "\033[32m[";
    }
    strcpy(prompt_msg, (const char*) first_color);
    strcat(prompt_msg, (const char*) last_return_value_str);
    strcat(prompt_msg, (const char*) "]");

    //Chemin symbolique
    strcat(prompt_msg, (const char*) "\033[34m");

    strcpy(chemin_sym, (const char*) getenv("PWD"));
    int len_chemin = strlen(chemin_sym);
    int len_tmp = strlen(last_return_value_str);
    // + len_tmp = len last_return_value; + 4 []$' '
    if( len_chemin + len_tmp + 4 > 30){
        strcat(prompt_msg, (const char*) "...");
        // + len_tmp = taille last_return_value; + 7 = []...$' '; -31 = -30(taille max) -1 = taille vers indice
        strcat(prompt_msg, (const char*) &chemin_sym[len_chemin - 31 + len_tmp + 8]);
    }else{
        strcat(prompt_msg, (const char*) chemin_sym);
    }
    strcat(prompt_msg, (const char*) "\033[00m");
    strcat(prompt_msg, (const char*) "$ ");
    return prompt_msg;
}

/*
    char* print_error(char* error_msg):
    affiche le message d'erreur sur la sortie d'erreur et le renvoi
*/
char* print_error(char* error_msg){
    write(STDERR_FILENO, (const void*) error_msg, strlen(error_msg));
    write(STDERR_FILENO, (const void*) "\n", 1);
}


/*
    void fexit(int val):
    termine le programme avec la valeur val
*/
void fexit(int val){
    exit(val);
}


/*
    int split_line(char* line):
    recupère la commande et ses arguments pour les mettres dans les variable command et args
    et renvoie le nombre d'arguments
*/
int split_line(char* line){
    if(line == NULL){
        strcpy(command, (const char*) "");
        fexit(last_return_value);
    }
    //char *tmp = (char *) malloc(sizeof(char) * MAX_ARGS_STRLEN);
    char * tmp;
    int i = 0;  //nombre d'arguments

    //Rien n'est rentré dans le prompt
    if(strcmp((const char*) line, (const char*) "") == 0){
        strcpy(command, (const char*) "");
        return 0;
    }
    //strcpy(tmp, strtok(NULL, " "));
    char * tmp2 = strtok(line, " ");   //découpe la partie avant le première espace
    strcpy(command, (const char*) tmp2);     //la première partie est la commande
    while(tmp2 != NULL){
        if(i != 0){
            //on ajoute un argument
            args[i-1] = tmp;
        }
        tmp2 = strtok(NULL, " ");       //découpe la partie avant le première espace
        if(tmp2 != NULL){
            //free(tmp);
            tmp = (char *) malloc(sizeof(char) * MAX_ARGS_STRLEN);
            strcpy(tmp, tmp2);
        }
        //tmp = strtok(NULL, " ");    
        i++;
    }
    return i-1;
}


/*

int interpretation_command(int argc, char commandesAndArguments):
    regarde quel commande correspond a quel fonction et l'execute
    puis renvoie la valeur de retour de la fonction*/

int interpretation_command(int argc){
    //aucune commande
    
    if (strcmp((const char*) command, (const char*) "") == 0){
        return last_return_value;
    }

    //commande exit
    if(strcmp((const char*) command, (const char*) "exit") == 0){
        int tmp_exit_value;
        switch (argc)
        {
        case 0:
            fexit(last_return_value);
            break;
        case 1:
            sscanf((const char*) args[0], "%d", &tmp_exit_value);
            fexit(tmp_exit_value);
            break;
        default:
            print_error("exit: too many arguments");
            return 1;
        }
    }else
    //Commande pwd
    if(strcmp((const char*) command, (const char*) "pwd") == 0){
        return pwd(argc, args);
    }else
    //Comande cd
    if(strcmp((const char*) command, (const char*) "cd") == 0){
        return cd(argc, args);
    }else if(strcmp((const char*) command, (const char*) "true") == 0){
        return 0;
    }else if(strcmp((const char*) command, (const char*) "false") == 0){
        return 1;
    }
    //commandes externs
    else
    {
        char* argvTmp[MAX_ARGS_STRLEN];
        //initialisation des champs
        for(int k = 0; k<=MAX_ARGS_STRLEN; k++)
        {
            argvTmp[k] = NULL;
        }
        argvTmp[0] = command;
        for (size_t i = 0; i < argc; i++){
            argvTmp[i+1] = args[i];
        }
        return commandesExternes(argvTmp);
    }
    
    //Commande introuvable
    char error_msg[100];
    strcpy(error_msg, (const char*) command);
    strcat(error_msg, (const char*) " : command not found");
    print_error(error_msg);
    return last_return_value;
}

void freeAll(int argc){
    for (size_t i = 0; i < argc; i++){
        if(args[i] != NULL){
            free(args[i]);
        }
    }
}

int main(int argc, char **argv){
    char* line = (char*)NULL;
    rl_outstream = stderr;  //changement de la sortie vers la sortie d'erreur
    

    while(1){
        //affiche le prompt et attend l'utilisateur
        line = readline(prompt_format());        
        


        //permet de retrouver une commande exécutée avec les flèches du haut et du bas
        add_history(line);    

        /*
        S'il y'a l'existance de au moins un des signes de redirections
        -On recupère la chaine se trouvant avant le tous premier signe
        redirections et le metre dans lineAvantRedrection
        -On applique la toute première redirection vers le fichier concerné,
        puis 
        -On effectue un split de lineAvantRedrection(1), 2 et 3

        */
        
        //1
        int nb_args = split_line(line);
        //2
        int joker_return_value = joker(nb_args, args);
        //3
        if(joker_return_value != -1){
            nb_args = joker_return_value;
        }

        // for (size_t i = 0; i < nb_args; i++){
        //     printf("%ld = %s\n", i, args[i]);
        // }

        /*TRAITEMENTS DES REDIRECTIONS*/
        


        last_return_value = interpretation_command(nb_args) % 256;  //return_value entre -256 et 256

        //libération des arguments
        freeAll(nb_args);
        free(line);
    }
}