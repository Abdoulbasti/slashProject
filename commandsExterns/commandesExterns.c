#include "constant.h"
#include "commandsExterns.h"


/*
Ajout de NULL à la fin de du tableau de tableau de la chaine recuperer par depuis le prompt
*/
void ajouterNULL(char** arguments)
{
    int taille = strlen(arguments);
    arguments[taille] = NULL;
}


/*
gestion d'erreur, si le code de retour est -1 on affiche la dernière valeur de 
la variable errno
*/

void gestionErreurEntier(char* nomFonction, int retourFonction)
{
    if(retourFonction == -1)
    {
        perror(nomFonction);
        exit(1);
    }
}

void gestionErreurPointeur(char* nomFonction, void* poiteurRetour)
{
    if(poiteurRetour == NULL)
    {
        perror(nomFonction);
        exit(1);
    }
}



/*
Gestion des argument ds commandes externes aux apssage aux commandes externes
*/


/*
Recuperer une commandes et ces parametres à stocker dans un tableau de tableau.

Reserver toujours une place de plus pour le NULL

*/
char** recupererCommandeEtArguments(char* commandesEtArguments)
{
    char** arguments;
    char* idChaineSuivant;
    char* espace = " ";
    int compteur = 0;

    idChaineSuivant = strtok(commandesEtArguments, espace);

    while (idChaineSuivant != NULL)
    {
        sprintf(arguments[compteur], "%s", idChaineSuivant);
        idChaineSuivant = strtok(NULL, espace);
        compteur++;
    }
    arguments[compteur] = NULL;
    
    return arguments;
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
        executionCommandesExternesAutres(arguments);
    }
}


/*
Execution de tous les autres commandes se trouvant à un emplacement donner.
*/
void executionCommandesExternesAutres(char** arguments)
{
    int retourExec  = execv(arguments[0], arguments);
    char* exec = "execv";
    gestionErreurEntier(exec, retourExec);
}


/*
Execution de l'ensemble de tous les commandes externs
    ->
    ->
    ->
    ->
*/
void commandesExternes(char** arguments)
{
    pid_t pid = fork();
    char* nomFonction = "fork";

    gestionErreurEntier(nomFonction, (int)pid);
    if (pid == 0)
    {
        executionCommandeExternes(arguments);
    }
    else 
    {
        wait(NULL);
    }
}