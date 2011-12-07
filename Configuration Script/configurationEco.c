#include "main.h"

void configurationEco()
{
    int i = 1, page = 0;
    char temp[100];
    FILE* test = NULL;
    long wait = 0;

    printf("           -----------------------------------------\n");
    printf("          *                                          *\n");
    printf("          *         Bienvenue dans ce script de      *\n");
    printf("          *      configuration pour import rapide.   *\n");
    printf("          *                                          *\n");
    printf("          *             Ce script est autonome       *\n");
    printf("          *                                          *\n");
    printf("          *      Script conçu pour le Restorator     *\n");
    printf("          *       Contenu dans le kit d'import       *\n");
    printf("          *   Veuillez ne pas distribuer ce script   *\n");
    printf("          *              Code par Taiki              *\n");
    printf("          *         Libéré le 06/12/2011 - 10h       *\n");
    printf("          *                                          *\n");
    printf("           -----------------------------------------\n");

    for(page = 0; i; page++)
    {
        crashTemp(temp, 100);
        sprintf(temp, "%d.jpg", page);
        if(fopen(temp, "r") == NULL)
            i = 0;
    }
    page = page - 2;
    test = fopen("config.dat", "w+");
    fprintf(test, "%d", page);
    fclose(test);

    printf("Création du config.dat pour un import rapide terminé.\nNombre de pages identifiées: %d", page);
    for(wait = 0; wait < 1000000000; wait++);
}
