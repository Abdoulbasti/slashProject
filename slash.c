#include <stdlib.h>
#include <stdio.h> 
#include <unistd.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/stat.h>
#include "pwd/pwd.h"

#define MAX_PATH 1000
#define MAX_ARGS_NUMBER 4096
#define MAX_ARGS_STRLEN 4096

char prompt_msg[30];        //Message du prompt
int last_return_value = 0;  //valeur retour de la dernière commande
char args[MAX_ARGS_NUMBER][MAX_ARGS_STRLEN];    //arguments de la commande entrée dans le prompt
char command[MAX_ARGS_STRLEN];      //commande entrée dans le prompt     
char chemin_sym[MAX_PATH] = "/";    //Chemin relatif

//initialiser args a

char* prompt_format(){

    //Dernière commande exécutée
    char tmp[3];
    sprintf(tmp, "%d", last_return_value);
    strcpy(prompt_msg, (const char*) "[");
    strcat(prompt_msg, (const char*) tmp);
    strcat(prompt_msg, (const char*) "]");

    //Chemin symbolique
    strcat(prompt_msg, (const char*) chemin_sym);
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
    int split_line(char* line):
    recupère la commande et ses arguments pour les mettres dans les variable command et args
    et renvoie le nombre d'arguments
*/
int split_line(char* line){
    char *tmp;
    int i = 0;  //nombre d'arguments

    //Rien n'est rentré dans le prompt
    if(strcmp((const char*) line, (const char*) "") == 0){
        strcpy(command, (const char*) "");
        return 0;
    }

    tmp = strtok(line, " ");    //découpe la partie avant le première espace
    strcpy(command, (const char*) tmp);     //la première partie est la commande
    while(tmp != NULL){
        if(i != 0){
            //on ajoute un argument
            strcpy(args[i-1], (const char*) tmp);
        }
        tmp = strtok(NULL, " ");    //découpe la partie avant le première espace
        i++;
    }
    return i-1;
}


/*
    void fexit(int val):
    termine le programme avec la valeur val
*/
void fexit(int val){
    exit(val);
}


/*
    int interpretation_command(int argc):
    regarde quel commande correspond a quel fonction et l'execute
    puis renvoie la valeur de retour de la fonction
*/
int interpretation_command(int argc){
    //aucune commande
    if (strcmp((const char*) command, (const char*) "") == 0){
        return last_return_value;
    }

    //commande exit
    if(strcmp((const char*) command, (const char*) "exit") == 0){
        switch (argc)
        {
            case 0:
                fexit(last_return_value);
                break;
            case 1:
                int tmp;
                sscanf(args[0], "%d", &tmp);
                fexit(tmp);
                break;
            default:
                print_error("exit: too many arguments");
                return 1;
        }
    }

    //Commande pwd
    if(strcmp((const char*) command, (const char*) "pwd") == 0)
    {
        //pwd -P
        if (strcmp(args[0], "-P") == 0) 
        {
            last_return_value = pwdForP();
            //return last_return_value;
        }
        //Cas par defaut, cas d'un lien logique
        else if (strcmp(args[0], "-L") == 0 || strcmp(args[0], "")==0)
        {
            last_return_value = pwdForL();
            //return last_return_value;
        }
        else
        {
            print_error("pwd: wrong argument");
            last_return_value = 1;
        }
    }

    //Commande cd
    
    return last_return_value;
}

int main(int argc, char **argv){
    char* line = (char*)NULL;
    rl_outstream = stderr;  //changement de la sortie vers la sortie d'erreur
    while(1){
        //affiche le prompt et attend l'utilisateur
        line = readline(prompt_format());
        //permet de retrouver une commande exécutée avec les flèches du haut et du bas
        add_history(line);

        int nb_args = split_line(line);
        last_return_value = interpretation_command(nb_args);
    }
}