#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    char syntaxe[40] = {0}, extansion[5] = {0}, syntaxeDoublage[10] = {0};
    int page = 0, i = 0, j = 0, pageDoublee = 0, cmbPDouble = 0, pageDouble[20] = {-1}, numerotation = 1, credit = 0;
    FILE* config = NULL;
    printf("Quelle est la syntaxe des fichiers?\n-> ");
    scanf("%s", syntaxe);
    printf("Quelle est l'extansion (sans point)?\n-> ");
    scanf("%s", extansion);
    printf("Combien y a t'il de pages (fichiers)? \n(pages doubles = une page, la page des credits compte comme une)\n-> ");
    scanf("%d", &page);
    printf("Y a t-il des pages doubles et si oui, combien, sinon, tapez 0.\nTapez ensuite leur position, en sautant un ligne\n-> ");
    scanf("%d", &cmbPDouble);
    if(cmbPDouble != 0 && cmbPDouble < 20)
    {
        printf("\n");
        for(i = 0; i < cmbPDouble; i++)
        {
            printf("-> ");
            scanf("%d", &pageDouble[i]);
        }
        printf("Quelle est la syntaxe liant les deux pages? (juste a la place du '_')\n%s&numero_de_page_n 1&_____&numero_de_page_n 2&.%s\n-> ", syntaxe, extansion);
        scanf("%s", syntaxeDoublage);
    }
    printf("La page de credit est-elle inclue dans la numerotation?\n0: Inclue dans la numerotation ou a la fin\n1: Non inclut dans la numerotation & en premiere position.\nATTENTION, SI VOUS FAITES 1, VOUS DEVREZ METTRE LE NOM DU FICHIER MANUELLEMENT\n->");
    scanf("%d", &credit);
    config = fopen("config.dat", "w+");
    fprintf(config, "     \n");
    for(i = 1; i <= page; i++)
    {
        pageDoublee = 0;
        if(i == 1 && credit == 1)
        {
            fprintf(config, "%d %s00\n", i, syntaxe);
        }
        else
        {
                for(j = 0; j < 20; j++)
            {
                if(pageDouble[j] == i)

                    {
                        if(numerotation < 10)
                            fprintf(config, "%d %s0%d%s%d.%s\n", i, syntaxe, numerotation, syntaxeDoublage, numerotation + 1, extansion);
                        else
                            fprintf(config, "%d %s%d%s%d.%s\n", i, syntaxe, numerotation, syntaxeDoublage, numerotation + 1, extansion);

                        numerotation++;
                        pageDoublee++;
                    }
            }
            if(j == 20 && pageDoublee == 0)
            {
                if(numerotation < 10)
                    fprintf(config, "%d %s0%d.%s\n", i, syntaxe, numerotation, extansion);

                else
                    fprintf(config, "%d %s%d.%s\n", i, syntaxe, numerotation, extansion);
            }
            numerotation++;
        }

    }
    rewind(config);
    fprintf(config, "%d", i - 1);
    fclose(config);
    return 0;
}

