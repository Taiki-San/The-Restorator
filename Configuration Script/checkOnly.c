#include "main.h"

void checkOnly()
{
    int nombreDeBoulettes = 0, i = 0, j = 0, page = 0;
    char erreur[300][100], temp[100], superTemp[300], REPERTOIREEXECUTION[250];
    FILE *test = NULL;
    long wait = 0;
    for(i = 0; i < 300; i++)
    {
        for(j = 0; j < 100; j++)
        {
            erreur[i][j] = 0;
        }
    }

    crashTemp(REPERTOIREEXECUTION, 250);
    GetCurrentDirectory(sizeof(REPERTOIREEXECUTION),REPERTOIREEXECUTION);

    printf("           -----------------------------------------\n");
    printf("          *                                          *\n");
    printf("          *         Bienvenue dans ce script de      *\n");
    printf("          *    vérification Des fichiers config.dat  *\n");
    printf("          *     Ce script se fermera seul quand le   *\n");
    printf("          *         fichier sera correct, si ce      *\n");
    printf("          *  script détecte un problème, le chapitre *\n");
    printf("          *             NE PEUT PAS MARCHER!         *\n");
    printf("          *                                          *\n");
    printf("          *      Script conçu pour le Restorator     *\n");
    printf("          *       Contenu dans le kit d'import       *\n");
    printf("          *   Veuillez ne pas distribuer ce script   *\n");
    printf("          *              Code par Taiki              *\n");
    printf("          *         Libéré le 06/12/2011 - 10h       *\n");
    printf("          *                                          *\n");
    printf("           -----------------------------------------\n");

    do
    {
        test = fopen("config.dat", "r");
        fscanf(test, "%d\n", &page);
        if(fgetc(test) == 'N')
        {
            fgetc(test); //On saute le \n
            nombreDeBoulettes = 0;
            for(i = 0; i < page; i++)
            {
                crashTemp(temp, 250);
                fscanf(test, "%d %s\n", &i, temp);

                /*Conversion*/
                for(j = 0; j < 100; j++)
                {
                    if(temp[j] == '&')
                        temp[j] = ' ';
                }

                /*Si erreur*/
                if(fopen(temp, "r") == 0)
                {
                    strcpy(erreur[nombreDeBoulettes], temp);
                    nombreDeBoulettes++;
                }
            }
            fclose(test);
            if(nombreDeBoulettes != 0)
            {
                printf("Il semblerait que des erreurs sont présentes dans le fichier.\n");
                printf("Nombre d'erreur detectees: %d\nErreurs aux pages:\n", nombreDeBoulettes);
                for(i = 0; i < nombreDeBoulettes + 1; i++)
                {
                    printf("- %s\n", erreur[i]);
                }
                printf("Le script reprendra lorsque vous aurez fermé le bloc-note après correction.\n");

                /*Code a adapter*/
                crashTemp(temp, 250);
                crashTemp(superTemp, 300);
                j = 0;
                for(i = 0; REPERTOIREEXECUTION[i] != 0; i++)
                {
                    if(REPERTOIREEXECUTION[i] == '\\')
                    {
                        temp[j] = '\\';
                        j++;
                    }
                    temp[j] = REPERTOIREEXECUTION[i];
                    j++;
                }
                sprintf(superTemp, "notepad %s\\\\config.dat", temp);
                system(superTemp);
            }
            else
                printf("Aucun problème détecté\n");
        }

        else if(fgetc(test) == EOF)
        {
            for(i = 0; i < page; i++)
            {
                crashTemp(temp, 100);
                sprintf(temp, "%d.jpg", i);
                if(fopen(temp, "r") == NULL)
                {
                    nombreDeBoulettes = i;
                    i = page + 10;
                }
            }
            if(i > page + 5)
            {
                printf("Erreur detectée: arrèt à la page %d.jpg\nCorrection du config.dat\n", nombreDeBoulettes);
                fclose(test);
                test = fopen("config.dat", "w+");
                fprintf(test, "%d", nombreDeBoulettes);
                fclose(test);
                printf("Correction terminée\n");
                nombreDeBoulettes = 0;
            }
            else
                printf("\n\nAucun problème détecté.");
        }
    } while(nombreDeBoulettes != 0);

    for(wait = 0; wait < 1000000000; wait++);
}
