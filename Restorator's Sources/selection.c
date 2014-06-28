/*********************************************
**           								**
**           The Restorator v3.2.1          **
**     		 Released by 29/05/2012 		**
**           								**
**********************************************/

#include "main.h"

int section()
{
    /*Initialisation*/
    int i = 0, longueur[NOMBRESECTION], sectionChoisis = 0, hauteurTexte = 0;
    SDL_Surface *texte = NULL;
    TTF_Font *police = NULL;
    SDL_Rect position;
    SDL_Event event;

    SDL_Color couleurTexte = {POLICE_R, POLICE_G, POLICE_B};

    /*Affichage du texte*/
    SDL_BlitSurface(fond, NULL, ecran, NULL);

    char texteTrad[9][LONGUEURTEXTE];
    lectureTrad(texteTrad, 17);

    police = TTF_OpenFont(FONTUSED, POLICE_MOYEN);
    TTF_SetFontStyle(police, TTF_STYLE_ITALIC);

    texte = TTF_RenderText_Blended(police, texteTrad[1], couleurTexte);
    position.x = (ecran->w / 2) - (texte->w / 2);
    position.y = BORDURE_SUP_MENU + texte->h + INTERLIGNE_MENU;
    SDL_BlitSurface(texte, NULL, ecran, &position);
    SDL_FreeSurface(texte);

    TTF_CloseFont(police);
    police = TTF_OpenFont(FONTUSED, POLICE_GROS);

    texte = TTF_RenderText_Blended(police, texteTrad[0], couleurTexte);
    position.x = (ecran->w / 2) - (texte->w / 2);
    position.y = BORDURE_SUP_MENU;
    SDL_BlitSurface(texte, NULL, ecran, &position);
    //On libère pas la mémoire pour la boucle d'affichage en dessous -> pour enregistrer hauteurTexte

    TTF_SetFontStyle(police, TTF_STYLE_UNDERLINE);

    for(i = 1; i <= NOMBRESECTION; i++)
    {
        SDL_FreeSurface(texte);
        texte = TTF_RenderText_Blended(police, texteTrad[i + 2], couleurTexte);
        position.x = ecran->w / 2 - texte->w / 2;
        position.y = BORDURE_SUP_SECTION + ((texte->h + INTERLIGNE_SECTION) * i);
        SDL_BlitSurface(texte, NULL, ecran, &position);
        longueur[i - 1] = texte->w;
    }
    hauteurTexte = texte->h;
    SDL_FreeSurface(texte);
    SDL_Flip(ecran);
    TTF_SetFontStyle(police, TTF_STYLE_NORMAL);

    /*Attente de l'evenement*/
    while(sectionChoisis == 0)
    {
        event.type = -1;
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                sectionChoisis = -4;
                break;

            case SDL_KEYDOWN: //If a keyboard letter is pushed
            {
				if ((KMOD_LMETA & event.key.keysym.mod) && event.key.keysym.sym == SDLK_q)
                    sectionChoisis = -4;
				else
				{
					sectionChoisis = nombreEntree(event);
					switch(event.key.keysym.sym)
					{
						case SDLK_DELETE:
							sectionChoisis = -2;
							break;

						case SDLK_ESCAPE:
							sectionChoisis = -3;
							break;

						default: //If other one
							break;
					}
				}
                break;
            }

            case SDL_MOUSEBUTTONUP:
            {
                i = 1;
                while(((((hauteurTexte + INTERLIGNE_SECTION) * i + BORDURE_SUP_SECTION) > event.button.y) || ((hauteurTexte + INTERLIGNE_SECTION) * i + BORDURE_SUP_SECTION + hauteurTexte) < event.button.y) && i < NOMBRESECTION + 1)
                    i++;

                if(i == 7)
                    i = 0;

                else if((longueur[i - 1] + ecran->w / 2) > event.button.x && ecran->w / 2 - longueur[i - 1] < event.button.x)
                    sectionChoisis = i;
            }

            default:
                break;
        }
        if(sectionChoisis > NOMBRESECTION)
            sectionChoisis = 0;
    }
    TTF_CloseFont(police);
    return sectionChoisis;
}

int manga(int sectionChoisis, int sectionManga[NOMBRE_MANGA_MAX], char mangaDispo[NOMBRE_MANGA_MAX][LONGUEUR_NOM_MANGA_MAX], int nombreChapitre)
{
    int mangaElligibles[NOMBRE_MANGA_MAX] = {0};
    /*Initilisation*/
    int mangaChoisis = 0, i = 0, nombreMangaElligible = 0, bandeauInf = 0;
    SDL_Color couleurTexte = {POLICE_R, POLICE_G, POLICE_B};
    SDL_Surface *texte = NULL;
    SDL_Rect position;
    TTF_Font *police = NULL;

    /*Récupère les manga elligible*/
    for(i = 0; i < NOMBRE_MANGA_MAX; i++)
        mangaElligibles[i] = -1;

    for(i = 0; sectionManga[i] != 0; i++)
        if(sectionManga[i] == sectionChoisis)
            mangaElligibles[nombreMangaElligible++] = i;

    if(sectionChoisis == SECTION_DL)
        bandeauInf = 50;
    else
        bandeauInf = BANDEAU_INF_NEW;

    if(nombreMangaElligible > 0)
    {
        if(nombreMangaElligible <= MANGAPARPAGE)
        {
            i = BORDURE_SUP_SELEC_MANGA + (LARGEUR_MOYENNE_MANGA_PETIT + MINIINTERLIGNE) * (nombreMangaElligible / NBRCOLONNES + 1) + bandeauInf;
        }
        else
        {
            i = BORDURE_SUP_SELEC_MANGA + (LARGEUR_MOYENNE_MANGA_PETIT + MINIINTERLIGNE) * (MANGAPARPAGE / NBRCOLONNES + 1) + bandeauInf;
        }

        SDL_FreeSurface(ecran);
        SDL_FreeSurface(fond);
        ecran = SDL_SetVideoMode(LARGEUR, i, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
        fond = SDL_CreateRGBSurface(SDL_HWSURFACE, LARGEUR, i, 32, 0, 0 , 0, 0); //on initialise le fond
#ifdef MACOS
        SDL_FillRect(fond, NULL, SDL_Swap32(SDL_MapRGB(ecran->format, FOND_R, FOND_G, FOND_B))); //We change background color
#else
        SDL_FillRect(fond, NULL, SDL_MapRGB(ecran->format, FOND_R, FOND_G, FOND_B)); //We change background color
#endif
        SDL_BlitSurface(fond, NULL, ecran, NULL);
        char texteTrad[3][LONGUEURTEXTE];
        lectureTrad(texteTrad, 18);

        SDL_BlitSurface(fond, NULL, ecran, NULL);

        police = TTF_OpenFont(FONTUSED, POLICE_MOYEN);
        TTF_SetFontStyle(police, TTF_STYLE_ITALIC);
        texte = TTF_RenderText_Blended(police, texteTrad[1], couleurTexte);
        position.x = (ecran->w / 2) - (texte->w / 2);
        position.y = BORDURE_SUP_TITRE_MANGA + texte->h + INTERLIGNE_MENU;
        SDL_BlitSurface(texte, NULL, ecran, &position);
        SDL_FreeSurface(texte);

        TTF_CloseFont(police);
        police = TTF_OpenFont(FONTUSED, POLICE_GROS);

        if(sectionChoisis == SECTION_DL)
        {
            char temp[TAILLE_BUFFER];
            if(langue == 4) //Petit détail dans la syntax de la traduction allemande
                sprintf(temp, "%s %s", texteTrad[2], texteTrad[0]);
            else
                sprintf(temp, "%s %s", texteTrad[0], texteTrad[2]);
            texte = TTF_RenderText_Blended(police, temp, couleurTexte);
        }
        else
            texte = TTF_RenderText_Blended(police, texteTrad[0], couleurTexte);
        position.x = (ecran->w / 2) - (texte->w / 2);
        position.y = BORDURE_SUP_TITRE_MANGA;
        SDL_BlitSurface(texte, NULL, ecran, &position);
        SDL_FreeSurface(texte);

        SDL_Flip(ecran);
        /*Définition de l'affichage*/
        conversionAvant(mangaDispo);
        i = (BORDURE_SUP_SELEC_MANGA + (LARGEUR_MOYENNE_MANGA_GROS + MINIINTERLIGNE) * nombreMangaElligible);
        if((BORDURE_SUP_SELEC_MANGA + (LARGEUR_MOYENNE_MANGA_GROS + MINIINTERLIGNE) * nombreMangaElligible) < HAUTEUR_MAX && nombreMangaElligible < 10 && sectionChoisis!= SECTION_DL)
        {
            mangaChoisis = oldShowManga(police, mangaDispo, sectionChoisis, nombreMangaElligible, mangaElligibles);
        }
        else
        {
            /*Nouvel affichage*/
            mangaChoisis = newShowManga(mangaDispo, sectionChoisis, nombreMangaElligible, mangaElligibles, nombreChapitre, BORDURE_SUP_SELEC_MANGA);
        }

        conversionApres(mangaDispo);
    }
    else
    {
        mangaChoisis = rienALire();
        if(mangaChoisis > -2)
            mangaChoisis = -2;
    }

    restartEcran();
    TTF_CloseFont(police);
    return mangaChoisis;
}

int chapitre(char team[LONGUEUR_NOM_MANGA_MAX], char mangaSoumis[LONGUEUR_NOM_MANGA_MAX], int mode)
{
    /*Initialisations*/
    int extreme[2] = {0}, buffer = 0, i = 0, j = 0, k = 0, chapitreChoisis = 0, dernierLu = 0, nombreChapitre[NOMBRE_MANGA_MAX], canard = 0;
    char temp[TAILLE_BUFFER] = {0}, nomsChapitre[NOMBRE_CHAPITRE_MAX][LONGUEUR_NOM_MANGA_MAX];
    register FILE* checkE = NULL; //Make that stuff faster
    TTF_Font *police = NULL;
    SDL_Surface *texte = NULL;
    SDL_Color couleurTexte = {POLICE_R, POLICE_G, POLICE_B};
    SDL_Rect position;

    //Please wait
    SDL_BlitSurface(fond, NULL, ecran, NULL);
    chargement();

    char texteTrad[14][LONGUEURTEXTE];
    lectureTrad(texteTrad, 19);

    /*On convertit mangaATester*/
    for(i = 0; i < LONGUEUR_NOM_MANGA_MAX; i++)
    {
        if(mangaSoumis[i] == ' ')
            mangaSoumis[i] = '_';
    }
    for(i = 0; i < NOMBRE_CHAPITRE_MAX; i++)
    {
        for(dernierLu = 0; dernierLu < 100; dernierLu++)
            nomsChapitre[i][dernierLu] = 0;
    }
    dernierLu = 0;

    /*Recherche la ligne puis le dernier chapitre disponible du manga*/
    checkE = fopenR("mangas", "r");
    while(strcmp(temp, mangaSoumis))
    {
        crashTemp(temp, TAILLE_BUFFER);
        fscanf(checkE, "%s", temp);
    }
    fscanf(checkE, "%s %d %d", temp, &extreme[0], &extreme[1]);
    fclose(checkE);

    crashTemp(temp, TAILLE_BUFFER);
    sprintf(temp, "manga/%s/config.dat", mangaSoumis);
    checkE = fopenR(temp, "r");
    if(checkE != NULL || mode == 2)
    {
        if(checkE == NULL && mode == 2)
        {
            i = extreme[0];
            dernierLu = -1;
        }

        else if (mode != 2)
        {
            fscanf(checkE, "%d %d", &i, &extreme[1]);
            if(fgetc(checkE) != EOF)
            {
                fseek(checkE, -1, SEEK_CUR);
                fscanf(checkE, "%d", &dernierLu);
            }
            else
                dernierLu = -1;
            fclose(checkE);

            if(i < extreme[0]) //Si il y a des chapitre a supprimer
            {
                for(; i < extreme[0]; i++)
                {
                    crashTemp(temp, TAILLE_BUFFER);
                    sprintf(temp, "manga\\%s\\Chapitre_%d", mangaSoumis, i);
                    removeFolder(temp, 100);
                }
                for(; checkE == NULL; i++)
                {
                    crashTemp(temp, TAILLE_BUFFER);
                    sprintf(temp, "manga/%s/Chapitre_%d/config.dat", mangaSoumis, i);
                    checkE = fopenR(temp, "r");
                }
                fclose(checkE);
                crashTemp(temp, TAILLE_BUFFER);
                sprintf(temp, "manga/%s/config.dat", mangaSoumis);
                checkE = fopenR(temp, "w+");
                fprintf(checkE, "%d %d", i, extreme[1]);
                if(dernierLu != -1)
                    fprintf(checkE, " %d", dernierLu);
                fclose(checkE);
            }
            else
                extreme[0] = i;

            if(mode == 3)
                dernierLu = -1;
        }

        /************************************************************
        ** Génère le noms des chapitre en vérifiant leur existance **
        **    Sauf si on télécharge auquel cas, c'est l'inverse    **
        ************************************************************/
        j = 0;
        if(mode == 2 || mode == 3)
        {
            //Bouton All
            sprintf(nomsChapitre[j], "%s", texteTrad[11]);
            nombreChapitre[j] = j;
            nombreChapitre[++j] = -1;
        }
        if(dernierLu == -1)
        {
            for(i = extreme[0]; i <= extreme[1]; i++)
            {
                crashTemp(temp, TAILLE_BUFFER);
                sprintf(temp, "manga/%s/Chapitre_%d/config.dat", mangaSoumis, i);
                checkE = fopenR(temp, "r");
                if(checkE != NULL && (mode == 1 || mode == 3))
                {
                    sprintf(nomsChapitre[j], "%s %d", texteTrad[10], i);
                    nombreChapitre[j] = j;
                    nombreChapitre[++j] = -1;
                    fclose(checkE);
                }
                else if(checkE == NULL && mode == 2)
                {
                    sprintf(nomsChapitre[j], "%s %d", texteTrad[10], i);
                    nombreChapitre[j] = j;
                    nombreChapitre[++j] = -1;
                }
                else if(checkE != NULL)
                    fclose(checkE);
            }
        }
        else
        {
            for(i = extreme[1]; i >= extreme[0]; i--)
            {
                crashTemp(temp, TAILLE_BUFFER);
                sprintf(temp, "manga/%s/Chapitre_%d/config.dat", mangaSoumis, i);
                checkE = fopenR(temp, "r");
                if(checkE != NULL && (mode == 1 || mode == 3))
                {
                    sprintf(nomsChapitre[j], "%s %d", texteTrad[10], i);
                    nombreChapitre[j] = j;
                    nombreChapitre[++j] = -1;
                    fclose(checkE);
                }
                else if(checkE == NULL && mode == 2)
                {
                    sprintf(nomsChapitre[j], "%s %d", texteTrad[10], i);
                    nombreChapitre[j] = j;
                    nombreChapitre[++j] = -1;
                }
                else
                    fclose(checkE);
            }
        }

        canard = j;

        SDL_BlitSurface(fond, NULL, ecran, NULL);

        police = TTF_OpenFont(FONTUSED, POLICE_GROS);
        crashTemp(temp, TAILLE_BUFFER);

        if(j == 1 && mode == 2) //Si aucun chapitre (uniquement au DL)
        {
            TTF_SetFontStyle(police, TTF_STYLE_UNDERLINE);
            texte = TTF_RenderText_Blended(police, texteTrad[12], couleurTexte);
            position.x = ecran->w / 2 - texte->w / 2;
            position.y = ecran->h / 2 - texte->h;
            SDL_BlitSurface(texte, NULL, ecran, &position);
            texte = TTF_RenderText_Blended(police, texteTrad[13], couleurTexte);
            position.x = ecran->w / 2 - texte->w / 2;
            position.y = ecran->h / 2 + texte->h;
            SDL_BlitSurface(texte, NULL, ecran, &position);
            SDL_Flip(ecran);
            chapitreChoisis = waitEnter();
            TTF_SetFontStyle(police, TTF_STYLE_NORMAL);
        }
        else
        {
            if(j == 1)
                k = j;
            //On affiche pas le même titre en fonction de la section
            sprintf(temp, "%s %s", texteTrad[0], texteTrad[mode]);
            texte = TTF_RenderText_Blended(police, temp, couleurTexte);
            position.x = (ecran->w / 2) - (texte->w / 2);
            position.y = BORDURE_SUP_TITRE_CHAPITRE;
            SDL_BlitSurface(texte, NULL, ecran, &position);
            SDL_FreeSurface(texte);

            /*Affichage des infos sur la team*/
            crashTemp(temp, TAILLE_BUFFER);


            for(i = 0; i < LONGUEUR_NOM_MANGA_MAX; i++)
            {
                if(mangaSoumis[i] == '_')
                    mangaSoumis[i] = ' ';

                if(team[i] == '_')
                    team[i] = ' ';
            }

            sprintf(temp, "%s '%s' %s '%s'", texteTrad[6], mangaSoumis, texteTrad[7], team);

            for(i = 0; i < LONGUEUR_NOM_MANGA_MAX; i++)
            {
                if(mangaSoumis[i] == ' ')
                    mangaSoumis[i] = '_';

                if(team[i] == ' ')
                    team[i] = '_';
            }

            TTF_CloseFont(police);
            police = TTF_OpenFont(FONTUSED, POLICE_MOYEN);
            TTF_SetFontStyle(police, TTF_STYLE_UNDERLINE);

            texte = TTF_RenderText_Blended(police, temp, couleurTexte);
            position.x = (ecran->w / 2) - (texte->w / 2);
            position.y = BORDURE_SUP_INFOS_TEAM_CHAPITRE;
            SDL_BlitSurface(texte, NULL, ecran, &position);
            SDL_FreeSurface(texte);


            /*Affichage des boutons du bas, central puis gauche, puis droit*/
            position.y = HAUTEUR_BOUTONS_CHAPITRE;
            if(mode == 1 || mode == 3)
            {
                crashTemp(temp, TAILLE_BUFFER);
                if(dernierLu == -1)
                   sprintf(temp, "%s", texteTrad[8]);
                else
                    sprintf(temp, "%s %d", texteTrad[9], dernierLu);

                texte = TTF_RenderText_Blended(police, temp, couleurTexte);
                position.x = ecran->w / 2 - texte->w / 2;
                SDL_BlitSurface(texte, NULL, ecran, &position);
                SDL_FreeSurface(texte);
            }
            if(mode == 2)
                TTF_SetFontStyle(police, TTF_STYLE_NORMAL);
            crashTemp(temp, TAILLE_BUFFER);
            sprintf(temp, "%s %d", texteTrad[4], extreme[0]);
            texte = TTF_RenderText_Blended(police, temp, couleurTexte);
            position.x = BORDURE_BOUTON_LECTEUR;
            SDL_BlitSurface(texte, NULL, ecran, &position);
            SDL_FreeSurface(texte);

            crashTemp(temp, TAILLE_BUFFER);
            sprintf(temp, "%s %d", texteTrad[5], extreme[1]);
            texte = TTF_RenderText_Blended(police, temp, couleurTexte);
            position.x = ecran->w - texte->w - BORDURE_BOUTON_LECTEUR;
            SDL_BlitSurface(texte, NULL, ecran, &position);
            SDL_FreeSurface(texte);
            SDL_Flip(ecran);

            chapitreChoisis = -1;
            while(chapitreChoisis == -1)
            {
                do
                {
                    if(mode == 2)
                        nombreChapitre[0] = 0;
                    chapitreChoisis = newShowManga(nomsChapitre, SECTION_CHOISIS_CHAPITRE, canard, nombreChapitre, extreme[1], BORDURE_SUP_SELEC_MANGA);
                    if(chapitreChoisis == CODE_CLIC_LIEN_CHAPITRE)
                    {
                        /*Il nous faut le nom court du manga or, on a que le long*/
                        checkE = fopenR("repo", "r");
                        if(checkE == NULL)
                        {
                            checkE = fopenR("erreur.txt", "a+");
                            fprintf(checkE, "\"repo\" introuvable\n");
                            fclose(checkE);
                            exit(0);
                        }
                        while(strcmp(temp, team) != 0 && fgetc(checkE) != EOF)
                        {
                            fseek(checkE, -1, SEEK_CUR);
                            crashTemp(temp, TAILLE_BUFFER);
                            fscanf(checkE, "%s ", temp);
                        }
                        crashTemp(temp, TAILLE_BUFFER);
                        fscanf(checkE, "%s ", temp);
                        ouvrirSite(temp);
                    }
                }while((chapitreChoisis == CODE_CLIC_LIEN_CHAPITRE && mode != 2) || (chapitreChoisis > CODE_CLIC_LIEN_CHAPITRE && mode == 2));

                if(mode != 1 && chapitreChoisis >= 0 && nombreChapitre[0] != -1 && chapitreChoisis < CODE_CLIC_LIEN_CHAPITRE)
                {
                    chapitreChoisis--;
                    if(chapitreChoisis == 0) //Si bouton all choisis
                        nombreChapitre[0] = -1;
                }

                if(nombreChapitre[0] != -1) //Si ce n'est pas all
                {
                    j = 0;
                    if(dernierLu == -1 && chapitreChoisis > 0 && chapitreChoisis < CODE_CLIC_LIEN_CHAPITRE)
                    {
                        if(mode != 2)
                        {
                            for(i = extreme[0]; i <= extreme[1] && chapitreChoisis != j; i++)
                            {
                                crashTemp(temp, TAILLE_BUFFER);
                                sprintf(temp, "manga/%s/Chapitre_%d/config.dat", mangaSoumis, i);
                                checkE = fopenR(temp, "r");
                                if(checkE != NULL)
                                {
                                    fclose(checkE);
                                    j++;
                                }
                            }
                        }

                        else
                        {
                            for(i = extreme[0]; i <= extreme[1] && chapitreChoisis != j; i++)
                            {
                                crashTemp(temp, TAILLE_BUFFER);
                                sprintf(temp, "manga/%s/Chapitre_%d/config.dat", mangaSoumis, i);
                                checkE = fopenR(temp, "r");
                                if(checkE == NULL)
                                    j++;
                                else
                                    fclose(checkE);
                            }
                        }
                        i--;
                        if(i <= extreme[1])
                            chapitreChoisis = i;
                        else
                            chapitreChoisis = -1;
                    }

                    else if (chapitreChoisis > 0 && chapitreChoisis < CODE_CLIC_LIEN_CHAPITRE)
                    {
                        if(mode != 2)
                        {
                            for(i = extreme[1]; i >= extreme[0] && chapitreChoisis != j; i--)
                            {
                                crashTemp(temp, TAILLE_BUFFER);
                                sprintf(temp, "manga/%s/Chapitre_%d/config.dat", mangaSoumis, i);
                                checkE = fopenR(temp, "r");
                                if(checkE != NULL)
                                {
                                    fclose(checkE);
                                    j++;
                                }
                            }
                        }

                        else
                        {
                            for(i = extreme[1]; i >= extreme[0] && chapitreChoisis != j; i--)
                            {
                                crashTemp(temp, TAILLE_BUFFER);
                                sprintf(temp, "manga/%s/Chapitre_%d/config.dat", mangaSoumis, i);
                                checkE = fopenR(temp, "r");
                                if(checkE == NULL)
                                    j++;
                                else
                                    fclose(checkE);
                            }
                        }
                        if(chapitreChoisis == j)
                            chapitreChoisis = i + 1;
                        else
                            chapitreChoisis = -1;
                    }

                    else if (chapitreChoisis > CODE_CLIC_LIEN_CHAPITRE && mode != 2)
                    {
                        if(chapitreChoisis == CODE_BOUTON_1_CHAPITRE) //Premier chapitre
                            chapitreChoisis = extreme[0];
                        else if(chapitreChoisis == CODE_BOUTON_2_CHAPITRE)
                        {
                            if(mode == 3)
                                chapitreChoisis = -1;
                            else
                                chapitreChoisis = dernierLu; //Dernier lu
                        }
                        else
                            chapitreChoisis = extreme[1]; //Dernier chapitre
                    }

                    else if(chapitreChoisis >= 0)
                        chapitreChoisis--;
                }

                if(k)
                    chapitreChoisis--;
            }
        }
    }
    else
    {
        buffer = showError();
        if(buffer > -3)
            buffer = -3;
        return buffer;
    }
    TTF_CloseFont(police);
    return chapitreChoisis;
}

