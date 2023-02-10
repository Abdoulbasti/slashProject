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
#include <errno.h>
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

void ajouterFd(int fd);
int FDs[10] = {0};




int main(int argc, char** argv)
{
    char* line = "ls -al < texte >> out 2> err";
    char* commande = appliquerRedirections(line);
    printf("redirection vers la sortie standard de : %s\n", commande);
    
    execlp("ls", "ls", "-al", NULL);
    //execlp("cat", "cat", "-ddgjgf", NULL);
    

    return 0;
}

//Test ok
void laGestionErreur(char* nomFonction)
{
    perror(nomFonction);
}

//Test ok
int ouvertureFichierEnConcatenation(char* nomFichier)
{
    mode_t mode = 0666;
    int fd = open(nomFichier, O_WRONLY | O_CREAT | O_APPEND , mode);
    if(fd == -1){   laGestionErreur("open ");}

    return fd;
}


//Test ok
int ouvertureFichierSansEcrasement(char* nomFichier)
{
    mode_t mode = 0666;
    int fd = open(nomFichier, O_WRONLY | O_CREAT | O_EXCL , mode);

    if(fd == -1){   laGestionErreur("open ");}

    return fd;
}


//Test ok
int ouvertureFichierAvecEcrasement(char* nomFichier)
{
    mode_t mode = 0666;
    
    int fd = open(nomFichier, O_WRONLY | O_CREAT | O_TRUNC , mode);

    if(fd == -1){   laGestionErreur("open ");}

    return fd;
}

//Test ok
int ouvertureFichierStdin(char* nomFichier)
{
    mode_t mode = 0666;
    
    int fd = open(nomFichier, O_RDONLY | O_CREAT, mode);

    if(fd == -1){   laGestionErreur("open ");}

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

    if(retourDup == -1) {laGestionErreur("dup2 ");}
}

//Test ok
void redirectionsStdoutAvecEcrasement(char* nomFichier)
{
    int fd = ouvertureFichierAvecEcrasement(nomFichier);
    int retourDup = dup2(fd, STDOUT_FILENO);

    if(retourDup == -1) {laGestionErreur("dup2 ");}
}

//Test ok
void redirectionsStdoutEnConcatenation(char* nomFichier)
{
    int fd = ouvertureFichierEnConcatenation(nomFichier);
    int retourDup = dup2(fd, STDOUT_FILENO);

    if(retourDup == -1) {laGestionErreur("dup2 ");}
}

/*REDIRECTIONS DE LA STDERR*/
//Test ok
void redirectionsStderrEnConctenation(char* nomFichier)
{
    int fd = ouvertureFichierEnConcatenation(nomFichier);
    int retourDup = dup2(fd, STDERR_FILENO);

    if(retourDup == -1) {laGestionErreur("dup2 ");}
}

//Test ok
void redirectionsStderrSansEcrasement(char* nomFichier)
{
    int fd = ouvertureFichierSansEcrasement(nomFichier);
    int retourDup = dup2(fd, STDERR_FILENO);

    if(retourDup == -1) {laGestionErreur("dup2 ");}
}

//Test ok
void redirectionsStderrAvecEcrasement(char* nomFichier)
{
    int fd = ouvertureFichierAvecEcrasement(nomFichier);
    int retourDup = dup2(fd, STDERR_FILENO);

    if(retourDup == -1) {laGestionErreur("dup2 ");}
}


/*REDIRECTION DE LA STDIN*/
//Test ok
void redirectionsStdin(char* redirection, char* nomFichier)
{
    int statusSansEcrasement = strcmp(redirection, "<");
    if(statusSansEcrasement == 0)
    {
        int fd = ouvertureFichierStdin(nomFichier);
        int retourDup = dup2(fd, STDIN_FILENO);

        if(retourDup == -1) {laGestionErreur("dup2 ");}   
    }
    else 
    {
        printf("Veuillez passer < pour la redirections du stdin\n");
    }
}


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
Appliquer tous les redirections sauf les pipes
Appliquer une redirection en fonction du cas*/
//Test ok
void redirections(char* redirection, char* nomFichier)
{
    if(estRedirectionStdin(redirection))
    {
        redirectionsStdin(redirection, nomFichier);
    }

    if(estRedirectionStdout(redirection))
    {
        redirectionsStdout(redirection, nomFichier);
    }

    if(estRedirectionStderr(redirection))
    {
        redirectionsStderr(redirection, nomFichier);
    }
}

//Test ok
int estRedirectionStdin(char* redirection)
{
    if(strcmp(redirection, "<") == 0)
    {
        return 1;
    }
    return 0;
}

//test ok
int estRedirectionStdout(char* redirection)
{
    if( 
        strcmp(redirection, ">")    == 0    || 
        strcmp(redirection, ">>")   == 0    ||
        strcmp(redirection, ">|")   == 0        
        )
    {
        return 1;
    }
    return 0;
}

//Test ok
int estRedirectionStderr(char* redirection)
{
    if( 
        strcmp(redirection, "2>")    == 0    || 
        strcmp(redirection, "2>>")   == 0    ||
        strcmp(redirection, "2>|")   == 0        
        )
    {
        return 1;
    }
    return 0;
}


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


/*Cette fonction recupere la commande à executer*/
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




/*Verifie si le format de redirection recuperer est correcte ou pas.
Le format de redirection est le suivant : red1 fichier1 red2 fichier2 red3 fichier3 ... redN fichierN
*/

//Test ok
int verifierFormatRedirection(char* chaineAPartirPremiereRedirection)
{
    //Si la taille des redirections est paire 
    if(verifierTailleFormatRedirection(chaineAPartirPremiereRedirection))
    {
        if(redirectionOrdonee(chaineAPartirPremiereRedirection))
        {
            printf("Le format de la redirection est correct\n");
            return 1;
        }
        else 
        {
            printf("Au moins un fichier ou redirection n'est pas au bon emplacement,\
            donner un redirections correcte svp\n");
            return 0;
        }
    }
    else 
    {
        printf("Les commandes de redirections doivent être paire\n");
        return 0;
    }
}

/*Cette fonction verifie si la les redirections sont au bon format*/
//Test ok
int redirectionOrdonee(char* chaineAPartirPremiereRedirection)
{
    int boolRedirections = 1;
    int boolFichiers = 1;

    char **substrings;
    char str[200];
    strcpy(str, chaineAPartirPremiereRedirection);
    substrings = splitChaine(str, " ");

    int i = 0;
    while(substrings[i] != NULL)
    {   
        if(entierPaire(i))
        {
            //Si ce qui est à l'indice i est un est une redirection
            if(
                strcmp(substrings[i],   "<")  ==0    ||
                strcmp(substrings[i],   ">")  ==0    ||
                strcmp(substrings[i],   ">>") ==0    ||
                strcmp(substrings[i],   ">|") ==0    ||
                strcmp(substrings[i],   "2>") ==0    ||
                strcmp(substrings[i],   "2>>")==0    ||
                strcmp(substrings[i],   "2>|")==0
            )
            //" < ", " > ", " >> ", " >| ", " 2> ", " 2>> ", " 2>| "
            {
                //QLQ CHOSE...
                boolRedirections = boolRedirections && 1;
                i++;
            }
            else 
            {   
                boolRedirections = boolRedirections && 0;
                i++;
            }
        }
        else 
        {
            if(
                strcmp(substrings[i],   "<")  !=0    &&
                strcmp(substrings[i],   ">")  !=0    &&
                strcmp(substrings[i],   ">>") !=0    &&
                strcmp(substrings[i],   ">|") !=0    &&
                strcmp(substrings[i],   "2>") !=0    &&
                strcmp(substrings[i],   "2>>")!=0    &&
                strcmp(substrings[i],   "2>|")!=0
            )
            {
                boolFichiers = boolFichiers && 1;
                i++;
            }
            else 
            {
                boolFichiers = boolFichiers && 0;
                i++;
            }
        }   
    }
    return boolRedirections && boolFichiers;
}

/*Cet format de redirection doit être paire*/
//Test ok
int verifierTailleFormatRedirection(char* chaineAPartirPremiereRedirection)
{
    char **substrings;
    char str[200];
    strcpy(str, chaineAPartirPremiereRedirection);
    substrings = splitChaine(str, " ");

    int i = 0;
    while(substrings[i] != NULL)
    {    i++;   }
    
    if(entierPaire(i))
    {   return 1;   }
    else
    {   return 0;   }
}

/*
Recupère la une ligne de commande contenant au moins une redirections,

*/
//Test ok
void stockerRedirectionsEtFichiers(char* line, char* tabRedirections[MAX_REDIRECTIONS], char* tabFichier[MAX_FICHIERS])
{
    if(verifierExisteRedirection(line))
    {
        int nombreRedirections = compterNombreRedirections(line);
        if(nombreRedirections != 0)
        {
            //char* chaineApresPremierRedirection = recupererChaineApresRedirection(line);
            char* chaineAvantRedirection;
            chaineAvantRedirection = recupererChaineAvantRedirection(line, chaineAvantRedirection);
            char* chaineApresPremierRedirection =  recupererChaineApresRedirection(chaineAvantRedirection, line);
            if(chaineApresPremierRedirection != NULL)
            {
                //Verifier le format de redirection

                char **substrings;
                char str[200];
                strcpy(str, line);
                //strcpy(str, chaineApresPremierRedirection);
                substrings = splitChaine(str, " ");
                int indexTab = 0;

                for (int n =0; n< MAX_REDIRECTIONS; n++)
                {
                    int tailleMax = 100;
                    tabRedirections[n] = (char *) malloc(tailleMax* sizeof(char));
                    tabFichier[n] = (char* ) malloc(tailleMax* sizeof(char));
                }

                int compteur1 = 0;
                for (int i = 0; substrings[i] != NULL; i ++) 
                {
                    if (entierPaire(i))
                    {
                        strcpy(tabRedirections[compteur1], substrings[i]);
                        compteur1++;
                    }
                }

                int compteur2 = 0;
                for (int j = 1; substrings[j] != NULL; j ++)
                {
                    if (!entierPaire(j))
                    {
                        strcpy(tabFichier[compteur2], substrings[j]);
                        compteur2++;
                    }
                }
                free(substrings);
            }
        }
    }
}


/*Verifie si un entier est paire ou non*/
//Test ok
int entierPaire(int i)
{
    if ( (i % 2) == 0)
        return 1;
    else
        return 0;
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


/*Recuperer la chaine à partir de la toute premier signe de redirection*/
//Test ok
char* recupererChaineApresRedirection(char* chaineAvantRedirection, char* line)
{
    size_t tailleCommande = strlen(chaineAvantRedirection);
    size_t tailleRedirection = strlen(line) - tailleCommande;
    char* redirections = calloc(120, sizeof(char));

    strncpy(redirections, line +tailleCommande, tailleRedirection);

    *(redirections + tailleRedirection) = '\0';

    return redirections;
}


//Test ok
void initialiserTabRF(char* tabRedirections[MAX_REDIRECTIONS], char* tabFichier[MAX_FICHIERS])
{
    for(int i = 0; i<MAX_FICHIERS; i++)
    {
        tabRedirections[i] = NULL;
        tabFichier[i] = NULL;
    }
}

/*
Cette fonction applique les redirections sur les fichiers, et revois la commande qui doit être exécuter
*/
//Test ok

char* appliquerRedirections(char* line)
{
    char* commande;
    commande = recupererChaineAvantRedirection(line, commande);
    char* redirectionsLine = recupererChaineApresRedirection(commande, line);

    if(commande == NULL)
    {   printf("La commande n'a pas été recuperer correctement\n"); return NULL; }

    //ON APPLIQUE LES REDIRECTIONS SUR LES FICHIERS
    if(verifierFormatRedirection(redirectionsLine))
    {
        int nRedirections = compterNombreRedirections(redirectionsLine);
        char* tabRedirections[MAX_REDIRECTIONS];    char* tabFichier[MAX_FICHIERS];
        initialiserTabRF(tabRedirections, tabFichier);
        
        stockerRedirectionsEtFichiers(redirectionsLine, tabRedirections, tabFichier);
        for(int i = 0; i<nRedirections; i++)
        {
            redirections(tabRedirections[i], tabFichier[i]);
        }

        //Liberer les ressources
        for(int n = 0; n<MAX_REDIRECTIONS; n++)
        {
            free(tabRedirections[n]);
            free(tabFichier[n]);
        }
        //free(redirectionsLine);

        return commande;
    }
    /*else 
    {
        //free(redirectionsLine);
    }*/

    commande = NULL;
    return commande;
}
