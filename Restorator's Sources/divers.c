#include "main.h"

void crashTemp(char *temp, int longueur)
{
    int i = 0;
    for (i = 0; i < longueur; i++)
    {
        temp[i] = 0;
    }
}

int check(SDL_Surface *ecran, char mangaDispo[MANGA_MAX][30], int dernierChapDispo[MANGA_MAX], int categorie[MANGA_MAX])
{
    FILE* config = NULL;
    FILE* config2 = NULL;
    int temp2 = 0, tempsEcoule = 0, mangaEnregistres = 0, i = 0, j = 0;
    char temp[100] = {0}, superTemp[200] = {0};
    SDL_Surface *accueil = NULL;
    SDL_Event event;

    config = fopen("manga/config.dat", "r"); //On ouvre le fichier de config
    if(config != NULL)//Si ouverture r�ussie
    {
        while(temp2 != 2)//Tant qu'on est pas au bout du fichier
        {
            fscanf(config, "%s", temp);
            if(strcmp(temp, "->") == 0) //Si on est juste avant un nom de manga
            {
                fscanf(config, "%s %d %d", mangaDispo[mangaEnregistres], &dernierChapDispo[mangaEnregistres], &categorie[mangaEnregistres]);
                mangaEnregistres++;
            }
            else if(strcmp(temp, "<-") == 0)
                temp2 = 2;
        }
        fclose(config);
        config = fopen("manga/LTTS/config.dat", "r");
        if(config != 0)
        {
            fclose(config);
            rename("manga/LTTS", "manga/Letter_To_The_Sky");
            config = fopen("manga/config.dat", "r");
            config2 = fopen("manga/config.dat.new", "w+");
            temp2 = 0;
            while(fgetc(config) != EOF)
            {
                fseek(config, -1, SEEK_CUR);
                crashTemp(temp, 100);
                fscanf(config, "%s", temp);
                if(strcmp(temp, "LTTS") != 0)
                    fprintf(config2 ,"%s ", temp);
                else
                    fprintf(config2, "Letter_To_The_Sky ");
                temp2++;
                if(temp2 == 4)
                {
                    fprintf(config2, "\n");
                    temp2 = 0;
                }
            }
            fclose(config);
            fclose(config2);
            remove("manga/config.dat");
            rename("manga/config.dat.new", "manga/config.dat");
        }
    }
    else
    {
        accueil = IMG_Load("data/erreur.jpg");
        SDL_BlitSurface(accueil, NULL, ecran, NULL);
        SDL_Flip(ecran);
        tempsEcoule = SDL_GetTicks();
        while(SDL_GetTicks() - tempsEcoule < 10000)
        {
            SDL_WaitEvent(&event);

            switch(event.type)
            {
                case SDL_QUIT:
                    SDL_FreeSurface(accueil);
                    return -1;
                    break;
            }
        }
        SDL_FreeSurface(accueil);
        exit(EXIT_FAILURE);
    }
    SDL_FreeSurface(accueil);
    config = fopen("update/update.exe", "r");
    if(config != 0)
    {
        crashTemp(temp, 100);
        crashTemp(superTemp, 200);
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
        sprintf(superTemp, "\"%s\\\\update\\\\update.exe\"", temp);
        system(superTemp);
        exit(0);
    }
    return 0;
}

int check4update(SDL_Surface *ecran)
{
    SDL_Surface *chargement = NULL;
    SDL_Rect positionChargement;
    FILE* check = NULL;
    FILE* reference = NULL;
    FILE* config = NULL;
    FILE* config2 = NULL;
    int continuer = 1, reinit = 0, numeroChapitre = 0, classe = 0, extreme[2] = {0}, numeroChapitreTemporaire = 0, i = 0;
    char nomChapitre[30]= {0}, nomMangaComplet[30] = {0}, nomMangaCourt[5] = {0}, temp[100] = {0}, temp2[200] = {0};

    chargement = IMG_Load("data/mise a jour.jpg");
    positionChargement.x = 0;
    positionChargement.y = (HAUTEUR / 2) - (chargement->h / 2);
    SDL_BlitSurface(chargement, NULL, ecran, &positionChargement);
    SDL_Flip(ecran);

    reference = fopen("mangadisponible", "r");
    check = fopen("update/toimport.dat", "r");
    if(check == 0)
    {
        SDL_FreeSurface(chargement);
        return 0;
    }

    else
    {
        while(continuer)
        {
            fscanf(check, "%s %d\n", nomChapitre, &numeroChapitre);
            if(strcmp(nomChapitre, "<-") == 0)
            {
                fclose(check);
                continuer = 0;
            }
            else
            {
                /*On fait la correspondance entre le nom du manga raccourcis et le vrai*/
                while(strcmp(nomMangaCourt, nomChapitre) != 0)
                {
                    for(reinit = 0; reinit < 30; reinit++)
                    {
                        nomMangaComplet[reinit] = 0;
                    }
                    for(reinit = 0; reinit < 5; reinit++)
                    {
                        nomMangaCourt[reinit] = 0;
                    }
                    fscanf(reference, "%s %s %d\n", nomMangaComplet, nomMangaCourt, &classe);

                }
                crashTemp(temp, 100);
                sprintf(temp, "manga/%s/Chapitre_%d/config.dat", nomMangaComplet, numeroChapitre);
                config = fopen(temp, "r");
                if(config == NULL)
                {
                    crashTemp(temp, 100);
                    sprintf(temp, "manga/%s/config.dat", nomMangaComplet);
                    config = fopen(temp, "r");
                    if(config == NULL)
                    {
                        crashTemp(temp, 100);
                        sprintf(temp, "manga/%s", nomMangaComplet);
                        mkdir(temp);
                    }
                    else
                        fclose(config);
                    crashTemp(temp, 100);
                    sprintf(temp, "move update/%s_Chapitre_%d manga/%s", nomChapitre, numeroChapitre, nomMangaComplet);

                    system(temp);
                    crashTemp(temp, 100);
                    sprintf(temp, "manga/%s/%s_Chapitre_%d", nomMangaComplet, nomMangaCourt, numeroChapitre);
                    sprintf(temp2, "manga/%s/Chapitre_%d", nomMangaComplet, numeroChapitre);
                    rename(temp, temp2);
                    config = fopen("manga/config.dat", "r+");
                    config2 = fopen("manga/config.dat.temp", "w+");
                    i = 0;
                    while(strcmp(nomMangaComplet, temp) != 0 && strcmp(temp, "<-") != 0)
                    {
                        fscanf(config, "%s", temp);
                        if(strcmp(temp, "<-") == 0)
                        {
                            fprintf(config2, "-> %s %d %d\n<-", nomMangaComplet, numeroChapitre, classe);
                            for(reinit = 0; reinit < 10; reinit++)
                            {
                                temp[reinit] = 0;
                            }
                            strcpy(temp, "<-");
                        }
                        else
                            fprintf(config2, "%s ", temp);
                        i++;
                        if(i >= 4)
                        {
                            fprintf(config2, "\n");
                            i = 0;
                        }
                    }
                    if(strcmp(temp, "<-") != 0)
                    {
                        fscanf(config, "%d", &numeroChapitreTemporaire);
                        if(numeroChapitreTemporaire < numeroChapitre)
                            fprintf(config2, "%d ", numeroChapitre);
                        else
                            fprintf(config2, "%d ", numeroChapitreTemporaire);
                        i++;
                    }
                    while(strcmp(temp, "<-") != 0)
                    {
                        fscanf(config, "%s", temp);
                        fprintf(config2, "%s ", temp);
                        i++;
                        if(i == 4)
                        {
                            fprintf(config2, "\n");
                            i = 0;
                        }
                    }
                    fclose(config2);
                    fclose(config);
                    remove("manga/config.dat");
                    rename("manga/config.dat.temp", "manga/config.dat");
                    crashTemp(temp, 100);
                    sprintf(temp, "manga/%s/config.dat", nomMangaComplet);
                    config = NULL;
                    config = fopen(temp, "r+");
                    if(config != NULL)
                    {
                        fscanf(config, "%d %d", &extreme[0], &extreme[1]);
                        rewind(config);
                        if(extreme[0] > numeroChapitre)
                            fprintf(config, "%d %d", numeroChapitre, extreme[1]);
                        else if (extreme[1] < numeroChapitre)
                            fprintf(config, "%d %d", extreme[0], numeroChapitre);
                    }
                    else
                    {
                        config = fopen(temp, "w+");
                        fprintf(config, "%d %d", numeroChapitre, numeroChapitre);
                    }
                    fclose(config);
                }
                else
                    fclose(config);

                rewind(reference);
            }
        }
        fclose(reference);
        remove("update/toimport.dat");
    }
    return 0;
}

