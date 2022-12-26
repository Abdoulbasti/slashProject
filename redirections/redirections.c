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
#include "../constant.h"
#include <sys/types.h>
#include <fcntl.h>

#define MAX_REDIRECTIONS 10
#define MAX_FICHIERS 10


//cmd > fic
void redirectionsStdoutSansEcrasement(char* nomFichier);

//cmd >| fic
void redirectionsStdoutAvecEcrasement(char* nomFichier);

//cmd >> fic
void redirectionsStdoutEnConcatenation(char* nomFichier);



//cmd 2>> fic
void redirectionsStderrEnConctenation(char* nomFichier);

//cmd 2> fic
void redirectionsStderrSansEcrasement(char* nomFichier);

//cmd 2>| fic
void redirectionsStderrAvecEcrasement(char* nomFichier);





int ouvertureFichierEnConcatenation(char* nomFichier);

int ouvertureFichierAvecEcrasement(char* nomFichier);

int ouvertureFichierSansEcrasement(char* nomFichier);



int ouvertureFichierStdin(char* nomFichier);


//cmd < fic
void redirectionsStdin(char* nomFichier);

//cmd1 | cmd2   et cmd1 | cmd2 | ... | cmdn
void redirectionsPipe(char* monFichier);

//void redirectionStdout(char* commandesIntvoid redirectionStdout(char* commandesInternesExternes[MAX_ARGS_NUMBER], char* redirection, char* nomFichier);ernesExternes[MAX_ARGS_NUMBER], char* redirection, char* nomFichier);

void redirectionsStdout(char* redirection, char* nomFichier);

void redirectionsStderr(char* redirection, char* nomFichier);

//void redirections(char* typesRedirections, );


int compterNombreRedirections(char* line);

int verifierExisteRedirection(char* line);

void* recupererChaineAvantRedirection(char* line, char* sub_string);

char* recupererChaineApresRedirection(char* line);

void stockerRedirections(char* line, char* stocker[MAX_REDIRECTIONS]);

char** splitChaine(char *str, const char *delimiter);


int main(int argc, char** argv)
{
    char* sousChaine = NULL;
    char* line = "./split.sh 11 < texte > dossier/out 2  %PWD%/dossier/err";


    /*char str[] = "Je suis une chaîne de caractères.";
    char **substrings;
    int i;

    substrings = splitChaine(line, " ");
    for (i = 0; substrings[i] != NULL; i++) {
        printf("Sous-chaîne %d : %s\n", i + 1, substrings[i]);
    }

    free(substrings);*/

    char* stocker[MAX_REDIRECTIONS];
    for(int k = 0; k<MAX_REDIRECTIONS; k++)
    {
        stocker[k] = NULL;
    }
    stockerRedirections(line, stocker);

    for(int i = 0; i< 10; i++)
    {   
        if(stocker[i] != NULL)
        {
            printf("%s\n", stocker[i]);  
        }
    }


    return 0;
}


//Test ok
void gestionErreur(char* nomFonction)
{
    perror(nomFonction);
}

//Test ok
int ouvertureFichierEnConcatenation(char* nomFichier)
{
    mode_t mode = 0666;
    int fd = open(nomFichier, O_WRONLY | O_CREAT | O_APPEND , mode);
    if(fd == -1){   gestionErreur("open ");}

    return fd;
}


//Test ok
int ouvertureFichierSansEcrasement(char* nomFichier)
{
    mode_t mode = 0666;
    int fd = open(nomFichier, O_WRONLY | O_CREAT | O_EXCL , mode);

    if(fd == -1){   gestionErreur("open ");}

    return fd;
}


//Test ok
int ouvertureFichierAvecEcrasement(char* nomFichier)
{
    mode_t mode = 0666;
    
    int fd = open(nomFichier, O_WRONLY | O_CREAT | O_TRUNC , mode);

    if(fd == -1){   gestionErreur("open ");}

    return fd;
}

//Test ok
int ouvertureFichierStdin(char* nomFichier)
{
    mode_t mode = 0666;
    
    int fd = open(nomFichier, O_RDONLY | O_CREAT, mode);

    if(fd == -1){   gestionErreur("open ");}

    return fd;
}

int ouvertureFichierPipe(char* nomFichier)
{   

}


/*REDIRECTIONS DE LA STDOUT*/
//Test ok
void redirectionsStdoutSansEcrasement(char* nomFichier)
{
    int fd = ouvertureFichierSansEcrasement(nomFichier);
    int retourDup = dup2(fd, STDOUT_FILENO);

    if(retourDup == -1) {gestionErreur("dup2 ");}
}

//Test ok
void redirectionsStdoutAvecEcrasement(char* nomFichier)
{
    int fd = ouvertureFichierAvecEcrasement(nomFichier);
    int retourDup = dup2(fd, STDOUT_FILENO);

    if(retourDup == -1) {gestionErreur("dup2 ");}
}

//Test ok
void redirectionsStdoutEnConcatenation(char* nomFichier)
{
    int fd = ouvertureFichierEnConcatenation(nomFichier);
    int retourDup = dup2(fd, STDOUT_FILENO);

    if(retourDup == -1) {gestionErreur("dup2 ");}
}



/*REDIRECTIONS DE LA STDERR*/
//Test ok
void redirectionsStderrEnConctenation(char* nomFichier)
{
    int fd = ouvertureFichierEnConcatenation(nomFichier);
    int retourDup = dup2(fd, STDERR_FILENO);

    if(retourDup == -1) {gestionErreur("dup2 ");}
}

//Test ok
void redirectionsStderrSansEcrasement(char* nomFichier)
{
    int fd = ouvertureFichierSansEcrasement(nomFichier);
    int retourDup = dup2(fd, STDERR_FILENO);

    if(retourDup == -1) {gestionErreur("dup2 ");}
}

//Test ok
void redirectionsStderrAvecEcrasement(char* nomFichier)
{
    int fd = ouvertureFichierAvecEcrasement(nomFichier);
    int retourDup = dup2(fd, STDERR_FILENO);

    if(retourDup == -1) {gestionErreur("dup2 ");}
}


/*REDIRECTION DE LA STDIN*/
//Test ok
void redirectionsStdin(char* nomFichier)
{
    int fd = ouvertureFichierStdin(nomFichier);
    int retourDup = dup2(fd, STDIN_FILENO);

    if(retourDup == -1) {gestionErreur("dup2 ");}
}


/*
1. Effectuer la redirection
2. Executer la commande à executer.
*/

//Test ok
void redirectionsStdout(char* redirections, char* nomFichier)
{
    int statusSansEcrasement = strcmp(redirections, ">");
    int statusAvecEcrasement = strcmp(redirections, ">|");
    int statusEnConcatenation = strcmp(redirections, ">>");
    
    if(statusSansEcrasement == 0)
    {
        redirectionsStdoutSansEcrasement(nomFichier);
    }
    else
    if(statusAvecEcrasement == 0)
    {
        redirectionsStdoutAvecEcrasement(nomFichier);
    }
    else
    if(statusEnConcatenation == 0)
    {
        redirectionsStdoutEnConcatenation(nomFichier);
    }
    else 
    {
        char* message = "Veuillez entrer le bon signe de redirection vers la stdout\n";
        printf("%s", message);
    }
}


//Test ok
void redirectionsStderr(char* redirections, char* nomFichier)
{
    int statusSansEcrasement = strcmp(redirections, "2>");
    int statusAvecEcrasement = strcmp(redirections, "2>|");
    int statusEnConcatenation = strcmp(redirections, "2>>");
    
    if(statusSansEcrasement == 0)
    {
        redirectionsStderrSansEcrasement(nomFichier);
    }
    else
    if(statusAvecEcrasement == 0)
    {
        redirectionsStderrAvecEcrasement(nomFichier);
    }
    else
    if(statusEnConcatenation == 0)
    {
        redirectionsStderrEnConctenation(nomFichier);
    }
    else 
    {
        char* message = "Veuillez entrer le bon signe de redirection vers la stderr\n";
        printf("%s", message);
    }
}

/*
Return 0, s'il y'a au moins le signe | dans la commande
Return -1 s'il y'a n'y a pas le signe | dans la commande
*/
/*int pipeExiste(char* line)
{

}*/


/*Il y'a autant de nombre de commande à executer que de exec à faire
-Renvoyer le nombre de commande separer par le signe pipe et stocke 
les commandes dans pipeCommande
-S'il y'a une erreur renvoyé -1
*/
/*void compterNombrePipe(int nombrePipeCommande, char* pipeCommande[MAX_ARGS_NUMBER])
{

}*/




//cmd1 | cmd2   et cmd1 | cmd2 | ... | cmdn
/*void redirectionsPipe(int nombreCommandes, char* pipeCommande[MAX_ARGS_NUMBER])
{
    
    int status;
    int retourPipe = pipe();
    if (retourPipe == -1){ gestionErreur("pipe ");}
    pid_t pid1 =fork();
    if (pid1 == -1) { gestionErreur("fork1 redirections pipe "); }
    else if (pid == 0)
    {
        //Fermeture du descpteur qui n'est pas utile
        dup2();
        //Execution de la commande i avec exec
    }
    else 
    {
        //wait(&status); -> Attendre la de l'execution de la commande i
        pid_t pid2 = fork();
        if(pid2 == -1) { gestionErreur("fork2 redirections pipe ");}
        else if (pid2 == 0)
        {
            dup2();
            //Execution de la commande i+1 avec exec
        }
        else 
        {
            //wait()
        }
    }


    for (int i = 0; i < nombreCommandes; i++)
    {

    }
}*/




/*
La redirections des erreurs est prioritaire au autres redirections
*/


/*int verifierRedirectionErreur(char* line)
{

}*/


/*Compte le nombre de redirections dans une chaine de caractère*/
//Test ok
int compterNombreRedirections(char* line)
{
    const int nombreRedirections = 7;

    char haystack[100];
    strcpy(haystack ,line);
    char* needles[] = { " < ", " > ", " >> ", " >| ", " 2> ", " 2>> ", " 2>| "};
    int count = 0;

    for(int i = 0; i<nombreRedirections; i++)
    {
        char *ptr = haystack;
        // Boucle de recherche des occurrences de la chaîne courante
        while ((ptr = strstr(ptr, needles[i])) != NULL) 
        {
            count++;

            // Déplacement du pointeur pour continuer la recherche à partir de la fin de la chaîne trouvée
            ptr += strlen(needles[i]);
        }
    }
    return count;
}

/*
Verifie s'il existe au moins un signe de redirection
*/
//Test ok
int verifierExisteRedirection(char* line)
{
    const int nombreRedirections = 7;

    char haystack[100];
    strcpy(haystack ,line);
    char* needles[] = { " < ", " > ", " >> ", " >| ", " 2> ", " 2>> ", " 2>| "};
    int bool = 0;

    for(int i = 0; i<nombreRedirections; i++)
    {
        char *ptr = haystack;
        // Boucle de recherche des occurrences de la chaîne courante
        if((ptr = strstr(ptr, needles[i])) != NULL) 
        {
            bool = 1;

            // Déplacement du pointeur pour continuer la recherche à partir de la fin de la chaîne trouvée
            ptr += strlen(needles[i]);
        }
    }
    return bool;
}


//Test ok
void* recupererChaineAvantRedirection(char* line, char* sub_string)
{
    char haystack[100];
    strcpy(haystack ,line);
    const int nombreRedirections = 7;
    char* needles[] = {" < ", " > ", " >> ", " >| ", " 2> ", " 2>> ", " 2>| "};

    if(verifierExisteRedirection(line))
    {
        for (int i = 0; i<nombreRedirections; i++)
        {
            char *ptr = haystack;   
            if((ptr = strstr(ptr, needles[i])) != NULL) 
            {
                size_t length = ptr - haystack;
                //char sub_string[length + 1];
                sub_string = malloc(length + 1);
                strncpy(sub_string, haystack, length);
                sub_string[length] = '\0';
                
                return sub_string;
            }
        }  
    }
    return NULL;
}





/*
Par ordre de presence dans line
*/
void stockerRedirections(char* line,char* stocker[MAX_REDIRECTIONS])
{
    if(verifierExisteRedirection(line))
    {
        int nombreRedirections = compterNombreRedirections(line);
        if(nombreRedirections != 0)
        {
            char* chaineApresPremierRedirection = recupererChaineApresRedirection(line);
            if(chaineApresPremierRedirection != NULL)
            {
                char **substrings;
                char str[200];
                strcpy(str, line);
                //strcpy(str, chaineApresPremierRedirection);
                substrings = splitChaine(str, " ");

                for (int i = 0; substrings[i] != NULL; i++) 
                {
                    stocker[i] = substrings[i];
                    printf("Sous-chaîne %d : %s\n", i + 1, substrings[i]);
                }




                free(substrings);
            }
        }
    }
}




//Test ok
char **splitChaine(char *str, const char *delimiter) {
    char **substrings = NULL;
    char *substr = NULL;
    int i = 0;

    substr = strtok(str, delimiter);
    while (substr != NULL) {
        substrings = realloc(substrings, (i + 1) * sizeof(char *));
        substrings[i] = substr;
        i++;
        substr = strtok(NULL, delimiter);
    }

    substrings = realloc(substrings, (i + 1) * sizeof(char *));
    substrings[i] = NULL;

    return substrings;
}



void stockerNomFichiers(char* line, char* stockerNomFichier[MAX_FICHIERS])
{

}


//Test ok
char* recupererChaineApresRedirection(char* line)
{
    char haystack[100];
    strcpy(haystack ,line);
    const int nombreRedirections = 7;
    char* needles[] = {" < ", " > ", " >> ", " >| ", " 2> ", " 2>> ", " 2>| "};

    if(verifierExisteRedirection(line))
    {
        for (int i = 0; i<nombreRedirections; i++)
        {
            char *ptr = haystack;   
            if((ptr = strstr(ptr, needles[i])) != NULL)
            {                   
                return ptr;
            }
        }  
    }
    return NULL;
}