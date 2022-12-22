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



//cmd > fic
void redirectionStdoutSansEcrasement(char* nomFichier);

//cmd >| fic
void redirectionStdoutAvecEcrasement(char* nomFichier);

//cmd >> fic
void redirectionStdoutEnConcatenation(char* nomFichier);



//cmd 2>> fic
void redirectionStderrEnConctenation(char* nomFichier);

//cmd 2> fic
void redirectionStderrSansEcrasement(char* nomFichier);

//cmd 2>| fic
void redirectionStderrAvecEcrasement(char* nomFichier);





int ouvertureFichierEnConcatenation(char* nomFichier);

int ouvertureFichierAvecEcrasement(char* nomFichier);

int ouvertureFichierSansEcrasement(char* nomFichier);



//cmd < fic
void redirectionsStdin(int fd);

//cmd1 | cmd2   et cmd1 | cmd2 | ... | cmdn
void redirectionsPipe(char* monFichier);

//void redirectionStdout(char* commandesIntvoid redirectionStdout(char* commandesInternesExternes[MAX_ARGS_NUMBER], char* redirection, char* nomFichier);ernesExternes[MAX_ARGS_NUMBER], char* redirection, char* nomFichier);

void redirectionStdout(char* redirection, char* nomFichier);

void redirectionStderr(char* redirection, char* nomFichier);


int main(int argc, char** argv)
{

    redirectionStderr("2>>", "fic1.txt");
    char* buff = "Ceci est un autre message3\n";
    write(2, buff, strlen(buff));

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


/*REDIRECTIONS DE LA STDOUT*/
//Test ok
void redirectionStdoutSansEcrasement(char* nomFichier)
{
    int fd = ouvertureFichierSansEcrasement(nomFichier);
    int retourDup = dup2(fd, STDOUT_FILENO);

    if(retourDup == -1) {gestionErreur("dup2 ");}
}

//Test ok
void redirectionStdoutAvecEcrasement(char* nomFichier)
{
    int fd = ouvertureFichierAvecEcrasement(nomFichier);
    int retourDup = dup2(fd, STDOUT_FILENO);

    if(retourDup == -1) {gestionErreur("dup2 ");}
}

//Test ok
void redirectionStdoutEnConcatenation(char* nomFichier)
{
    int fd = ouvertureFichierEnConcatenation(nomFichier);
    int retourDup = dup2(fd, STDOUT_FILENO);

    if(retourDup == -1) {gestionErreur("dup2 ");}
}



/*REDIRECTIONS DE LA STDERR*/
//Test ok
void redirectionStderrEnConctenation(char* nomFichier)
{
    int fd = ouvertureFichierEnConcatenation(nomFichier);
    int retourDup = dup2(fd, STDERR_FILENO);

    if(retourDup == -1) {gestionErreur("dup2 ");}
}

//Test ok
void redirectionStderrSansEcrasement(char* nomFichier)
{
    int fd = ouvertureFichierSansEcrasement(nomFichier);
    int retourDup = dup2(fd, STDERR_FILENO);

    if(retourDup == -1) {gestionErreur("dup2 ");}
}

//Test ok
void redirectionStderrAvecEcrasement(char* nomFichier)
{
    int fd = ouvertureFichierAvecEcrasement(nomFichier);
    int retourDup = dup2(fd, STDERR_FILENO);

    if(retourDup == -1) {gestionErreur("dup2 ");}
}



/*
1. Effectuer la redirection
2. Executer la commande à executer.
*/

//Test ok
void redirectionStdout(char* redirections, char* nomFichier)
{
    int statusSansEcrasement = strcmp(redirections, ">");
    int statusAvecEcrasement = strcmp(redirections, ">|");
    int statusEnConcatenation = strcmp(redirections, ">>");
    
    if(statusSansEcrasement == 0)
    {
        redirectionStdoutSansEcrasement(nomFichier);
    }
    else
    if(statusAvecEcrasement == 0)
    {
        redirectionStdoutAvecEcrasement(nomFichier);
    }
    else
    if(statusEnConcatenation == 0)
    {
        redirectionStdoutEnConcatenation(nomFichier);
    }
    else 
    {
        char* message = "Veuillez entrer le bon signe de redirection vers la stdout\n";
        printf("%s", message);
    }
}


//Test ok
void redirectionStderr(char* redirections, char* nomFichier)
{
    int statusSansEcrasement = strcmp(redirections, "2>");
    int statusAvecEcrasement = strcmp(redirections, "2>|");
    int statusEnConcatenation = strcmp(redirections, "2>>");
    
    if(statusSansEcrasement == 0)
    {
        redirectionStderrSansEcrasement(nomFichier);
    }
    else
    if(statusAvecEcrasement == 0)
    {
        redirectionStderrAvecEcrasement(nomFichier);
    }
    else
    if(statusEnConcatenation == 0)
    {
        redirectionStderrEnConctenation(nomFichier);
    }
    else 
    {
        char* message = "Veuillez entrer le bon signe de redirection vers la stderr\n";
        printf("%s", message);
    }
}