#include "main.h"

int check(SDL_Surface *ecran, char mangaDispo[MANGA_MAX][30], int dernierChapDispo[MANGA_MAX], int categorie[MANGA_MAX])
{
    FILE* config = NULL;
    int temp2 = 0, tempsEcoule = 0, mangaEnregistres = 0;
    char temp[100] = {0};
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
        return -1;
    }
    SDL_FreeSurface(accueil);
    return 0;
}

int choixSection(SDL_Surface *ecran)
{
    SDL_Surface *accueil = NULL;
    SDL_Event event;
    int selection = 0;

    accueil = IMG_Load("data/selection.jpg");
    SDL_BlitSurface(accueil, NULL, ecran, NULL);
    SDL_Flip(ecran);
    while(selection == 0)
    {
        SDL_WaitEvent(&event);

        switch(event.type)
        {
            case SDL_QUIT:
                SDL_FreeSurface(accueil);
                return -1;
                //appeler ici la fonction de nettoyage
                break;

            case SDL_KEYDOWN:
            {
                switch(event.key.keysym.sym)
                {
                    case SDLK_1:
                    case SDLK_KP1:
                        selection = 1;
                        break;

                    case SDLK_2:
                    case SDLK_KP2:
                        selection = 2;
                        break;

                    case SDLK_3:
                    case SDLK_KP3:
                        selection = 3;
                        break;

                    case SDLK_ESCAPE:
                        return 0;

                    default:
                        break;
                }
                break;
            }
        }
    }
    return selection;
}

int choixManga(SDL_Surface *ecran, SDL_Color couleurAccueil, char mangaDispo[MANGA_MAX][30], int categorie[MANGA_MAX], int sectionChoisie)
{
    SDL_Surface *accueil = NULL, *chapitre1 = NULL, *chapitre2 = NULL, *chapitre3 = NULL, *chapitre4 = NULL, *chapitre5 = NULL, *chapitre6 = NULL, *chapitre7 = NULL, *chapitre8 = NULL, *chapitre9 = NULL;
    SDL_Event event;
    SDL_Rect positionTemp;
    TTF_Font *police = NULL;

    police = TTF_OpenFont("data/Timoteo.ttf", 30);

    int i = 0, temp2 = 0, mangaChoisis = 0, j = 0;
    char temp[100] = {0};

    accueil = IMG_Load("data/manga disponible.jpg");
    for(i = 0; i < MANGA_MAX; i++)
    {
        crashTemp(temp);
        if(categorie[i] == sectionChoisie)
        {
            temp2++;
            for(j = 0; j < 30; j++)
            {
                if(mangaDispo[i][j] == '_')
                    mangaDispo[i][j] = ' ';
            }
            sprintf(temp, "%d. %s", temp2, mangaDispo[i]);
            positionTemp.x = 140;
            switch(temp2) //Savoir a quel manga on en est
            {
                case 1:
                    SDL_BlitSurface(accueil, NULL, ecran, NULL);
                    chapitre1 = TTF_RenderText_Blended(police, temp, couleurAccueil);
                    positionTemp.y = 120;
                    SDL_BlitSurface(chapitre1, NULL, ecran, &positionTemp);
                    break;

                case 2:
                    chapitre2 = TTF_RenderText_Blended(police, temp, couleurAccueil);
                    positionTemp.y = 120 + ((20 + chapitre1->h) * (temp2 - 1));
                    SDL_BlitSurface(chapitre2, NULL, ecran, &positionTemp);
                    break;

                case 3:
                    chapitre3 = TTF_RenderText_Blended(police, temp, couleurAccueil);
                    positionTemp.y = 120 + ((20 + chapitre1->h) * (temp2 - 1));
                    SDL_BlitSurface(chapitre3, NULL, ecran, &positionTemp);
                    break;

                case 4:
                    chapitre4 = TTF_RenderText_Blended(police, temp, couleurAccueil);
                    positionTemp.y = 120 + ((20 + chapitre1->h) * (temp2 - 1));
                    SDL_BlitSurface(chapitre4, NULL, ecran, &positionTemp);
                    break;

                case 5:
                    chapitre5 = TTF_RenderText_Blended(police, temp, couleurAccueil);
                    positionTemp.y = 120 + ((20 + chapitre1->h) * (temp2 - 1));
                    SDL_BlitSurface(chapitre5, NULL, ecran, &positionTemp);
                    break;

                case 6:
                    chapitre6 = TTF_RenderText_Blended(police, temp, couleurAccueil);
                    positionTemp.y = 120 + ((20 + chapitre1->h) * (temp2 - 1));
                    SDL_BlitSurface(chapitre6, NULL, ecran, &positionTemp);
                    break;

                case 7:
                    chapitre7 = TTF_RenderText_Blended(police, temp, couleurAccueil);
                    positionTemp.y = 120 + ((20 + chapitre1->h) * (temp2 - 1));
                    SDL_BlitSurface(chapitre7, NULL, ecran, &positionTemp);
                    break;

                case 8:
                    chapitre8 = TTF_RenderText_Blended(police, temp, couleurAccueil);
                    positionTemp.y = 120 + ((20 + chapitre1->h) * (temp2 - 1));
                    SDL_BlitSurface(chapitre8, NULL, ecran, &positionTemp);
                    break;

                case 9:
                    chapitre9 = TTF_RenderText_Blended(police, temp, couleurAccueil);
                    positionTemp.y = 120 + ((20 + chapitre1->h) * (temp2 - 1));
                    SDL_BlitSurface(chapitre9, NULL, ecran, &positionTemp);
                    break;
            }
            for(j = 0; j < 30; j++)
            {
                if(mangaDispo[i][j] == ' ')
                    mangaDispo[i][j] = '_';
            }
        }
    }
    SDL_Flip(ecran);
    while(mangaChoisis == 0)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                TTF_CloseFont(police);
                SDL_FreeSurface(accueil);
                SDL_FreeSurface(chapitre1);
                SDL_FreeSurface(chapitre2);
                SDL_FreeSurface(chapitre3);
                SDL_FreeSurface(chapitre4);
                SDL_FreeSurface(chapitre5);
                SDL_FreeSurface(chapitre6);
                SDL_FreeSurface(chapitre7);
                SDL_FreeSurface(chapitre8);
                SDL_FreeSurface(chapitre9);
                return -1;
                //appeler ici la fonction de nettoyage
                break;

            case SDL_KEYDOWN:
            {
                switch(event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        TTF_CloseFont(police);
                        SDL_FreeSurface(accueil);
                        SDL_FreeSurface(chapitre1);
                        SDL_FreeSurface(chapitre2);
                        SDL_FreeSurface(chapitre3);
                        SDL_FreeSurface(chapitre4);
                        SDL_FreeSurface(chapitre5);
                        SDL_FreeSurface(chapitre6);
                        SDL_FreeSurface(chapitre7);
                        SDL_FreeSurface(chapitre8);
                        SDL_FreeSurface(chapitre9);
                        return 0;
                        break;

                    case SDLK_1:
                    case SDLK_KP1:
                        if(mangaDispo[0][0] != 0)
                            mangaChoisis = 1;
                        break;

                    case SDLK_2:
                    case SDLK_KP2:
                        if(mangaDispo[1][0] != 0)
                            mangaChoisis = 2;
                        break;

                    case SDLK_3:
                    case SDLK_KP3:
                        if(mangaDispo[2][0] != 0)
                            mangaChoisis = 3;
                        break;

                    case SDLK_4:
                    case SDLK_KP4:
                        if(mangaDispo[3][0] != 0)
                            mangaChoisis = 4;
                        break;

                    case SDLK_5:
                    case SDLK_KP5:
                        if(mangaDispo[4][0] != 0)
                            mangaChoisis = 5;
                        break;

                    case SDLK_6:
                    case SDLK_KP6:
                        if(mangaDispo[5][0] != 0)
                            mangaChoisis = 6;
                        break;

                    case SDLK_7:
                    case SDLK_KP7:
                        if(mangaDispo[6][0] != 0)
                            mangaChoisis = 7;
                        break;

                    case SDLK_8:
                    case SDLK_KP8:
                        if(mangaDispo[7][0] != 0)
                            mangaChoisis = 8;
                        break;

                    case SDLK_9:
                    case SDLK_KP9:
                        if(mangaDispo[8][0] != 0)
                            mangaChoisis = 9;
                        break;

                    default:
                        break;
                }
                break;
            }
        }
    }
    TTF_CloseFont(police);
    SDL_FreeSurface(accueil);
    SDL_FreeSurface(chapitre1);
    SDL_FreeSurface(chapitre2);
    SDL_FreeSurface(chapitre3);
    SDL_FreeSurface(chapitre4);
    SDL_FreeSurface(chapitre5);
    SDL_FreeSurface(chapitre6);
    SDL_FreeSurface(chapitre7);
    SDL_FreeSurface(chapitre8);
    SDL_FreeSurface(chapitre9);
    return mangaChoisis;
}

int choixChapitre(int categorie[MANGA_MAX], int sectionChoisie, int mangaChoisis, int extremesManga[2], char mangaDispo[MANGA_MAX][30], SDL_Color couleurAccueil, SDL_Surface *ecran, int *IDManga)
{
    int temp2 = 0, temp3 = 0, coin = 0, chapitreChoisis = 0;
    SDL_Surface *chapitre1 = NULL, *chapitre2 = NULL, *accueil = NULL;
    SDL_Rect positionTemp;
    TTF_Font *police;
    char temp[100];
    FILE* config = NULL;
    SDL_Event event;

    police = TTF_OpenFont("data/Timoteo.ttf", 30);

    *IDManga = 0;
    while(coin < mangaChoisis)
    {
        if(categorie[*IDManga] == sectionChoisie)
            coin++;

        *IDManga = *IDManga + 1;
    }
    crashTemp(temp);
    sprintf(temp, "manga/%s/config.dat", mangaDispo[*IDManga - 1]);
    config = fopen(temp, "r");
    fscanf(config, "%d %d", &extremesManga[0], &extremesManga[1]);
    fclose(config);
    accueil = IMG_Load("data/chapitre disponible.jpg");
    crashTemp(temp);
    sprintf(temp, "Older chapter: %d -- Newest chapter: %d", extremesManga[0], extremesManga[1]);
    chapitre1 = TTF_RenderText_Blended(police, temp, couleurAccueil);
    crashTemp(temp);
    while (chapitreChoisis == 0)
    {
        SDL_BlitSurface(accueil, NULL, ecran, NULL);
        positionTemp.x = 400 - chapitre1->w / 2;
        positionTemp.y = 250;
        SDL_BlitSurface(chapitre1, NULL, ecran, &positionTemp);
        if (temp2 != -1)
        {
            crashTemp(temp);
            sprintf(temp, "%d", temp2);
            chapitre2 = TTF_RenderText_Blended(police, temp, couleurAccueil);
            temp3 = chapitre2->w;
            positionTemp.x = 400 - (temp3 / 2);
            positionTemp.y = 400;
            SDL_BlitSurface(chapitre2, NULL, ecran, &positionTemp);
        }
        SDL_Flip(ecran);
        SDL_WaitEvent(&event);

        switch(event.type)
        {
            case SDL_QUIT:
                TTF_CloseFont(police);
                SDL_FreeSurface(chapitre1);
                SDL_FreeSurface(chapitre2);
                SDL_FreeSurface(accueil);
                return -1;
                break;

            case SDL_KEYDOWN:
            {
                switch(event.key.keysym.sym)
                {
                    case SDLK_0:
                    case SDLK_KP0:
                        temp2 = temp2 * 10;
                        break;

                    case SDLK_1:
                    case SDLK_KP1:
                        temp2 = temp2 * 10 + 1;
                        break;

                    case SDLK_2:
                    case SDLK_KP2:
                        temp2 = temp2 * 10 + 2;
                        break;

                    case SDLK_3:
                    case SDLK_KP3:
                        temp2 = temp2 * 10 + 3;
                        break;

                    case SDLK_4:
                    case SDLK_KP4:
                        temp2 = temp2 * 10 + 4;
                        break;

                    case SDLK_5:
                    case SDLK_KP5:
                        temp2 = temp2 * 10 + 5;
                        break;

                    case SDLK_6:
                    case SDLK_KP6:
                        temp2 = temp2 * 10 + 6;
                        break;

                    case SDLK_7:
                    case SDLK_KP7:
                        temp2 = temp2 * 10 + 7;
                        break;

                    case SDLK_8:
                    case SDLK_KP8:
                        temp2 = temp2 * 10 + 8;
                        break;

                    case SDLK_9:
                    case SDLK_KP9:
                        temp2 = temp2 * 10 + 9;
                        break;

                    case SDLK_BACKSPACE:
                        temp2 = temp2 / 10;
                        break;

                    case SDLK_RETURN:
                    case SDLK_KP_ENTER:
                        if(temp2 >= extremesManga[0] && temp2 <= extremesManga[1])
                            chapitreChoisis = temp2;
                        break;

                    case SDLK_ESCAPE:
                        TTF_CloseFont(police);
                        SDL_FreeSurface(chapitre1);
                        SDL_FreeSurface(chapitre2);
                        SDL_FreeSurface(accueil);
                        return 0;
                        break;

                    default:
                        break;
                }
                break;
            }
        }
    }
    TTF_CloseFont(police);
    SDL_FreeSurface(chapitre1);
    SDL_FreeSurface(chapitre2);
    SDL_FreeSurface(accueil);
    return chapitreChoisis;
}

