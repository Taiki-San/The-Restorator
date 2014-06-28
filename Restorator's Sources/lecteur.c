/*********************************************
**                                          **
**           The Restorator v3.2.1          **
**           Released by 29/05/2012         **
**                                          **
**********************************************/

#include "main.h"

int lecteur(int *chapitreChoisis, int *fullscreen, char mangaDispo[LONGUEUR_NOM_MANGA_MAX], char team[LONGUEUR_NOM_MANGA_MAX])
{
    int i = 0, pageEnCoursDeLecture = 0, check4change = 0, changementPage = 0, pageTotal = 0, restoreState = 0, finDuChapitre = 0;
    int buffer = 0, largeurValide = 0, pageTropGrande = 0, modeEco = 0, scriptUtilise = 0, tempsDebutExplication = 0;
    int anciennePositionX = 0, anciennePositionY = 0, deplacementX = 0, deplacementY = 0, pageCharge = 0, changementEtat = 0;
    int deplacementEnCours = 0, largeurPages[NOMBRE_PAGE_MAX] = {0}, hauteurPages[NOMBRE_PAGE_MAX] = {0}, extremesManga[2];
    char temp[TAILLE_BUFFER], nomPage[NOMBRE_PAGE_MAX][LONGUEUR_NOM_PAGE], infos[300], texteTrad[8][LONGUEURTEXTE];
    FILE* config = NULL;
    FILE* testExistance = NULL;
    SDL_Surface *chapitre = NULL, *OChapitre = NULL, *NChapitre = NULL, *bandeauSup = NULL, *bandeauLat = NULL, *bandeauControle = NULL;
    SDL_Surface *infoSurface = NULL, *bufferExplication = NULL, *explication = NULL;
    TTF_Font *police = NULL;
    SDL_Rect position, positionPage, positionBandeauControle, positionSlide, positionTemp, positionExplication;
    SDL_Color couleurTexte = {POLICE_R, POLICE_G, POLICE_B}, couleurFinChapitre = {POLICE_NEW_R, POLICE_NEW_G, POLICE_NEW_B};;
    SDL_Event event;

    police = TTF_OpenFont(FONTUSED, POLICE_PETIT);
    TTF_SetFontStyle(police, TTF_STYLE_UNDERLINE);

    lectureTrad(texteTrad, 21);

    restoreState = checkRestore();
    if(restoreState)
    {
        testExistance = fopenR("data/laststate.dat", "r");
        fscanf(testExistance, "%s %d %d", temp, &i, &pageEnCoursDeLecture);
        fclose(testExistance);
        removeR("data/laststate.dat");

        /**Création de la fenêtre d'infos**/
        explication = SDL_CreateRGBSurface(SDL_HWSURFACE, LARGEUR_EXPLICATION, HAUTEUR_EXPLICATION, 32, 0, 0 , 0, 0);
#ifdef MACOS
		SDL_FillRect(explication, NULL, SDL_Swap32(SDL_MapRGB(ecran->format, FOND_R, FOND_G, FOND_B))); //We change background color
#else
		SDL_FillRect(explication, NULL, SDL_MapRGB(ecran->format, FOND_R, FOND_G, FOND_B)); //We change background color
#endif
        position.y = BORDURE_SUP_EXPLICATION;
        for(i = 0; i < 4; i++)
        {
            bufferExplication = TTF_RenderText_Blended(police, texteTrad[i + 2], couleurTexte);
            position.x = explication->w / 2 - bufferExplication->w / 2;
            SDL_BlitSurface(bufferExplication, NULL, explication, &position);
            position.y = position.y + bufferExplication->h + INTERLIGNE_EXPLICATION;
            SDL_FreeSurface(bufferExplication);
        }
        tempsDebutExplication = SDL_GetTicks();
    }

    positionPage.x = BORDURE_LAT_LECTURE;
    positionSlide.x = 0;
    positionSlide.y = 0;

    crashTemp(temp, TAILLE_BUFFER);
    sprintf(temp, "manga/%s/config.dat", mangaDispo);
    config = fopenR(temp, "r");
    if(config != NULL)
    {
        fscanf(config, "%d %d", &extremesManga[0], &extremesManga[1]);
        fclose(config);
    }

    crashTemp(temp, TAILLE_BUFFER);
    sprintf(temp, "manga/%s/Chapitre_%d/config.dat", mangaDispo, *chapitreChoisis);
    config = fopenR(temp, "r");

    /*Si chapitre manquant*/
    while(config == NULL && *chapitreChoisis < extremesManga[1])
    {
        *chapitreChoisis = *chapitreChoisis + 1;
        crashTemp(temp, TAILLE_BUFFER);
        sprintf(temp, "manga/%s/Chapitre_%d/config.dat", mangaDispo, *chapitreChoisis);
        config = fopenR(temp, "r");
    }

    if(config == NULL)
    {
        testExistance = fopenR("erreur.txt", "a+");
        fprintf(testExistance, "Chapitre non-existant: Team: %s - Manga: %s - Chapitre: %d\n", team, mangaDispo, *chapitreChoisis);
        fclose(testExistance);

        i = showError();
        SDL_FreeSurface(chapitre);
        SDL_FreeSurface(OChapitre);
        SDL_FreeSurface(NChapitre);
        SDL_FreeSurface(bandeauSup);
        SDL_FreeSurface(infoSurface);
        SDL_FreeSurface(bandeauControle);
        restartEcran();
        if(i > -3)
            return -2;
        else
            return i;
    }

    /*Réinintialisation*/
    for(i = 0; i < NOMBRE_PAGE_MAX; i++)
    {
        for(changementPage = 0; changementPage < LONGUEUR_NOM_PAGE; changementPage++)
            nomPage[i][changementPage] = 0;
    }

    sprintf(temp, "%sdata/%s/bandeau.png", BUNDLENAME, LANGUAGE_PATH[langue - 1]);
    bandeauControle = IMG_Load(temp);
    changementPage = 0;


    fscanf(config, "%d\n", &pageTotal);
    if(fgetc(config) != EOF)
    {
        scriptUtilise = 0;
        fseek(config, -1, SEEK_CUR);
        if(fgetc(config) == 'N')
            scriptUtilise = 1;
        else
            fseek(config, -1, SEEK_CUR);
        for(i = 0; i < pageTotal; i++)
        {
            if(!scriptUtilise)
                fscanf(config, "%d %s %d %d\n", &changementPage, nomPage[i], &largeurPages[i], &hauteurPages[i]);

            else if(scriptUtilise)
                fscanf(config, "%d %s\n", &changementPage, nomPage[i]);

            for(buffer = 0; buffer < LONGUEUR_NOM_PAGE; buffer++)
            {
                if(nomPage[i][buffer] == '&')
                    nomPage[i][buffer] = ' ';
            }
        }
        modeEco = 0;
    }
    else
    {
        pageTotal++;
        for(i = 0; i < pageTotal; i++)
        {
            sprintf(nomPage[i], "%d.jpg", i);
        }
        modeEco = 1;
    }
    fclose(config);
    changementPage = 2;

    while(1)
    {
        crashTemp(temp, TAILLE_BUFFER);
        sprintf(temp, "manga/%s/Chapitre_%d/%s", mangaDispo, *chapitreChoisis, nomPage[pageEnCoursDeLecture]);
        testExistance = fopenR(temp, "r");
        if(testExistance == NULL)
        {
            testExistance = fopenR("erreur.txt", "a+");
            fprintf(testExistance, "Page non-existant: Team: %s - Manga: %s - Chapitre: %d - Nom Page: %s\n", team, mangaDispo, *chapitreChoisis, nomPage[pageEnCoursDeLecture]);
            fclose(testExistance);

            i = showError();
            SDL_FreeSurface(chapitre);
            if(pageEnCoursDeLecture > 0)
                SDL_FreeSurface(OChapitre);
            if(pageEnCoursDeLecture < pageTotal)
                SDL_FreeSurface(NChapitre);
            SDL_FreeSurface(bandeauSup);
            SDL_FreeSurface(infoSurface);
            SDL_FreeSurface(bandeauControle);
            restartEcran();
            if(i > -3)
                return -2;
            else
                return i;
        }

        fclose(testExistance);


        /*On libère la mémoire <3*/
        SDL_FreeSurface(bandeauSup);
        SDL_FreeSurface(infoSurface);

        /*Chargement image*/
        if(changementPage == 1 && pageEnCoursDeLecture <= pageTotal && finDuChapitre == 0 && !changementEtat)
        {
            if(pageEnCoursDeLecture != 1)
                SDL_FreeSurface(OChapitre);
            OChapitre = SDL_CreateRGBSurface(SDL_HWSURFACE, chapitre->w, chapitre->h, 32, 0, 0 , 0, 0);
            SDL_BlitSurface(chapitre, NULL, OChapitre, NULL);
            SDL_FreeSurface(chapitre);
            chapitre = SDL_CreateRGBSurface(SDL_HWSURFACE, NChapitre->w, NChapitre->h, 32, 0, 0 , 0, 0);
            SDL_BlitSurface(NChapitre, NULL, chapitre, NULL);
            SDL_FreeSurface(NChapitre);
        }
        else if(changementPage == -1 && pageEnCoursDeLecture >= 0 && finDuChapitre == 0 && !changementEtat)
        {
            if(pageEnCoursDeLecture != pageTotal - 1)
                SDL_FreeSurface(NChapitre);
            NChapitre = SDL_CreateRGBSurface(SDL_HWSURFACE, chapitre->w, chapitre->h, 32, 0, 0 , 0, 0);
            SDL_BlitSurface(chapitre, NULL, NChapitre, NULL);
            SDL_FreeSurface(chapitre);
            chapitre = SDL_CreateRGBSurface(SDL_HWSURFACE, OChapitre->w, OChapitre->h, 32, 0, 0 , 0, 0);
            SDL_BlitSurface(OChapitre, NULL, chapitre, NULL);
            SDL_FreeSurface(OChapitre);
        }
        else if(pageEnCoursDeLecture >= 0 && pageEnCoursDeLecture <= pageTotal && finDuChapitre == 0 && !changementEtat)
        {
            sprintf(temp, "%smanga/%s/Chapitre_%d/%s", BUNDLENAME, mangaDispo, *chapitreChoisis, nomPage[pageEnCoursDeLecture]);
            changementPage = 1;
            chapitre = IMG_Load(temp); //On ouvre le chapitre
        }

        if(modeEco == 1 || scriptUtilise == 1)
        {
            largeurPages[pageEnCoursDeLecture] = chapitre->w;
            hauteurPages[pageEnCoursDeLecture] = chapitre->h;
        }


        largeurValide = largeurPages[pageEnCoursDeLecture] + BORDURE_LAT_LECTURE * 2;
        buffer = hauteurPages[pageEnCoursDeLecture] + BORDURE_HOR_LECTURE + BORDURE_CONTROLE_LECTEUR;

        if(buffer > HAUTEUR_MAX)
            buffer = HAUTEUR_MAX;

        /*Affichage des infos*/
        crashTemp(infos, 300);
        for(i = 0; i < LONGUEUR_NOM_MANGA_MAX; i++)
        {
            if(mangaDispo[i] == '_')
                mangaDispo[i] = ' ';

            if(team[i] == '_')
                team[i] = ' ';
        }

        if(*fullscreen == 1)
            sprintf(infos, "%s - Team: %s - Manga: %s - %s: %d - %s: %d / %d - %s", texteTrad[6], team, mangaDispo, texteTrad[0], *chapitreChoisis, texteTrad[1], (pageEnCoursDeLecture + 1), pageTotal, texteTrad[7]);

        else
            sprintf(infos, "Team: %s - Manga: %s - %s: %d - %s: %d / %d", team, mangaDispo, texteTrad[0], *chapitreChoisis, texteTrad[1], (pageEnCoursDeLecture + 1), pageTotal);
        for(i = 0; i < LONGUEUR_NOM_MANGA_MAX; i++)
        {
            if(mangaDispo[i] == ' ')
                mangaDispo[i] = '_';

            if(team[i] == ' ')
                team[i] = '_';
        }

        /*Initialisation des différentes surfaces*/

        if(*fullscreen == 0)
        {
            /*Si grosse page*/
            if(largeurValide > LARGEUR_MAX)
            {
                largeurValide = LARGEUR_MAX;
                pageTropGrande = 1;
            }

            else if(largeurValide < LARGEUR)
            {
                largeurValide = LARGEUR;
                pageTropGrande = 0;
            }

            else
            {
                pageTropGrande = 0;
            }
            SDL_FreeSurface(ecran);
            SDL_FreeSurface(fond);

            ecran = SDL_SetVideoMode(largeurValide, buffer, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
            fond = SDL_CreateRGBSurface(SDL_HWSURFACE, largeurValide, buffer, 32, 0, 0 , 0, 0); //on initialise le fond
            bandeauLat = SDL_CreateRGBSurface(SDL_HWSURFACE, largeurValide, BORDURE_CONTROLE_LECTEUR, 32, 0, 0 , 0, 0);
            bandeauSup = SDL_CreateRGBSurface(SDL_HWSURFACE, largeurValide, BORDURE_HOR_LECTURE, 32, 0, 0 , 0, 0);
#ifdef MACOS
            SDL_FillRect(fond, NULL, SDL_Swap32(SDL_MapRGB(ecran->format, FOND_R, FOND_G, FOND_B))); //We change background color
            SDL_FillRect(bandeauLat, NULL, SDL_Swap32(SDL_MapRGB(ecran->format, FOND_R, FOND_G, FOND_B))); //We change background color
            SDL_FillRect(bandeauSup, NULL, SDL_Swap32(SDL_MapRGB(ecran->format, FOND_R, FOND_G, FOND_B))); //We change background color
#else
            SDL_FillRect(fond, NULL, SDL_MapRGB(ecran->format, FOND_R, FOND_G, FOND_B)); //We change background color
            SDL_FillRect(bandeauLat, NULL, SDL_MapRGB(ecran->format, FOND_R, FOND_G, FOND_B)); //We change background color
            SDL_FillRect(bandeauSup, NULL, SDL_MapRGB(ecran->format, FOND_R, FOND_G, FOND_B)); //We change background color
#endif
            SDL_BlitSurface(fond, NULL, ecran, NULL);
        }

        if(*fullscreen == 1)
        {
            if(changementEtat)
            {
                SDL_FreeSurface(ecran);
                SDL_FreeSurface(fond);
                ecran = SDL_SetVideoMode(RESOLUTION[0], RESOLUTION[1], 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);
                fond = SDL_CreateRGBSurface(SDL_HWSURFACE, RESOLUTION[0], RESOLUTION[1], 32, 0, 0 , 0, 0); //on initialise le fond
                SDL_FillRect(fond, NULL, SDL_MapRGB(ecran->format, FOND_R, FOND_G, FOND_B)); //We change background color
            }

            SDL_BlitSurface(fond, NULL, ecran, NULL);
            bandeauLat = SDL_CreateRGBSurface(SDL_HWSURFACE, RESOLUTION[0], BORDURE_CONTROLE_LECTEUR, 32, 0, 0 , 0, 0);
            SDL_FillRect(bandeauLat, NULL, SDL_MapRGB(ecran->format, FOND_R, FOND_G, FOND_B)); //We change background color
            bandeauSup = SDL_CreateRGBSurface(SDL_HWSURFACE, RESOLUTION[0], BORDURE_HOR_LECTURE, 32, 0, 0 , 0, 0);
            SDL_FillRect(bandeauSup, NULL, SDL_MapRGB(ecran->format, FOND_R, FOND_G, FOND_B)); //En mode plein écran, pas besoin d'inverser

            /*Si grosse page*/
            if(largeurValide > ecran->w)
            {
                largeurValide = ecran->w;
                pageTropGrande = 1;
            }

            else if(largeurValide < LARGEUR)
            {
                largeurValide = LARGEUR;
                pageTropGrande = 0;
            }

            else
            {
                pageTropGrande = 0;
            }
        }

        if(*fullscreen == 1)
        {
            TTF_CloseFont(police);
            police = TTF_OpenFont(FONTUSED, POLICE_TOUT_PETIT);
            TTF_SetFontStyle(police, TTF_STYLE_UNDERLINE);
        }
        if(finDuChapitre == 0)
            infoSurface = TTF_RenderText_Blended(police, infos, couleurTexte);
        else
            infoSurface = TTF_RenderText_Blended(police, infos, couleurFinChapitre);

        if(*fullscreen == 1)
        {
            TTF_CloseFont(police);
            police = TTF_OpenFont(FONTUSED, POLICE_PETIT);
            TTF_SetFontStyle(police, TTF_STYLE_UNDERLINE);
        }

        /*On affiche les différentes surfaces*/
        SDL_BlitSurface(fond, NULL, ecran, NULL);

        /*On affiche les bordures latéralles*/
        SDL_BlitSurface(bandeauLat, NULL, ecran, NULL);
        /*On positionne a l'extrême droite de la fenêtre*/
        positionTemp.x = 0;
        positionTemp.y = ecran->h - BORDURE_CONTROLE_LECTEUR;
        SDL_BlitSurface(bandeauLat, NULL, ecran, &positionTemp);
        SDL_FreeSurface(bandeauLat);

        /*On affiche la bordure supérieur (bandeau + infos*/
        SDL_BlitSurface(bandeauSup, NULL, ecran, NULL);
        position.x = (ecran->w / 2) - (infoSurface->w / 2);
        position.y = (BORDURE_HOR_LECTURE / 2) - (infoSurface->h / 2);
        SDL_BlitSurface(infoSurface, NULL, ecran, &position);

        /*On affiche le bandeau de contrôle inférieur*/
        positionBandeauControle.x = 0;
        positionBandeauControle.y = (ecran->h - BORDURE_CONTROLE_LECTEUR);
        SDL_BlitSurface(fond, NULL, ecran, &positionBandeauControle);
        positionBandeauControle.x = (ecran->w / 2) - (bandeauControle->w / 2);
        SDL_BlitSurface(bandeauControle, NULL, ecran, &positionBandeauControle);

        /*Calcul position page*/
        if(pageTropGrande == 0)
        {
            positionSlide.w = ecran->w - (2 * BORDURE_LAT_LECTURE);
            positionSlide.h = ecran->h - (2 * BORDURE_HOR_LECTURE);
            if(!finDuChapitre)
            {
                positionSlide.x = 0;
                positionSlide.y = 0;
            }
            if(chapitre->w < LARGEUR - BORDURE_LAT_LECTURE * 2 || *fullscreen)
                positionPage.x = ecran->w / 2 - chapitre->w / 2;
            else if (!*fullscreen)
                positionPage.x = BORDURE_LAT_LECTURE;
        }
        else
        {
            positionSlide.w = chapitre->w;
            positionSlide.h = chapitre->h;
            if(!finDuChapitre)
            {
                positionSlide.x = chapitre->w - (ecran->w - 2 * BORDURE_LAT_LECTURE);
                positionSlide.y = 0;
            }
            positionPage.x = BORDURE_LAT_LECTURE;
        }
        if(!changementEtat)
            pageCharge = 0;
        else
            changementEtat = 0;

        SDL_Flip(ecran);
        do
        {
            check4change = 1;
            positionPage.y = BORDURE_HOR_LECTURE;

            SDL_BlitSurface(fond, NULL, ecran, NULL);

            SDL_BlitSurface(chapitre, &positionSlide, ecran, &positionPage);

            positionBandeauControle.x = 0;
            positionBandeauControle.y = (ecran->h - BORDURE_CONTROLE_LECTEUR);
            SDL_BlitSurface(fond, NULL, ecran, &positionBandeauControle);

            positionBandeauControle.x = (ecran->w / 2) - (bandeauControle->w / 2);
            SDL_BlitSurface(bandeauControle, NULL, ecran, &positionBandeauControle);

            SDL_BlitSurface(bandeauSup, NULL, ecran, NULL);
            SDL_BlitSurface(infoSurface, NULL, ecran, &position);

            if(restoreState && SDL_GetTicks() - tempsDebutExplication < DUREE_ARRICHAGE_EXPLCIATION)
            {
                positionExplication.x = ecran->w / 2 - explication->w / 2;
                positionExplication.y = ecran->h / 2 - explication->h / 2;
                SDL_BlitSurface(explication, NULL, ecran, &positionExplication);
            }

            else if(restoreState)
            {
                SDL_FreeSurface(explication);
                restoreState = 0;
            }

            SDL_Flip(ecran);

            if(!pageCharge)
            {
                crashTemp(temp, TAILLE_BUFFER);
                if(changementPage == 1 && pageEnCoursDeLecture < pageTotal)
                {
                    sprintf(temp, "%smanga/%s/Chapitre_%d/%s", BUNDLENAME, mangaDispo, *chapitreChoisis, nomPage[pageEnCoursDeLecture + 1]);
                    NChapitre = IMG_Load(temp);
                }
                else if (changementPage == -1 && pageEnCoursDeLecture > 0)
                {
                    sprintf(temp, "%smanga/%s/Chapitre_%d/%s", BUNDLENAME, mangaDispo, *chapitreChoisis, nomPage[pageEnCoursDeLecture - 1]);
                    OChapitre = IMG_Load(temp);
                }
                pageCharge = 1;
                changementPage = 0;
            }

            event.type = 0;
            SDL_WaitEvent(&event);

            switch(event.type)
            {
				case SDL_QUIT:
                    /*Si on quitte, on enregistre le point d'arret*/
                    testExistance = fopenR("data/laststate.dat", "w+");
                    fprintf(testExistance, "%s %d %d", mangaDispo, *chapitreChoisis, pageEnCoursDeLecture);
                    fclose(testExistance);

                    SDL_FreeSurface(chapitre);
                    if(pageEnCoursDeLecture > 0)
                        SDL_FreeSurface(OChapitre);
                    if(pageEnCoursDeLecture < pageTotal)
                        SDL_FreeSurface(NChapitre);
                    SDL_FreeSurface(bandeauSup);
                    SDL_FreeSurface(infoSurface);
                    SDL_FreeSurface(bandeauControle);
                    restartEcran();

                    return -4;
                    break;

                case SDL_MOUSEBUTTONUP:
                {
                    if(event.button.button == SDL_BUTTON_WHEELDOWN)
                    {
                        if(positionSlide.y < hauteurPages[pageEnCoursDeLecture] - (ecran->h - BORDURE_CONTROLE_LECTEUR - BORDURE_HOR_LECTURE) - DEPLACEMENT)
                        {
                            positionSlide.y = positionSlide.y + DEPLACEMENT;
                        }
                        else
                        {
                            positionSlide.y = hauteurPages[pageEnCoursDeLecture] - (ecran->h - BORDURE_CONTROLE_LECTEUR - BORDURE_HOR_LECTURE);
                        }
                    }

                    else if (event.button.button == SDL_BUTTON_WHEELUP)
                    {
                        if(positionSlide.y > DEPLACEMENT)
                        {
                            positionSlide.y = positionSlide.y - DEPLACEMENT;
                        }
                        else
                        {
                            positionSlide.y = 0;
                        }
                    }

                    else if (event.button.y < BORDURE_HOR_LECTURE)
                        ouvrirSite(team); //Ouverture du site de la team

                    else
                    {
                        if(event.button.y > (ecran->h - BORDURE_CONTROLE_LECTEUR + 10) && event.button.y < (ecran->h - BORDURE_CONTROLE_LECTEUR + 75)) //Si le curseur est au niveau des fleches sur l'axe y
                        {
                            if(event.button.x >= positionBandeauControle.x + 30 && event.button.x <= positionBandeauControle.x + 125)
                            {
                                anythingNew(extremesManga, mangaDispo);
                                if(*chapitreChoisis > extremesManga[0])
                                {
                                        *chapitreChoisis = *chapitreChoisis - 1;
                                        SDL_FreeSurface(chapitre);
                                        if(pageEnCoursDeLecture > 0)
                                            SDL_FreeSurface(OChapitre);
                                        if(pageEnCoursDeLecture < pageTotal)
                                            SDL_FreeSurface(NChapitre);
                                        SDL_FreeSurface(bandeauSup);
                                        SDL_FreeSurface(infoSurface);
                                        SDL_FreeSurface(bandeauControle);
                                        return 0;
                                }
                                else
                                {
                                    check4change = 0;
                                    if(finDuChapitre == 1)
                                        check4change = 1;
                                    else
                                        finDuChapitre = 1;

                                }
                            }

                            else if(event.button.x >= positionBandeauControle.x + 185 && event.button.x <= positionBandeauControle.x + 285)
                            {
                                anythingNew(extremesManga, mangaDispo);
                                if (pageEnCoursDeLecture > 0)
                                {
                                    pageEnCoursDeLecture--;
                                    changementPage = -1;
                                    check4change = 0;
                                    finDuChapitre = 0;
                                }
                                else if(*chapitreChoisis > extremesManga[0])
                                {
                                    *chapitreChoisis = *chapitreChoisis - 1;
                                    SDL_FreeSurface(chapitre);
                                    if(pageEnCoursDeLecture > 0)
                                        SDL_FreeSurface(OChapitre);
                                    if(pageEnCoursDeLecture < pageTotal)
                                        SDL_FreeSurface(NChapitre);
                                    SDL_FreeSurface(bandeauSup);
                                    SDL_FreeSurface(infoSurface);
                                    SDL_FreeSurface(bandeauControle);
                                    return 0;
                                }
                                else
                                {
                                    check4change = 0;
                                    if(finDuChapitre == 1)
                                        check4change = 1;
                                    else
                                        finDuChapitre = 1;

                                }
                            }

                            else if (event.button.x >= positionBandeauControle.x + 510 && event.button.x <= positionBandeauControle.x + 605)
                            {
                                anythingNew(extremesManga, mangaDispo);
                                if (pageEnCoursDeLecture < pageTotal - 1)
                                {
                                    pageEnCoursDeLecture++;
                                    changementPage = 1;
                                    check4change = 0;
                                    finDuChapitre = 0;
                                }
                                else if(*chapitreChoisis < extremesManga[1])
                                {
                                    *chapitreChoisis = *chapitreChoisis + 1;
                                    SDL_FreeSurface(chapitre);
                                    if(pageEnCoursDeLecture > 0)
                                        SDL_FreeSurface(OChapitre);
                                    if(pageEnCoursDeLecture < pageTotal)
                                        SDL_FreeSurface(NChapitre);
                                    SDL_FreeSurface(bandeauSup);
                                    SDL_FreeSurface(infoSurface);
                                    SDL_FreeSurface(bandeauControle);
                                    return 0;
                                }
                                else
                                {
                                    check4change = 0;
                                    if(finDuChapitre == 1)
                                        check4change = 1;
                                    else
                                        finDuChapitre = 1;

                                }
                            }

                            else if (event.button.x >= positionBandeauControle.x + 665 && event.button.x <= positionBandeauControle.x + 760)
                            {
                                anythingNew(extremesManga, mangaDispo);
                                if(*chapitreChoisis < extremesManga[1])
                                {
                                    *chapitreChoisis = *chapitreChoisis + 1; //Il faut faire en sorte qu'il soit possible de changer de chapitre
                                    SDL_FreeSurface(chapitre);
                                    if(pageEnCoursDeLecture > 0)
                                        SDL_FreeSurface(OChapitre);
                                    if(pageEnCoursDeLecture < pageTotal)
                                        SDL_FreeSurface(NChapitre);
                                    SDL_FreeSurface(bandeauSup);
                                    SDL_FreeSurface(infoSurface);
                                    SDL_FreeSurface(bandeauControle);
                                    return 0;
                                }
                                else
                                {
                                    check4change = 0;
                                    if(finDuChapitre == 1)
                                        check4change = 1;
                                    else
                                        finDuChapitre = 1;
                                }
                            }
                        }
                    }
                    break;
                }

                case SDL_MOUSEBUTTONDOWN:
                {
                    if(event.button.button == SDL_BUTTON_WHEELDOWN)
                    {
                        if(positionSlide.y < hauteurPages[pageEnCoursDeLecture] - (ecran->h - BORDURE_CONTROLE_LECTEUR - BORDURE_HOR_LECTURE) - DEPLACEMENT)
                        {
                            positionSlide.y = positionSlide.y + DEPLACEMENT;
                        }
                        else
                        {
                            positionSlide.y = hauteurPages[pageEnCoursDeLecture] - (ecran->h - BORDURE_CONTROLE_LECTEUR - BORDURE_HOR_LECTURE);
                        }
                    }

                    else if (event.button.button == SDL_BUTTON_WHEELUP)
                    {
                        if(positionSlide.y > DEPLACEMENT)
                        {
                            positionSlide.y = positionSlide.y - DEPLACEMENT;
                        }
                        else
                        {
                            positionSlide.y = 0;
                        }
                    }

                    else if(pageTropGrande == 1 && event.button.y < ecran->h - BORDURE_CONTROLE_LECTEUR)
                    {
                        deplacementEnCours = 1;
                        while(deplacementEnCours)
                        {
                            anciennePositionX = event.button.x;
                            anciennePositionY = event.button.y;
                            event.type = -1;
                            SDL_WaitEvent(&event);
                            switch(event.type)
                            {
                                case SDL_MOUSEMOTION:
                                {
                                    /*Si on déplace la souris:
                                    On va récuperer le déplacement, tester si il est possible puis l'appliquer*/
                                    deplacementX = (anciennePositionX - event.motion.x);
                                    deplacementY = (anciennePositionY - event.motion.y);
                                    if(deplacementX < 0)
                                    {
                                        deplacementX = deplacementX % -10;
                                    }
                                    else
                                    {
                                        deplacementX = deplacementX % 10;
                                    }

                                    if(deplacementY < 0)
                                    {
                                        deplacementY = deplacementY % -10;
                                    }
                                    else
                                    {
                                        deplacementY = deplacementY % 10;
                                    }

                                    if(deplacementX > 0)
                                    {
                                        /*Si un déplacement vers le droite*/
                                        if(positionSlide.x + deplacementX * DEPLACEMENT_LATERAL_PAGE <= chapitre->w - (ecran->w - 2 * BORDURE_LAT_LECTURE))
                                        {
                                            positionSlide.x = positionSlide.x + deplacementX * DEPLACEMENT_LATERAL_PAGE;
                                        }
                                        else if(positionSlide.x + deplacementX * DEPLACEMENT_LATERAL_PAGE < 0)
                                        {
                                            positionSlide.x = chapitre->w - (ecran->w - 2 * BORDURE_LAT_LECTURE);
                                        }
                                    }
                                    else if (deplacementX < 0)
                                    {
                                        /*Si un déplacement vers le gauche*/
                                        if(positionSlide.x + deplacementX * DEPLACEMENT_LATERAL_PAGE > 0)
                                        {
                                            positionSlide.x = positionSlide.x + deplacementX * DEPLACEMENT_LATERAL_PAGE;
                                        }
                                        else
                                        {
                                            positionSlide.x = 0;
                                        }
                                    }
                                    if(deplacementY > 0)
                                    {
                                        /*Si un déplacement vers le haut*/
                                        if(positionSlide.y < (hauteurPages[pageEnCoursDeLecture] - (ecran->h - BORDURE_CONTROLE_LECTEUR - BORDURE_HOR_LECTURE)))
                                        {
                                            positionSlide.y = positionSlide.y + deplacementY * DEPLACEMENT_HORIZONTAL_PAGE;
                                        }
                                        else
                                        {
                                            positionSlide.y = hauteurPages[pageEnCoursDeLecture] - (ecran->h - BORDURE_CONTROLE_LECTEUR - BORDURE_HOR_LECTURE);
                                        }
                                    }
                                    else if(deplacementY < 0)
                                    {
                                        /*Si un déplacement vers le bas*/
                                        if(positionSlide.y > -deplacementY * DEPLACEMENT_HORIZONTAL_PAGE)
                                        {
                                            positionSlide.y = positionSlide.y + deplacementY * DEPLACEMENT_HORIZONTAL_PAGE;
                                        }
                                        else
                                        {
                                            positionSlide.y = 0;
                                        }
                                    }
                                    SDL_BlitSurface(fond, NULL, ecran, NULL);
                                    SDL_BlitSurface(chapitre, &positionSlide, ecran, &positionPage);
                                    positionBandeauControle.x = 0;
                                    positionBandeauControle.y = (ecran->h - BORDURE_CONTROLE_LECTEUR);
                                    SDL_BlitSurface(fond, NULL, ecran, &positionBandeauControle);
                                    positionBandeauControle.x = (ecran->w / 2) - (bandeauControle->w / 2);
                                    SDL_BlitSurface(bandeauControle, NULL, ecran, &positionBandeauControle);
                                    SDL_BlitSurface(bandeauSup, NULL, ecran, NULL);
                                    SDL_BlitSurface(infoSurface, NULL, ecran, &position);

                                    if(restoreState && SDL_GetTicks() - tempsDebutExplication < 3000)
                                        SDL_BlitSurface(explication, NULL, ecran, &positionExplication);
                                    else if(restoreState)
                                        restoreState = 0;
                                    SDL_Flip(ecran);
                                    break;
                                }
                                case SDL_MOUSEBUTTONUP:
                                    deplacementEnCours = 0;
                                    break;
                            }
                        }
                    }

                    if(restoreState)
                    {
                        restoreState = 0;
                        SDL_FreeSurface(explication);
                    }
                    break;
                }

                case SDL_KEYDOWN:
                {
                    switch(event.key.keysym.sym)
                    {
                        case SDLK_DOWN:
                            if(positionSlide.y < hauteurPages[pageEnCoursDeLecture] - (ecran->h - BORDURE_CONTROLE_LECTEUR - BORDURE_HOR_LECTURE) - DEPLACEMENT)
                            {
                                positionSlide.y = positionSlide.y + DEPLACEMENT;
                            }
                            else
                            {
                                positionSlide.y = hauteurPages[pageEnCoursDeLecture] - (ecran->h - BORDURE_CONTROLE_LECTEUR - BORDURE_HOR_LECTURE);
                            }
                            break;

                        case SDLK_UP:
                            if(positionSlide.y > DEPLACEMENT)
                            {
                                positionSlide.y = positionSlide.y - DEPLACEMENT;
                            }
                            else
                            {
                                positionSlide.y = 0;
                            }
                            break;

                        case SDLK_RIGHT:
                            anythingNew(extremesManga, mangaDispo);
                            if (pageEnCoursDeLecture < pageTotal - 1)
                            {
                                pageEnCoursDeLecture++;
                                changementPage = 1;
                                check4change = 0;
                                finDuChapitre = 0;
                            }
                            else if(*chapitreChoisis < extremesManga[1])
                            {
                                *chapitreChoisis = *chapitreChoisis + 1;
                                SDL_FreeSurface(chapitre);
                                if(pageEnCoursDeLecture > 0)
                                    SDL_FreeSurface(OChapitre);
                                if(pageEnCoursDeLecture < pageTotal)
                                    SDL_FreeSurface(NChapitre);
                                SDL_FreeSurface(bandeauSup);
                                SDL_FreeSurface(infoSurface);
                                SDL_FreeSurface(bandeauControle);
                                return 0;
                            }
                            else
                            {
                                check4change = 0;
                                if(finDuChapitre == 1)
                                    check4change = 1;
                                else
                                    finDuChapitre = 1;
                            }
                            break;

                        case SDLK_LEFT:
                            anythingNew(extremesManga, mangaDispo);
                            if (pageEnCoursDeLecture > 0)
                            {
                                pageEnCoursDeLecture--;
                                changementPage = -1;
                                check4change = 0;
                                finDuChapitre = 0;
                            }
                            else if(*chapitreChoisis > extremesManga[0])
                            {
                                *chapitreChoisis = *chapitreChoisis - 1;
                                SDL_FreeSurface(chapitre);
                                if(pageEnCoursDeLecture > 0)
                                    SDL_FreeSurface(OChapitre);
                                if(pageEnCoursDeLecture < pageTotal)
                                    SDL_FreeSurface(NChapitre);
                                SDL_FreeSurface(bandeauSup);
                                SDL_FreeSurface(infoSurface);
                                SDL_FreeSurface(bandeauControle);
                                return 0;
                            }
                            else
                            {
                                check4change = 0;
                                if(finDuChapitre == 1)
                                    check4change = 1;
                                else
                                    finDuChapitre = 1;
                            }
                            break;

                        case SDLK_ESCAPE:
                            SDL_FreeSurface(chapitre);
                            if(pageEnCoursDeLecture > 0)
                                SDL_FreeSurface(OChapitre);
                            if(pageEnCoursDeLecture < pageTotal)
                                SDL_FreeSurface(NChapitre);
                            SDL_FreeSurface(bandeauSup);
                            SDL_FreeSurface(infoSurface);
                            SDL_FreeSurface(bandeauControle);
                            restartEcran();
                            return -3;
                            break;

                        case SDLK_DELETE:
                            SDL_FreeSurface(chapitre);
                            if(pageEnCoursDeLecture > 0)
                                SDL_FreeSurface(OChapitre);
                            if(pageEnCoursDeLecture < pageTotal)
                                SDL_FreeSurface(NChapitre);
                            SDL_FreeSurface(bandeauSup);
                            SDL_FreeSurface(infoSurface);
                            SDL_FreeSurface(bandeauControle);
                            restartEcran();
                            return -2;
                            break;

                        case SDLK_RETURN:
                            if(restoreState)
                            {
                                restoreState = 0;
                                SDL_FreeSurface(explication);
                            }
                            break;

                        case SDLK_f:
                            if(*fullscreen == 1)
                                *fullscreen = 0;
                            else
                                *fullscreen = 1;
                            check4change = 0;
                            changementEtat = 1;
                            break;

                        case SDLK_q:
                        case SDLK_a:
                            /*Si on quitte, on enregistre le point d'arret*/
                            testExistance = fopenR("data/laststate.dat", "w+");
                            fprintf(testExistance, "%s %d %d", mangaDispo, *chapitreChoisis, pageEnCoursDeLecture);
                            fclose(testExistance);

                            SDL_FreeSurface(chapitre);
                            if(pageEnCoursDeLecture > 0)
                                SDL_FreeSurface(OChapitre);
                            if(pageEnCoursDeLecture < pageTotal)
                                SDL_FreeSurface(NChapitre);
                            SDL_FreeSurface(bandeauSup);
                            SDL_FreeSurface(infoSurface);
                            SDL_FreeSurface(bandeauControle);

                            return -4;
                            break;


                        default:
                            break;

                    }
                    break;
                }

				default:
					if ((KMOD_LMETA & event.key.keysym.mod) && event.key.keysym.sym == SDLK_q)
					{
						/*Si on quitte, on enregistre le point d'arret*/
						testExistance = fopenR("data/laststate.dat", "w+");
						fprintf(testExistance, "%s %d %d", mangaDispo, *chapitreChoisis, pageEnCoursDeLecture);
						fclose(testExistance);

						SDL_FreeSurface(chapitre);
                        if(pageEnCoursDeLecture > 0)
                            SDL_FreeSurface(OChapitre);
                        if(pageEnCoursDeLecture < pageTotal)
                            SDL_FreeSurface(NChapitre);
						SDL_FreeSurface(bandeauSup);
						SDL_FreeSurface(infoSurface);
						SDL_FreeSurface(bandeauControle);
						restartEcran();

						return -4;
					}
					check4change = 1;
					break;
            }

        } while(check4change);
    }
    return 0;
}

