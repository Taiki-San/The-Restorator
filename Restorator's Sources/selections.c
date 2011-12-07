#include "main.h"

int choixSection(SDL_Surface *ecran)
{
    SDL_Surface *accueil = NULL;
    SDL_Event event;
    SDL_Rect position;
    int selection = 0;
    position.x = 0;

    accueil = IMG_Load("data/selection.jpg");
    position.y = (HAUTEUR / 2) - (accueil->h / 2);
    SDL_BlitSurface(accueil, NULL, ecran, &position);
    SDL_Flip(ecran);
    while(selection == 0)
    {
        SDL_WaitEvent(&event);

        switch(event.type)
        {
            case SDL_QUIT:
                SDL_FreeSurface(accueil);
                return -1;
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

                    case SDLK_4:
                    case SDLK_KP4:
                        selection = 4;
                        break;

                    case SDLK_5:
                    case SDLK_KP5:
                        selection = 5;
                        break;

                    case SDLK_ESCAPE:
                        SDL_FreeSurface(accueil);
                        return 0;

                    default:
                        break;
                }
                break;
            }
        }
    }
    SDL_FreeSurface(accueil);
    return selection;
}

int choixManga(SDL_Surface *ecran, SDL_Color couleurAccueil, char mangaDispo[MANGA_MAX][30], int categorie[MANGA_MAX], int sectionChoisie)
{
    SDL_Surface *accueil = NULL, *chapitre[MANGAPARPAGE] = {NULL}, *numerotation = NULL, *cache = NULL;
    SDL_Event event;
    SDL_Rect positionTemp;
    TTF_Font *police = NULL;

    police = TTF_OpenFont("data/Timoteo.ttf", 30);

    cache = SDL_CreateRGBSurface(SDL_HWSURFACE, LARGEUR, HAUTEUR, 32, 0, 0 , 0, 0); //on initialise le fond
    SDL_FillRect(cache, NULL, SDL_MapRGB(ecran->format, 248, 205, 126)); //We change background color

    int i = 0, temp2 = 0, mangaChoisis = 0, j = 0, k = 0, pageTotale = 0, pageSelection = 1, affichageTeste = 1, nombreMangaValides = 0, exit = 1;
    int presentation = 0, buffer = 0, coincoin = 0;
    char temp[100] = {0};

    if(sectionChoisie == 4) //Si cette fonction est appelé par le module de MaJ
        accueil = IMG_Load("data/telechargement.jpg");

    else //Si par le lecteur
        accueil = IMG_Load("data/manga disponible.jpg");

    for(k = 0; k < 30; k++)
    {
        for(i = 0; i < MANGA_MAX; i++)
        {
            if(mangaDispo[i][k] == '_')
            mangaDispo[i][k] = ' ';
        }
    }
    SDL_BlitSurface(accueil, NULL, ecran, NULL);

    for(i = 0; mangaDispo[i][0] != 0; i++)
    {
        if(categorie[i] == sectionChoisie)
            nombreMangaValides++;
    }
    if(nombreMangaValides*(LARGEURTEXTE + INTERLIGNE) + BORDURESUPSELECTION <= ecran->h)
    {
        presentation = 1;
        TTF_CloseFont(police);
        police = TTF_OpenFont("data/Timoteo.ttf", 30);
        for(i = 0; mangaDispo[i][0] != 0; i++)
        {
            crashTemp(temp, 100);
            if(categorie[i] == sectionChoisie)
            {
                temp2++;
                sprintf(temp, "%d. %s", temp2, mangaDispo[i]);
                positionTemp.x = 140;
                if(temp2 == 1) //Savoir a quel manga on en est
                {
                    chapitre[temp2 - 1] = TTF_RenderText_Blended(police, temp, couleurAccueil);
                    positionTemp.y = BORDURESUPSELECTION;
                    SDL_BlitSurface(chapitre[temp2 - 1] , NULL, ecran, &positionTemp);
                }

                else if (temp2 > 1)
                {
                    chapitre[temp2 - 1]  = TTF_RenderText_Blended(police, temp, couleurAccueil);
                    positionTemp.y = BORDURESUPSELECTION + ((20 + chapitre[0] ->h) * (temp2 - 1));
                    SDL_BlitSurface(chapitre[temp2 - 1] , NULL, ecran, &positionTemp);
                }
            }
        }
        coincoin = temp2;
    }
    else if (nombreMangaValides*(LARGEURTEXTE + INTERLIGNE) + BORDURESUPSELECTION > ecran->h)
    {
        presentation = 2;
        if(affichageTeste)
        {
            /*Multi page necessaire?
            Il faudra rajouter du code mais pas urgent
            pour faire le numero de page*/
            if (nombreMangaValides > MANGAPARPAGE)
            {
                pageTotale = nombreMangaValides / MANGAPARPAGE;
                if(nombreMangaValides % MANGAPARPAGE > 0)
                    pageTotale++;
                pageSelection = 1;
                nombreMangaValides = (pageSelection - 1) * MANGAPARPAGE;
            }
            else
                nombreMangaValides = 0;
        }
        j = 0;
        for(i = (pageSelection - 1) * MANGAPARPAGE; (i < (pageSelection * MANGAPARPAGE) && mangaDispo[i][0] != 0); i++)
        {
            crashTemp(temp, 100);
            if(categorie[i] == sectionChoisie)
            {
                j++;
                TTF_CloseFont(police);
                police = TTF_OpenFont("data/Timoteo.ttf", 20);
                if(i < pageSelection * MANGAPARPAGE)
                {
                    sprintf(temp, "%d. %s", j, mangaDispo[i]);
                    chapitre[j - 1] = TTF_RenderText_Blended(police, temp, couleurAccueil);
                    positionTemp.x = BORDURELATSELECTION + ((j - 1) / (MANGAPARPAGE / NBRCOLONNES) * (LONGUEURTEXTE + BORDURECOLONNE));
                    if(j % (MANGAPARPAGE / NBRCOLONNES) == 1)
                        positionTemp.y = BORDURESUPSELECTION;
                    else
                        positionTemp.y = BORDURESUPSELECTION + ((chapitre[j - 1]->h + MINIINTERLIGNE) * ((j - 1) % (MANGAPARPAGE / NBRCOLONNES)));
                    SDL_BlitSurface(chapitre[j - 1] , NULL, ecran, &positionTemp);
                }
            }
        }
        coincoin = j;
    }
    for(k = 0; k < MANGA_MAX; k++)
    {
        for(i = 0; i < 30; i++)
        {
            if(mangaDispo[i][k] == ' ')
            mangaDispo[i][k] = '_';
        }
    }
    SDL_Flip(ecran);
    TTF_CloseFont(police);
    police = TTF_OpenFont("data/Timoteo.ttf", 30);
    buffer = -1;
    while(exit)
    {
        exit = 1;
        if(presentation == 2 && buffer != mangaChoisis)
        {
            /*Bliter une page vierge*/
            crashTemp(temp, 100);
            sprintf(temp, "%d", mangaChoisis);
            numerotation = TTF_RenderText_Blended(police, temp, couleurAccueil);
            positionTemp.x = 0;
            positionTemp.y = HAUTEURNUMEROTATION;
            SDL_BlitSurface(cache, NULL, ecran, &positionTemp);
            positionTemp.x = (ecran->w / 2) - (numerotation->w / 2);
            SDL_BlitSurface(numerotation, NULL, ecran, &positionTemp);
            SDL_Flip(ecran);
            buffer = mangaChoisis;
        }
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                TTF_CloseFont(police);
                SDL_FreeSurface(accueil);
                SDL_FreeSurface(numerotation);
                SDL_FreeSurface(cache);
                for(i = 0; i < MANGAPARPAGE; i++)
                {
                    SDL_FreeSurface(chapitre[i]);
                }
                return -1;
                break;

            case SDL_KEYDOWN:
            {
                switch(event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        TTF_CloseFont(police);
                        SDL_FreeSurface(accueil);
                        for(i = 0; i < MANGAPARPAGE; i++)
                        {
                            SDL_FreeSurface(chapitre[i]);
                        }
                        return 0;
                        break;

                    case SDLK_0:
                    case SDLK_KP0:
                        if(mangaChoisis * 10 <= coincoin)
                            mangaChoisis = mangaChoisis * 10;
                        break;


                    case SDLK_1:
                    case SDLK_KP1:
                        if(mangaChoisis * 10 + 1 <= coincoin)
                            mangaChoisis = mangaChoisis * 10 + 1;

                        break;

                    case SDLK_2:
                    case SDLK_KP2:
                        if(mangaChoisis * 10 + 2 <= coincoin)
                            mangaChoisis = mangaChoisis * 10 + 2;
                        break;

                    case SDLK_3:
                    case SDLK_KP3:
                        if(mangaChoisis * 10 + 3 <= coincoin)
                            mangaChoisis = mangaChoisis * 10 + 3;

                        break;

                    case SDLK_4:
                    case SDLK_KP4:
                        if(mangaChoisis * 10 + 4 <= coincoin)
                            mangaChoisis = mangaChoisis * 10 + 4;
                        break;

                    case SDLK_5:
                    case SDLK_KP5:
                        if(mangaChoisis * 10 + 5 <= coincoin)
                            mangaChoisis = mangaChoisis * 10 + 5;
                        break;

                    case SDLK_6:
                    case SDLK_KP6:
                        if(mangaChoisis * 10 + 6 <= coincoin)
                            mangaChoisis = mangaChoisis * 10 + 6;
                        break;

                    case SDLK_7:
                    case SDLK_KP7:
                        if(mangaChoisis * 10 + 7 <= coincoin)
                            mangaChoisis = mangaChoisis * 10 + 7;
                        break;

                    case SDLK_8:
                    case SDLK_KP8:
                        if(mangaChoisis * 10 + 8 <= coincoin)
                            mangaChoisis = mangaChoisis * 10 + 8;
                        break;

                    case SDLK_9:
                    case SDLK_KP9:
                        if(mangaChoisis * 10 + 9 <= coincoin)
                            mangaChoisis = mangaChoisis * 10 + 9;
                        break;

                    case SDLK_BACKSPACE:
                        mangaChoisis = mangaChoisis / 10;
                        break;

                    case SDLK_RETURN:
                    case SDLK_KP_ENTER:
                        if(mangaChoisis != 0)
                            exit = 0;
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
    SDL_FreeSurface(numerotation);
    SDL_FreeSurface(cache);
    for(i = 0; i < MANGAPARPAGE; i++)
    {
        SDL_FreeSurface(chapitre[i]);
    }
    return mangaChoisis;
}

int choixChapitre(int categorie[MANGA_MAX], int sectionChoisie, int mangaChoisis, int extremesManga[2], char mangaDispo[MANGA_MAX][30], SDL_Color couleurAccueil, SDL_Surface *ecran, int *IDManga)
{
    int temp2 = 0, temp3 = 0, coin = 0, chapitreChoisis = -3;
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
    crashTemp(temp, 100);
    if(sectionChoisie != 4)
    {
        sprintf(temp, "manga/%s/config.dat", mangaDispo[*IDManga - 1]);
        config = fopen(temp, "r");
        fscanf(config, "%d %d", &extremesManga[0], &extremesManga[1]);
        fclose(config);
    }
    if(sectionChoisie == 4)
        accueil = IMG_Load("data/chapitre disponible DL.jpg");
    else
        accueil = IMG_Load("data/chapitre disponible.jpg");
    crashTemp(temp, 100);
    #ifdef FRENCH
    sprintf(temp, "Plus ancien: %d -- Plus recent: %d", extremesManga[0], extremesManga[1]);
    #endif
    #ifdef ENGLISH
    sprintf(temp, "Older: %d -- Last: %d", extremesManga[0], extremesManga[1]);
    #endif
    chapitre1 = TTF_RenderText_Blended(police, temp, couleurAccueil);
    crashTemp(temp, 100);
    while (chapitreChoisis == -3)
    {
        SDL_BlitSurface(accueil, NULL, ecran, NULL);
        positionTemp.x = 400 - chapitre1->w / 2;
        positionTemp.y = 250;
        SDL_BlitSurface(chapitre1, NULL, ecran, &positionTemp);
        if (temp2 != -1)
        {
            crashTemp(temp, 100);
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
                        if(temp2 >= extremesManga[0] && temp2 <= extremesManga[1] && sectionChoisie != 4)
                            chapitreChoisis = temp2;

                        else if(((temp2 >= extremesManga[0] && temp2 <= extremesManga[1])|| temp2 == 0) && sectionChoisie == 4)
                            chapitreChoisis = temp2;
                        break;

                    case SDLK_ESCAPE:
                        TTF_CloseFont(police);
                        SDL_FreeSurface(chapitre1);
                        SDL_FreeSurface(chapitre2);
                        SDL_FreeSurface(accueil);
                        return -2;
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

