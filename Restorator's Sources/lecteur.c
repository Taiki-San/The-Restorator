#include "main.h"

int lecteur(int *chapitreChoisis, char mangaDispo[MANGA_MAX][30], int IDManga, SDL_Surface *ecran, int extremesManga[2], SDL_Color couleurAccueil)
{
    int j = 0, pageEnCoursDeLecture = 0, check4change = 0, changementPage = 0;
    char temp[100] = {0};
    FILE* config = NULL;
    FILE* testExistance = NULL;
    struct page pageActuelle;
    SDL_Rect positionTemp, positionPage, positionAffichage, positionInfos, positionAnimation, positionBordure;
    SDL_Surface *bandeauSup = NULL, *bandeauInf = NULL, *fond = NULL, *positionChap = NULL, *bandeauLat = NULL;
    SDL_Surface *chapitre = NULL;
    SDL_Event event;
    TTF_Font *police = NULL;

    police = TTF_OpenFont("data/Timoteo.ttf", 20);

    bandeauLat = SDL_CreateRGBSurface(SDL_HWSURFACE, BORDURE_VERTICALE, HAUTEUR, 32, 0, 0 , 0, 0);
    SDL_FillRect(bandeauLat, NULL, SDL_MapRGB(ecran->format, 248, 205, 126));
    positionBordure.y = 0;

    positionAffichage.y = BORDURE_HORIZONTALE;
    positionAnimation.y = 0;
    positionTemp.x = 0;
    positionPage.h = 0;

    for (j = 0; j < 50; j++)
    {
        pageActuelle.nomPage[j] = 0;
    }
    sprintf(temp, "manga/%s/Chapitre_%d/config.dat", mangaDispo[IDManga - 1], *chapitreChoisis);
    config = fopen(temp, "r");
    if(config == NULL)
    {
        if(*chapitreChoisis < extremesManga[1])
            *chapitreChoisis = *chapitreChoisis + 1;

        else
            *chapitreChoisis = -2;

        return 0;
    }
    fscanf(config, "%d", &pageActuelle.pageTotal);
    pageEnCoursDeLecture = 1;
    pageActuelle.numeroPage = 0;
    bandeauInf = IMG_Load("data/bandeau.jpg");
    while(pageActuelle.pageTotal >= pageActuelle.numeroPage)
    {
        while(pageActuelle.numeroPage != pageEnCoursDeLecture)
        {
            if(pageActuelle.pageTotal == pageActuelle.numeroPage)
            {
                rewind(config);
                fscanf(config, "%d", &pageActuelle.pageTotal);
            }

            fscanf(config, "%d %s %d %d", &pageActuelle.numeroPage, pageActuelle.nomPage, &pageActuelle.largeurP, &pageActuelle.hauteurP);
            positionPage.h = HAUTEUR - 100 - BORDURE_HORIZONTALE;
            positionPage.x = 0;
            positionPage.y = 0;
        }

        for(j = 0; j < 50; j++)
        {
            if(pageActuelle.nomPage[j] == '&')
                pageActuelle.nomPage[j] = ' ';
        }
        sprintf(temp, "manga/%s/Chapitre_%d/%s", mangaDispo[IDManga - 1], *chapitreChoisis, pageActuelle.nomPage);
        SDL_FreeSurface(chapitre);
        SDL_Surface *chapitre = NULL;
        testExistance = fopen(temp, "r");
        if(testExistance == NULL) //If the page don't exist
            chapitre = IMG_Load("data/erreur load.jpg");
        else
        {
            fclose(testExistance);
            chapitre = IMG_Load(temp);
        }
        positionPage.w = chapitre->w;
        if((pageActuelle.largeurP + 40) > 800)
        {
            SDL_FreeSurface(fond);
            SDL_FreeSurface(ecran);
	        ecran = SDL_SetVideoMode((pageActuelle.largeurP + 40), HAUTEUR, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
            fond = SDL_CreateRGBSurface(SDL_HWSURFACE, (pageActuelle.largeurP + (BORDURE_VERTICALE * 2)), HAUTEUR, 32, 0, 0 , 0, 0);
            bandeauSup = SDL_CreateRGBSurface(SDL_HWSURFACE, (pageActuelle.largeurP + (BORDURE_VERTICALE * 2)), BORDURE_HORIZONTALE, 32, 0, 0 , 0, 0);
            SDL_FillRect(bandeauSup, NULL, SDL_MapRGB(ecran->format, 248, 205, 126));
            SDL_FillRect(fond, NULL, SDL_MapRGB(ecran->format, 248, 205, 126));
            SDL_BlitSurface(bandeauSup, NULL, ecran, NULL);
            for (j = 0; j < 5; j++)
            {
                positionTemp.y = HAUTEUR - (j * BORDURE_HORIZONTALE);
                SDL_BlitSurface(bandeauSup, NULL, ecran, &positionTemp);
            }
        }
        else
        {
            SDL_FreeSurface(fond);
            SDL_FreeSurface(ecran);
            ecran = SDL_SetVideoMode(800, HAUTEUR, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
            fond = SDL_CreateRGBSurface(SDL_HWSURFACE, 800, HAUTEUR, 32, 0, 0 , 0, 0);
            bandeauSup = SDL_CreateRGBSurface(SDL_HWSURFACE, ecran->w, BORDURE_HORIZONTALE, 32, 0, 0 , 0, 0);
            SDL_FillRect(bandeauSup, NULL, SDL_MapRGB(ecran->format, 248, 205, 126));
            SDL_FillRect(fond, NULL, SDL_MapRGB(ecran->format, 248, 205, 126));
            SDL_BlitSurface(bandeauSup, NULL, ecran, NULL);
            positionTemp.x = 0;
            for (j = 0; j < 5; j++)
            {
                positionTemp.y = HAUTEUR - (j * BORDURE_HORIZONTALE);
                SDL_BlitSurface(bandeauSup, NULL, ecran, &positionTemp);
            }
        }

        SDL_BlitSurface(fond, NULL, ecran, NULL);
        SDL_BlitSurface(bandeauSup, NULL, ecran, NULL);
        positionAffichage.x = (ecran->w / 2) - (chapitre->w / 2);
        #ifdef FRENCH
        sprintf(temp, "Manga: %s - Chapitre: %d - Page %d/%d", mangaDispo[IDManga - 1], *chapitreChoisis, pageActuelle.numeroPage, pageActuelle.pageTotal);
        #endif
        #ifdef ENGLISH
        sprintf(temp, "Manga: %s - Chapter: %d - Page %d/%d", mangaDispo[IDManga - 1], *chapitreChoisis, pageActuelle.numeroPage, pageActuelle.pageTotal);
        #endif
        positionChap = TTF_RenderText_Blended(police, temp, couleurAccueil);
        positionInfos.x = (ecran->w / 2) - (positionChap->w / 2);
        positionInfos.y = 5;
        SDL_BlitSurface(positionChap, NULL, ecran, &positionInfos);

        if(changementPage == 1 || changementPage == -1) // Si on change de page
        {
            positionAnimation.y = 0;
            positionTemp.x = (ecran->w / 2) - (bandeauInf->w / 2);
            positionTemp.y = HAUTEUR - 100;
            SDL_BlitSurface(bandeauInf, NULL, ecran, &positionTemp);
            SDL_Flip(ecran);
            if(changementPage == 1) //Si on va a la page suivante
            {
                positionBordure.x = ecran->w - bandeauLat->w;
                for(j = 0; (j + 1) * DEPLACEMENT_ANIMATION < chapitre->w; j++)
                {
                    positionAnimation.y = BORDURE_HORIZONTALE;
                    positionAnimation.x = chapitre->w - (DEPLACEMENT_ANIMATION * j);
                    SDL_BlitSurface(chapitre, &positionPage, ecran, &positionAnimation);
                    SDL_BlitSurface(bandeauLat, NULL, ecran, &positionBordure);
                    SDL_Flip(ecran);
                }
            }
            else //Si on va a la page précédente
            {
                positionBordure.x = 0;
                for(j = (chapitre->w / DEPLACEMENT_ANIMATION) - 1; j > 1; j--)
                {
                    positionAnimation.x = DEPLACEMENT_ANIMATION * j;
                    SDL_BlitSurface(chapitre, &positionAnimation, ecran, &positionAffichage);
                    SDL_BlitSurface(bandeauLat, NULL, ecran, &positionBordure);
                    SDL_Flip(ecran);
                }
            }
        }
        changementPage = 0;
        SDL_BlitSurface(fond, NULL, ecran, NULL);
        SDL_BlitSurface(chapitre, &positionPage, ecran, &positionAffichage);
        SDL_BlitSurface(bandeauSup, NULL, ecran, NULL);
        positionTemp.x = (ecran->w / 2) - (bandeauInf->w / 2);
        positionTemp.y = HAUTEUR - 100;
        SDL_BlitSurface(bandeauInf, NULL, ecran, &positionTemp);
        crashTemp(temp, 100);
        SDL_BlitSurface(positionChap, NULL, ecran, &positionInfos);
        for(j = 0; j < 30; j++)
        {
            if(mangaDispo[IDManga - 1][j] == '_')
                mangaDispo[IDManga - 1][j] = ' ';
        }

        for(j = 0; j < 30; j++)
        {
            if(mangaDispo[IDManga - 1][j] == ' ')
            mangaDispo[IDManga - 1][j] = '_';
        }
        SDL_Flip(ecran);
        do
        {
            check4change = 0;

            SDL_WaitEvent(&event);

            switch(event.type)
            {
                case SDL_QUIT:
                    *chapitreChoisis = -1;
                    SDL_FreeSurface(bandeauSup);
                    SDL_FreeSurface(bandeauLat);
                    SDL_FreeSurface(chapitre);
                    SDL_FreeSurface(bandeauInf);
                    SDL_FreeSurface(fond);
                    SDL_FreeSurface(positionChap);
                    return 0;
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    if(event.button.button == SDL_BUTTON_WHEELDOWN)
                    {
                        if(positionPage.y < (pageActuelle.hauteurP - (HAUTEUR - bandeauInf->h - BORDURE_HORIZONTALE)))
                            positionPage.y = positionPage.y + DEPLACEMENT;

                        SDL_BlitSurface(chapitre, &positionPage, chapitre, &positionAffichage);
                    }

                    else if (event.button.button == SDL_BUTTON_WHEELUP)
                    {
                        if(positionPage.y > 0)
                            positionPage.y = positionPage.y - DEPLACEMENT;

                        SDL_BlitSurface(chapitre, &positionPage, chapitre, &positionAffichage);
                    }

                    else
                    {
                        if(event.button.y > (HAUTEUR - 100 + 10) && event.button.y < (HAUTEUR - 100 + 75)) //Si le curseur est au niveau des fleches sur l'axe y
                        {
                            if(event.button.x >= positionTemp.x + 30 && event.button.x <= positionTemp.x + 125)
                            {
                                if(*chapitreChoisis > extremesManga[0])
                                    {
                                        *chapitreChoisis = *chapitreChoisis - 1;
                                        return 0;
                                    }
                            }

                            else if(event.button.x >= positionTemp.x + 185 && event.button.x <= positionTemp.x + 285)
                            {
                                if (pageEnCoursDeLecture > 1)
                                {
                                    pageEnCoursDeLecture--;
                                    changementPage = -1;
                                }
                            }

                            else if (event.button.x >= positionTemp.x + 510 && event.button.x <= positionTemp.x + 605)
                            {
                                if (pageEnCoursDeLecture < pageActuelle.pageTotal)
                                {
                                    pageEnCoursDeLecture++;
                                    changementPage = 1;
                                }
                            }

                            else if (event.button.x >= positionTemp.x + 665 && event.button.x <= positionTemp.x + 760)
                            {
                                if(*chapitreChoisis < extremesManga[1])
                                {
                                    *chapitreChoisis = *chapitreChoisis + 1; //Il faut faire en sorte qu'il soit possible de changer de chapitre
                                    return 0;
                                }
                            }
                        }
                    }
                    SDL_Flip(ecran);
                    break;

                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym)
                    {
                        case SDLK_UP:
                            if(positionPage.y > 0)
                                positionPage.y = positionPage.y - DEPLACEMENT;

                            SDL_BlitSurface(chapitre, &positionPage, chapitre, &positionAffichage);
                            break;

                        case SDLK_DOWN:
                            if(positionPage.y < (pageActuelle.hauteurP - (HAUTEUR - bandeauInf->h - BORDURE_HORIZONTALE)))
                                positionPage.y = positionPage.y + DEPLACEMENT;

                            SDL_BlitSurface(chapitre, &positionPage, chapitre, &positionAffichage);
                            break;

                        case SDLK_n:
                            if(*chapitreChoisis < extremesManga[1])
                            {
                                *chapitreChoisis = *chapitreChoisis + 1; //Il faut faire en sorte qu'il soit possible de changer de chapitre
                                return 0;
                            }
                            break;

                        case SDLK_RIGHT:
                            if (pageEnCoursDeLecture < pageActuelle.pageTotal)
                            {
                                pageEnCoursDeLecture++;
                                changementPage = 1;
                            }
                            break;

                        case SDLK_p:
                            if(*chapitreChoisis > extremesManga[0])
                            {
                                *chapitreChoisis = *chapitreChoisis - 1;
                                return 0;
                            }
                            break;

                        case SDLK_LEFT:
                            if (pageEnCoursDeLecture > 1)
                            {
                                pageEnCoursDeLecture--;
                                changementPage = -1;
                            }
                            break;

                        case SDLK_ESCAPE:
                            *chapitreChoisis = -3;
                            SDL_FreeSurface(fond);
                            SDL_FreeSurface(ecran); //Test, risque de bug majeur
                            SDL_FreeSurface(chapitre);
                            SDL_FreeSurface(bandeauLat);
                            ecran = SDL_SetVideoMode(800, HAUTEUR, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
                            fond = SDL_CreateRGBSurface(SDL_HWSURFACE, 800, HAUTEUR, 32, 0, 0 , 0, 0);
                            SDL_BlitSurface(fond, NULL, ecran, NULL);
                            return 0;
                            break;

                        default:
                            break;

                    }
                    SDL_Flip(ecran);
                    break;

                default:
                    check4change = 1;
                    break;
            }
        } while(check4change);
    }
    SDL_FreeSurface(bandeauSup);
    SDL_FreeSurface(bandeauInf);
    SDL_FreeSurface(fond);
    SDL_FreeSurface(positionChap);
    SDL_FreeSurface(bandeauLat);
    fclose(config);
    return 0;
}
