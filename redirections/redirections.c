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






int main(int argc, char** argv)
{
    redirectionsStdin(argv[1]);
    
    int r =execlp("cat", "cat", NULL);
    if(r==-1)
    {
        perror("execl");
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



//cmd1 | cmd2   et cmd1 | cmd2 | ... | cmdn
void redirectionsPipe(char* monFichier)
{

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
