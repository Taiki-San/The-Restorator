#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    char syntaxe[40] = {0}, extansion[5] = {0}, syntaxeDoublage[10] = {0};
    int page = 0, i = 0, j = 0, pageDoublee = 0, cmbPDouble = 0, pageDouble[20] = {-1}, numerotation = 1, credit = 0;
    FILE* config = NULL;
    printf("What is the file's syntax?\n-> ");
    scanf("%s", syntaxe);
    printf("What is the extansion (withou dots)\n-> ");
    scanf("%s", extansion);
    printf("How many pages (files)? \n(double pages = one page, credits are a page)\n-> ");
    scanf("%d", &page);
    printf("is it some double pages and if so, how many, if not, type 0.\nType after them position, with lines skipping\n-> ");
    scanf("%d", &cmbPDouble);
    if(cmbPDouble != 0 && cmbPDouble < 20)
    {
        printf("\n");
        for(i = 0; i < cmbPDouble; i++)
        {
            printf("-> ");
            scanf("%d", &pageDouble[i]);
        }
        printf("What is the syntax who link the two pages? (in place of '_')\n%s&page_numer_one&_____&page_number_2&.%s\n-> ", syntaxe, extansion);
        scanf("%s", syntaxeDoublage);
    }
    printf("Does credits to be included inside the numbering?\n0: Include or at the end\n1: No-include and on the first position.\nWARNING, IF YOU TYPE 1, YOU'LL NEED WRITE THE FILE'S NAME MANUALY\n->");
    scanf("%d", &credit);
    config = fopen("config.dat", "w+");
    fprintf(config, "%d\n", page);
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

