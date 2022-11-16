#include <stdlib.h>
#include <stdio.h> 
#include "pwd/pwd.h"

int main(int argc, char **argv){

    //Traitement de la commande entrée avec la fonction readline

    //TRAITEMENT DE LA COMMANDE pwd
    if (argc<1) return 1;
    else
    {
        if (strcmp(argv[0], "pwd")==0)
        {
            //Cas d'un lien physique
            if (strcmp(argv[1], "-P") == 0) 
            {
                pwdForP();
            }
            //Cas par defaut, cas d'un lien logique
            else if (strcmp(argv[1], "-L") == 0)
            {
                pwdForL();
            }
            else if (argv[1] == NULL)
            {
                pwdForL();
            }
        }
        else 
        {
            perror("strcmp error ");
            printf("Veuillez entrer la commande : pwd\n");
            return 1;
        }
    }

}
