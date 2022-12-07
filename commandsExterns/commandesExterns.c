/*#include "constant.h"
#include "commandsExterns.h"*/
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>


#define MAX_ARGS_NUMBER 4096
#define MAX_ARGS_STRLEN 4096

char** allocation()
{
    char tableauEspace[MAX_ARGS_STRLEN];
    char** argument = (char**) malloc(MAX_ARGS_NUMBER * sizeof(char*));

    for(int i = 0; i< MAX_ARGS_NUMBER; i++)
    {
        //argument[i] = (char*) malloc(sizeof(char) * (MAX_ARGS_STRLEN));
        //argument[i] = tableauEspace;
        argument[i] = NULL;
    }
    return argument;
}

/*void liberer(char* variable[])
{
    for(int i = 0; i<MAX_ARGS_NUMBER; i++)
    {
        free((char*)variable[i]);
    }
    free((char**)variable);
}*/




/*
gestion d'erreur, si le code de retour est -1 on affiche la dernière valeur de 
la variable errno
*/

void gestionErreur(char* nomFonction)
{
    perror(nomFonction);
    exit(1);
}


/*
Gestion des argument ds commandes externes aux apssage aux commandes externes
*/


/*
Recuperer une commandes et ces parametres à stocker dans un tableau de tableau.

Reserver toujours une place de plus pour le NULL

*/
//char** recupererCommandeEtArguments(char str[])
char** recupererCommandeEtArguments(char strr[MAX_ARGS_STRLEN])
{
    char** arguments = allocation();
    //char* idChaineSuivant = (char*) malloc(MAX_ARGS_STRLEN);
    char* espace = " ";
    int compteur = 0;

    //idChaineSuivant = strtok(commandesEtArguments, espace);

    //char strr[] = "Geeks for Geeks Salut";
    //char* strr = "Geeks for Geeks Salut";
    char* tokenSuivant;
    char* chainesArguments = strr;
    //char* rest = "Geeks for Geeks Salut";
    //char* rest = str;
    
    while ((tokenSuivant = strtok_r(chainesArguments, " ", &chainesArguments)))
    {
        //char temporaire[MAX_ARGS_STRLEN] = tokenSuivant;
        arguments[compteur] = tokenSuivant;
        //printf("%s\n", tokenSuivant);
        compteur++;
    }
    arguments[compteur] = NULL;
    
    //char*  test = strtok("ceci-est-un-test", "  - ");
    
    /*while (idChaineSuivant != NULL)
    {
        //sprintf(arguments[compteur], "%s", idChaineSuivant);
        printf("%s\n", idChaineSuivant);
        arguments[compteur] = idChaineSuivant;
        idChaineSuivant = strtok(NULL, espace);
        compteur++;
    }*/
    //arguments[compteur] = NULL;
    
    //return arguments;
    return arguments;
}


/*
Execution de tous les autres commandes se trouvant à un emplacement donner.
*/
void executionCommandesExternesAutres(char** arguments)
{
    int retourExec  = execv(arguments[0], arguments);
    char* exec = "execv";
    if(retourExec == -1) 
    {
        printf("exection de %s a echoue \n", exec);
        gestionErreur(exec);
    }
}


/*
Execution de la commandes extern

On execute d'abord la commande dans path, 
si ça echoue on execute alors la commandes se trouvant à un emplacement fournis par l'utilisateur
*/
void executionCommandeExternes(char** arguments)
{
    int retourExec = execvp(arguments[0], arguments);
    char* exec = "execvp";

    
    //Si l'execution d'une commande de Path echou on essaye alors celui se trouvant dans un autre emplacement
    if (retourExec == -1)
    {
        printf("exection de %s a echoue \n", exec);
        executionCommandesExternesAutres(arguments);
    }
}


/*
Execution de l'ensemble de tous les commandes externs
    ->
    ->
    ->
    ->
*/
void commandesExternes(char* chainePrompt)
{
    //char* arguments[MAX_ARGS_NUMBER] = recupererCommandeEtArguments(chainePrompt);
    char** arguments;
    allocation(arguments);
    char* nomFonction = "fork";
    pid_t pid = fork();

    if(pid == -1) { gestionErreur(nomFonction);}

    if (pid == 0)
    {
        executionCommandeExternes(arguments);
    }
    else 
    {
        wait(NULL);
    }
}


int main()
{
    char prompt[MAX_ARGS_STRLEN] = "hgjhkjlksfd autres choses et on va voir se que ça va donner";
    char** commandes = allocation();
    //Sans erreur : L'initialisation de l'eelment doit passer par un tableau initialement initialiser
    /*char test[MAX_ARGS_STRLEN] = "test test2";
    commandes[0] = test;
    recupererCommandeEtArguments(commandes[0]);
    printf("%s", commandes[0]);*/
    /*
    Core dump error
    commandes[0] = "test test2";
    recupererCommandeEtArguments(commandes[0]);
    */
    //printf("%s", commandes[0]);
    int statusWait;
    commandes = recupererCommandeEtArguments(prompt);
    char nomFonction[] = "fork";
    

    pid_t pid = fork();
    if(pid == -1) { gestionErreur(nomFonction); }
    else if(pid == 0)
    {
        //void executionCommandeExternes(char** arguments)
        //....
    }
    else
    {
        wait(&statusWait);
    }
    
    free((char**)commandes);
    return 0;
}