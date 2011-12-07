#include "main.h"

int main()
{
    /* Au début du programme, on récupère l'encodage
     * utilisé par la console. */
    UINT default_codepage = GetConsoleOutputCP();

    /* Puis, on change cet encodage pour imposer celui
     * que l'on veut (ici, page de code 1252). */
    SetConsoleOutputCP(1252);

    int retour = 0;
    printf(" -----------------------------------------\n");
    printf("*                                          *\n");
    printf("*     Bienvenue dans ce script d'import    *\n");
    printf("*        Veuillez choisir quel script      *\n");
    printf("*           vous voulez utiliser           *\n");
    printf("*                                          *\n");
    printf("*           Afficher les accents:          *\n");
    printf("*  Clic droit->Par défaut->Lucide Console  *\n");
    printf("*        Ok puis relancer le script        *\n");
    printf("*                                          *\n");
    printf("*      Script conçu pour le Restorator     *\n");
    printf("*       Contenu dans le kit d'import       *\n");
    printf("*   Veuillez ne pas distribuer ce script   *\n");
    printf("*              Code par Taiki              *\n");
    printf("*         Libéré le 6/12/2011 - 14h        *\n");
    printf("*                                          *\n");
    printf(" ------------------------------------------\n\n\n\n");

    printf(" ------------------------------------------------\n");
    printf("**            Veuillez choisir un script:        **\n");
    printf("**     1. Création d'un config.dat standard      **\n");
    printf("**  2. Création d'un config.dat d'import massif  **\n");
    printf("**         3. Vérification du config.dat         **\n");
    printf("**                                               **\n");
    printf("**              Enter pour confirmer             **\n");
    printf(" ------------------------------------------------\n");

    while(retour < 1 || retour > 3)
    {
        printf("\n\n-> ");
        retour = 0;
        retour = getchar();
    }

    #ifdef WINDOWS
    system("cls");
    #endif

    switch(retour)
    {
        case 1:
            configurationStandard();
            break;

        case 2:
            configurationEco();
            break;

        case 3:
            checkOnly();
            break;
    }

    /* A la fin du programme, on restaure l'encodage initial. */
    SetConsoleOutputCP(default_codepage);
    return 0;
}


void crashTemp(char *temp, int longueur)
{
    int i = 0;
    for(i = 0; i < longueur; i++)
        temp[i] = 0;
}

