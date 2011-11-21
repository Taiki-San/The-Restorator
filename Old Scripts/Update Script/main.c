#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE* test = NULL;
    FILE* fichier = NULL;
    char temp[10] = {0}, manga[20][50], k[5] = {0}, raccourcis[20][5];
    int i = 0, j = 0, coin = 0, temp2 = 0, mangaStatus[20] = {0}, mangaAjoutes[20] = {0};

    for(i = 0; i < 20; i++)
    {
        for(j = 0; j < 50; j++)
        {
            manga[i][j] = 0;
        }

        for(j = 0; j < 5; j++)
        {
            raccourcis[i][j] = 0;
            k[j] = 0;
        }
    }

    test = fopen("../The Restorator.exe", "r");
    if(test == NULL)
    {
        printf("Hello, please move all uncompressed folders who contains chapters configured\non the 'Update' folder who is in the Restorator folder.\nThis programm need to run in this folder too\n\nType a number and enter for close this script.\n-> ");
        scanf("%s", temp);
        return 0;
    }
    else
    {
        fclose(test);
        test = fopen("mangadisponible", "r");
        if(test != NULL)
        {
            fclose(test);
            remove("../mangadisponible");
            system("move mangadisponible ../");
            printf("Know manga's database up to date.\n\n");
        }
        printf("Hello and welcome in this updating script.\n\nCheck the folders who contains chapters (1 folder/chapter) are directly on the 'Update' folder.\nWhen it's finish, type the numbers or the numbers corresponding to mangas\nwhose you want add chapters\n\nType 0 when you've finish and skip a line beetwen each manga.\n\n");
        test = fopen("../mangadisponible", "r");
        for(i = 0; i < 20; i++)
        {
            for(j = 0; j < 50; j++)
            {
                manga[i][j] = 0;
            }
            fscanf(test, "%s %s %d ", manga[i], raccourcis[i], &mangaStatus[i]);

            for(j = 0; j < 50; j++)
            {
                if(manga[i][j] == '_')
                    manga[i][j] = ' ';
            }

            if (manga[i][0] != 0)
            {
                printf("%d. %s\n", i + 1, manga[i]);
            }
            else
            {
                i = 20;
            }
        }
        for(i = 0; i < 20; i++)
        {
            scanf("%d", &mangaAjoutes[i]);
            if(mangaAjoutes[i] == 0)
                i = 21;
        }
        fichier = fopen("toimport.dat", "w+");
        for(i = 0; mangaAjoutes[i] != 0; i++)
        {
            temp2 = mangaAjoutes[i] - 1;
            printf("\n%s\nIf all the chapters you want to add follow, type 1, otherwise 0.\nAfter, type 'enter'\n->", manga[temp2]);
            scanf("%s", k);
            if(k[0] == '1')
            {
                printf("Type the first chapter number.\n-> ");
                scanf("%d", &j);
                printf("Type the las chapter number.\n-> ");
                scanf("%d", &coin);
                while(j <= coin)
                {
                    fprintf(fichier, "%s %d\n", raccourcis[temp2], j);
                    j++;
                }
            }
            else
            {
                printf("Type chapters number one per one and type 'enter' between each.\nWhen you've finish, type 0 and 'enter'");
                j = -1;
                while(j != 0)
                {
                    printf("\n-> ");
                    scanf("%d", &j);
                    if(j != 0)
                        fprintf(fichier, "%s %d\n", raccourcis[temp2], j);
                }
            }
        }
    }
    fprintf(fichier, "<-");
    fclose(fichier);
    return 0;
}


