#include "main.h"

int lecteur(int *chapitreChoisis, char mangaDispo[MANGA_MAX][30], int IDManga, SDL_Surface *ecran, int extremesManga[2])
{
    int j = 0, pageEnCoursDeLecture = 0, check4change = 0;
    char temp[100] = {0};
    FILE* config = NULL;
    struct page pageActuelle;
    SDL_Rect positionTemp, positionPage, positionAffichage;
    SDL_Surface *bandeauSup = NULL, *bandeauInf = NULL, *fond = NULL;
    SDL_Event event;

    check4change = *chapitreChoisis;

    positionAffichage.y = BORDURE_HORIZONTALE;
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
        SDL_Surface *chapitre = IMG_Load(temp);
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

        positionAffichage.x = (ecran->w / 2) - (chapitre->w / 2);
        SDL_BlitSurface(fond, NULL, ecran, NULL);
        SDL_BlitSurface(chapitre, &positionPage, ecran, &positionAffichage);
        SDL_BlitSurface(bandeauSup, NULL, ecran, NULL);
        positionTemp.x = (ecran->w / 2) - (bandeauInf->w / 2);
        positionTemp.y = HAUTEUR - 100;
        SDL_BlitSurface(bandeauInf, NULL, ecran, &positionTemp);
        SDL_Flip(ecran);
        SDL_WaitEvent(&event);

        if((event.active.state & SDL_APPACTIVE) == SDL_APPACTIVE || (event.active.state & SDL_APPINPUTFOCUS) == SDL_APPINPUTFOCUS)
            miseEnPause();

        switch(event.type)
        {
            case SDL_QUIT:
                *chapitreChoisis = -1;
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
                                    pageEnCoursDeLecture--;
                        }

                        else if (event.button.x >= positionTemp.x + 510 && event.button.x <= positionTemp.x + 605)
                        {
                            if (pageEnCoursDeLecture < pageActuelle.pageTotal)
                                pageEnCoursDeLecture++;
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
                            pageEnCoursDeLecture++;
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
                            pageEnCoursDeLecture--;
                        break;

                    case SDLK_ESCAPE:
                        *chapitreChoisis = -2;
                        SDL_FreeSurface(fond);
                        SDL_FreeSurface(ecran); //Test, risque de bug majeur
                        SDL_FreeSurface(chapitre);
                        ecran = SDL_SetVideoMode(800, HAUTEUR, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
                        fond = SDL_CreateRGBSurface(SDL_HWSURFACE, 800, HAUTEUR, 32, 0, 0 , 0, 0);
                        SDL_BlitSurface(fond, NULL, ecran, NULL);
                        return 0;
                        break;

                    default:
                        break;
                }
        }
        SDL_FreeSurface(bandeauSup);
        SDL_FreeSurface(chapitre);
    }
    fclose(config);
    return 0;
}
