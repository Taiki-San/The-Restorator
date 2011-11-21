#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

void crashTemp(char *temp, int longueur)
{
    int i = 0;
    for(i = 0; i < longueur; i++)
        temp[i] = 0;
}

int main()
{
    int i = 0, j = 0, page = 0, nombrePageDouble = 0, pageDouble[50] = {0}, debutNumerotation = 0, compteur = 0, nombreDeBoulettes = 0;
    char temp[250], syntaxe[100], extansion[5], syntaxeCredit[100], extansionCredit[5], erreur[300][100], numeration[100];
    char superTemp[300], REPERTOIREEXECUTION[250];
    FILE *test = NULL;
    printf("         --------------------------------------\n");
    printf("        |                                      |\n");
    printf("        |   Bienvenue dans ce script d'import  |\n");
    printf("        |   Veuillez appuyer sur enter apres   |\n");
    printf("        |   chaque choix. Pour la syntaxe des  |\n");
    printf("        |   pages, l'espace est remplace par & |\n");
    printf("        |                                      |\n");
    printf("        |   Script concu pour les Restorator   |\n");
    printf("        |     Contenu dans le kit d'import     |\n");
    printf("        | Veuillez ne pas distribuer ce script |\n");
    printf("        |            Code par Taiki            |\n");
    printf("        |                                      |\n");
    printf("         --------------------------------------\n");
    crashTemp(REPERTOIREEXECUTION, 250);
    GetCurrentDirectory(sizeof(REPERTOIREEXECUTION),REPERTOIREEXECUTION);
    printf("Quelle est la syntaxe des fichiers?\n-> ");
    crashTemp(syntaxe, 100);
    scanf("%s", syntaxe);
    for(i = 0; i < 100; i++)
    {
        if(syntaxe[i] == ' ')
            syntaxe[i] = '&';
    }
    printf("Quelle est l'extansion (sans point)?\n-> ");
    crashTemp(extansion, 5);
    scanf("%s", extansion);
    printf("Combien y a t'il de pages (fichiers)? \n(pages doubles = une page, la page des credits compte comme une)\n-> ");
    scanf("%d", &page);
    printf("Y a-t-il des pages doubles et si oui, combien, sinon, tapez 0.\n-> ");
    scanf("%d", &nombrePageDouble);
    if(nombrePageDouble != 0 && nombrePageDouble < 20)
    {
        printf("\nVeuillez en entrer les position en faisant enter entre chaque\n");
        for(i = 0; i < nombrePageDouble; i++)
        {
            printf("-> ");
            scanf("%d", &pageDouble[i]);
        }
    }
    crashTemp(temp, 250);
    sprintf(temp, "%s00.%s", syntaxe, extansion);
    if(fopen(temp, "r") == 0)
    {
        printf("\nLa page de credit est-elle en premier position et hors de la numerotation?(o/n)\n->");
        crashTemp(extansionCredit, 5);
        crashTemp(syntaxeCredit, 100);
        crashTemp(numeration, 100);
        scanf("%s", extansionCredit);
        if(extansionCredit[0] == 'o' || extansionCredit[0] == 'O')
        {
            printf("\nQuels sont les syntaxe/format de la page de credit?\n");
            printf("Nom du fichier (sans extension, 0 si standard).\n-> ");
            scanf("%s", syntaxeCredit);
            if(syntaxeCredit[0] == '0')
            {
                printf("Quelle est la \"numerotation\" de la page de credit?\n-> ");
                scanf("%s", numeration);
            }
            else
            {
                for(i = 0; i < 100; i++)
                {
                    if(syntaxeCredit[i] == ' ')
                        syntaxeCredit[i] = '&';
                }
            }
            printf("Format (0 si le meme)\n-> ");
            crashTemp(extansionCredit, 5);
            scanf("%s", extansionCredit);
        }
        else
        {
            crashTemp(extansionCredit, 5);
        }
    }
    crashTemp(temp, 250);
    sprintf(temp, "%s01.%s", syntaxe, extansion);
    if(fopen(temp, "r") == 0)
    {
        printf("\nA combien commence la numerotation\nLa page de credit est inclus sauf si decouple du chapitre\n-> ");
        scanf("%d", &debutNumerotation);
    }
    test = fopen("config.dat", "w+");
    fprintf(test, "%d\nN\n", page); //On laisse de la place pour Žcrire le numero plus tard
    compteur = debutNumerotation;
    for(i = 1; i <= page; i++)
    {
        if(i == 1)
        {
            if(extansionCredit[0] == '0' && syntaxeCredit[0] == '0')
            {
                fprintf(test, "1 %s%s.%s\n", syntaxe, numeration, extansion);
            }
            else if(syntaxeCredit[0] == '0')
            {
                fprintf(test, "1 %s%s.%s\n", syntaxe, numeration, extansionCredit);
            }
            else if(extansionCredit[0] == '0')
            {
                fprintf(test, "1 %s.%s\n", syntaxeCredit, extansion);
            }
            else
            {
                fprintf(test, "1 %s.%s\n", syntaxeCredit, extansionCredit);
            }
        }
        else
        {
            for(j = 0; j < page && pageDouble[j] != i; j++);
            /*Si c'est une page double*/
            if(pageDouble[j] == i)
            {
                if(compteur < 8)
                    fprintf(test, "%d %s0%d-0%d.%s\n", i, syntaxe, compteur, compteur + 1, extansion);

                else if(compteur < 9)
                    fprintf(test, "%d %s0%d-%d.%s\n", i, syntaxe, compteur, compteur + 1, extansion);

                else
                    fprintf(test, "%d %s%d-%d.%s\n", i, syntaxe, compteur, compteur + 1, extansion);
                compteur++;
            }
            else
            {
                if(compteur < 9)
                    fprintf(test, "%d %s0%d.%s\n", i, syntaxe, compteur, extansion);

                else
                    fprintf(test, "%d %s%d.%s\n", i, syntaxe, compteur, extansion);
            }
            compteur++;
        }
    }
    /*On fais un check*/
    fclose(test);
    nombreDeBoulettes = 0;
    for(i = 0; i < 300; i++)
    {
        for(j = 0; j < 100; j++)
        {
            erreur[i][j] = 0;
        }
    }
    do
    {
        test = fopen("config.dat", "r");
        fscanf(test, "%d\nN\n", &page);
        nombreDeBoulettes = 0;
        for(i = 0; i < page; i++)
        {
            crashTemp(temp, 250);
            fscanf(test, "%d %s\n", &i, temp);
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
            printf("Il semblerait que des erreurs sont presentes dans le fichier.\n");
            printf("Nombre d'erreur detectees: %d\nErreurs aux pages:\n", nombreDeBoulettes);
            for(i = 0; i < nombreDeBoulettes - 1; i++)
            {
                printf("- %s\n", erreur[i]);
            }
            printf("Le script reprendra lorsque vous aurez ferme le bloc note apres correction.\n");

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
            sprintf(superTemp, "notepad \\\"%s\\\\config.dat\\\"", temp);
            system(superTemp);
        }
    } while(nombreDeBoulettes != 0);
    return 0;
}

