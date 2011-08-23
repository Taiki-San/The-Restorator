#include "main.h"

void crashTemp(char temp[100])
{
    int i = 0;
    for (i = 0; i < 100; i++)
    {
        temp[i] = 0;
    }
}

void miseEnPause()
{
    int restart = 1;
    SDL_Event event;
    while(restart)
    {
        SDL_WaitEvent(&event);
        if ((event.active.state & SDL_APPACTIVE) == SDL_APPACTIVE)
        {
            if (event.active.gain == 0) /* La fenêtre a été réduite en barre des tâches */
                restart = 1;
            else if (event.active.gain == 1) /* La fenêtre a été restaurée */
                restart = 0;
        }

        if ((event.active.state & SDL_APPINPUTFOCUS) == SDL_APPINPUTFOCUS)
        {
            if (event.active.gain == 0) /* La fen^tre a quitté le premier plan */
                restart = 1;
            else if (event.active.gain == 1) /* La fenêtre a retrouvé le premier plan */
                restart = 0;
        }


    }
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
    char nomChapitre[30]= {0}, nomMangaComplet[30] = {0}, nomMangaCourt[5] = {0}, temp[200] = {0}, temp2[200] = {0};

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
                        nomMangaComplet[reinit] = 0;
                    }
                    fscanf(reference, "%s %s %d\n", nomMangaComplet, nomMangaCourt, &classe);
                }
                sprintf(temp, "manga/%s/Chapitre_%d/config.dat", nomMangaComplet, numeroChapitre);
                config = fopen(temp, "r");
                if(config == NULL)
                {
                    sprintf(temp, "manga/%s/config.dat", nomMangaComplet);
                    config = fopen(temp, "r");
                    if(config == NULL)
                    {
                        sprintf(temp, "manga/%s", nomMangaComplet);
                        mkdir(temp);
                    }
                    else
                        fclose(config);

                    sprintf(temp, "move update/%s_Chapitre_%d manga/%s", nomChapitre, numeroChapitre, nomMangaComplet);
                    system(temp);
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
                    i++;
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
            }
        }
        fclose(reference);
        remove("update/toimport.dat");
    }
    return 0;
}
