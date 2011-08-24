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
        printf("Bonjour, veuillez deplacer tous les dossier decompresse contenant les\nchapitres dans le repertoire 'Update' qui est dans le repertoire du Restorator.\nLe repertoire est ecrit dans la fenetre noir qui apparait au lancement.\nMettez aussi cet programme dans le dossier 'Update'\n\nEntrez un chiffre et 'enter' pour fermer le programme.\n-> ");
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
            printf("Base de données des manga existant a été mise à jour.\n\n");
        }
        printf("Bonjour et bievenue dans ce script de mise a jour.\n\nVerifiez que les dossier contenant les chapitres (1 dossier/chapitre) sont bien directement dans le dossier 'Update'.\nUne fois que c'est fais, entrez le ou les numero correspondant aux mangas \ndont vous ajoutez des chapitres.\n\nEntrez 0 quand vous avez fini et sautez une ligne entre chaque manga.\n\n");
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
            printf("\n%s\nSi tous les chapitre que vous ajoutez se suivent, faites 1, sinon 0.\nFaites ensuite 'enter'\n->", manga[temp2]);
            scanf("%s", k);
            if(k[0] == '1')
            {
                printf("Entrez le numero du premier chapitre.\n-> ");
                scanf("%d", &j);
                printf("Entrez le dernier chapitre.\n-> ");
                scanf("%d", &coin);
                while(j <= coin)
                {
                    fprintf(fichier, "%s %d\n", raccourcis[temp2], j);
                    j++;
                }
            }
            else
            {
                printf("Entrez les numero de chapitres un par un en faisant 'enter' entre chaques.\nQuand vous avez finis, tapez 0 puis 'enter'");
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
