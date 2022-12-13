#include "commandsExterns.h"

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


void gestionErreur(char* nomFonction)
{
    perror(nomFonction);
    exit(1);
}

/*
Recuperer la chainePrompt spliter la chaine sur la base du caractère " " et stocker 
tous les chaines recuperer dans un char* argument[] et ajouter NULL à la fin pour 
indiquer la fin*/
char** recupererCommandeEtArguments(char chainePrompt[MAX_ARGS_STRLEN])
{
    char** arguments = allocation();
    char* espace = " ";
    int compteur = 0;
    char* tokenSuivant;
    char* chainesArguments = chainePrompt;
    
    while ((tokenSuivant = strtok_r(chainesArguments, espace, &chainesArguments)))
    {
        arguments[compteur] = tokenSuivant;
        compteur++;
    }
    arguments[compteur] = NULL;

    return arguments;
}


/*
Execution de tous les autres commandes se trouvant à un autre emplacement que celui du path.
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
Execution des commandes extern
    -> On execute d'abord la commande dans path
    -> Si ça echoue on execute alors la commandes se trouvant à un emplacement fournis par l'utilisateur
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
Execution de l'ensemble de tous les commandes externs, celui du path et tous les autres 
*/
void commandesExternes(char* chainePrompt)
{
    char** commandes = allocation();
    int statusWait;
    commandes = recupererCommandeEtArguments(chainePrompt);
    char* nomFonction;
    nomFonction = "fork";
    
    pid_t pid = fork();
    if(pid == -1) { gestionErreur(nomFonction); }
    else if(pid == 0)
    {
        executionCommandeExternes(commandes);
    }
    else 
    {  
        int retourWait  = wait(&statusWait);
        nomFonction = "wait";
        if (retourWait == -1) { gestionErreur(nomFonction);}
    }
    
    free((char**)commandes);
}


/*int main()
{
    char line[] = "cat";

    commandesExternes(line);
}*/