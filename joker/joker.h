#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>
#include "../constant.h"

int joker(int argc, char** argv);


/*
 * prend une chaine de caractère et renvoie la position de la première étoile si il y en a une
 * et renvoi -1 sinon
*/
int is_etoile_simple(char* argv);


/*
 * prend une chaine de caractère et renvoie la position du premier slash si il y en a un
 * et renvoi -1 sinon
*/
int next_slash(char* arg);

/*
 * prend une chaine de caractère et renvoie la position du dernier slash si il y en a un
 * et renvoi -1 sinon
*/
int last_slash(char* arg);

/*
* prend un pointeur sur une chaine de caractère en argument et revoie un pointeur sur cette chaine de
* caractères en enlevant tous les sash en trop ("a//b" -> "a/b" ; "a////////b" -> "a/b")
*/
char *  del_double_slash(char* arg);

/*
* Renvoie 1 si le fichier correspondant à arg existe et 0 sinon.
*/
int check_file_exist(char* arg);

/*
* argv: la list des arguments
* arg: une copie de l'argument partiellement transformé ou pas
* place: la place de la première étoile dans arg
* num_arg: la place de l'argument dans arv
* la fonction renvoie le nombre d'arguments ajoutés
* recherche récursivement des fichiers correspondant à arg en trouvant tous les dossiers ou 
* fichiers qui peuvent remplacer les étoiles
* quand un fichier est trouvé il est ajouté dans la list des arguments (argv) en lui
* allouant avec malloc() un espace
*/
int cherche_prefixe(char** argv, char* arg, int place, int num_arg);

/*
* argv: la list des arguments
* arg: une copie de l'argument partiellement transformé ou pas
* num_arg: la place de l'argument dans arv
* preffixTmp: doit être initialisé à "" au premier lancement et s'actualise seul
* recherche récursivement tous les dossiers corespondant à la forme *\/*\/...*\/*\/...
* quand il y a le préfixe ** dans arg
*/
int recherche_recursive(char** argv, char* arg, int num_arg, char* preffixTmp);