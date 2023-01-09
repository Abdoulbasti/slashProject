#include "commandesExternes.h"
#include <signal.h>
#include <errno.h>

struct sigaction sa_dfl = {.sa_handler = SIG_DFL, 0};
struct sigaction sa_ign = {.sa_handler = SIG_IGN, 0};
//memset(&sa, 0, sizeof(struct sigaction));
//sa.sa_handler = SIG_DFL;

char** allocation()
{
    char** argument = (char**) malloc(MAX_ARGS_NUMBER * sizeof(char*));

    for(int i = 0; i< MAX_ARGS_NUMBER; i++)
    {
        argument[i] = NULL;
    }
    return argument;
}

void gestionErreur(char* nomFonction)
{
    perror("bash :");
    exit(1);
}


/*
Execution de tous les autres commandes se trouvant à un autre emplacement que celui du path.
*/
int executionCommandesExternesAutres(char* arguments[MAX_ARGS_NUMBER])
{
    int codeRetour = 0;

    //retour au comportement par défaut avant l'exécution de la commande externe
    sigaction(SIGINT, &sa_dfl, NULL);
    sigaction(SIGTERM, &sa_dfl, NULL);

    int retourExec  = execv(arguments[0], arguments);

    char* exec = "execv";
    if(retourExec == -1) 
    {
        codeRetour = 1;
        gestionErreur(exec);
        return errno;
    }
}


/*
Execution des commandes extern
    -> On execute d'abord la commande dans path
    -> Si ça echoue on execute alors la commandes se trouvant à un emplacement fournis par l'utilisateur
*/
int executionCommandeExternes(char* arguments[MAX_ARGS_NUMBER])
{
    int codeRetour =0;

    //retour au comportement par défaut avant l'exécution de la commande externe
    sigaction(SIGINT, &sa_dfl, NULL);
    sigaction(SIGTERM, &sa_dfl, NULL);
    
    int retourExec = execvp(arguments[0], arguments);

    char* exec = "execvp";
    
    //Si l'execution d'une commande de Path échoue on essaye alors celui se trouvant dans un autre emplacement
    if (retourExec == -1)
    {
        codeRetour = executionCommandesExternesAutres(arguments);

        return codeRetour;
    }
}


/*
Execute l'ensemble des commandes externes installer dans la machine et 
tout les autres commandes se trouvant à un emplacement fournis par 
l'utilisateur.
*/
int commandesExternes(char* commandes[MAX_ARGS_NUMBER])
{
    char* nomFonction = "fork";
    
    pid_t pid = fork();
    int codeRetour, statusWait;
    if(pid == -1) 
    { 
        gestionErreur(nomFonction); 
        codeRetour = 1;
    }
    else if(pid == 0)
    {
        codeRetour = executionCommandeExternes(commandes);

        return codeRetour;
    }
    else 
    {
        wait(&statusWait);
        nomFonction = "wait";
        /*if (retourWait == -1) 
        { 
            gestionErreur(nomFonction);
            codeRetour = 1;
        }*/
        if(WIFEXITED(statusWait)){
            codeRetour = 0;
        }
        else{
            if(WIFSIGNALED(statusWait)){
                codeRetour = 255;
            }
            else{
                codeRetour = 1;
            }
        }
    }

    return codeRetour;
}