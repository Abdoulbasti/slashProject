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


void laGestionErreur(char* nomFonction);

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




int estRedirectionStderr(char* redirection);
int estRedirectionStdout(char* redirection);
int estRedirectionStdin(char* redirection);

//cmd1 | cmd2   et cmd1 | cmd2 | ... | cmdn
void redirectionsPipe(char* monFichier);

//void redirectionStdout(char* commandesIntvoid redirectionStdout(char* commandesInternesExternes[MAX_ARGS_NUMBER], char* redirection, char* nomFichier);ernesExternes[MAX_ARGS_NUMBER], char* redirection, char* nomFichier);

//cmd < fic
void redirectionsStdin(char* redirection, char* nomFichier);
void redirectionsStdout(char* redirection, char* nomFichier);
void redirectionsStderr(char* redirection, char* nomFichier);
void redirections(char* redirection, char* nomFichier);




int compterNombreRedirections(char* line);

int verifierExisteRedirection(char* line);

void* recupererChaineAvantRedirection(char* line, char* sub_string);

char* recupererChaineApresRedirection(char* chaineAvantRedirection, char* line);

void stockerRedirectionsEtFichiers(char* line,char* tabRedirections[MAX_REDIRECTIONS], char* tabFichier[MAX_FICHIERS]);

int compterNombreFichiers(char* line);

char** splitChaine(char *str, const char *delimiter);

int entierPaire(int i);

int verifierFormatRedirection(char* chaineAPartirPremiereRedirection);

int verifierTailleFormatRedirection(char* chaineAPartirPremiereRedirection);


int redirectionOrdonee(char* chaineAPartirPremiereRedirection);

void initialiserTabRF(char* tabRedirections[MAX_REDIRECTIONS], char* tabFichier[MAX_FICHIERS]);

void executerCommande(char* line);

char* appliquerRedirections(char* line);