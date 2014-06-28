/*********************************************
**                                          **
**           The Restorator v3.2.1          **
**           Released by 29/05/2012         **
**                                          **
**********************************************/

#include "main.h"

void mainRestorator()
{
    int continuer = -1, restoringState = 0, sectionChoisis = 0, i = 0;
    FILE* test = NULL;

    /*Define screen resolution*/
    SDL_Rect** modes; //Use for have the screen resolution
    modes = SDL_ListModes(NULL, SDL_FULLSCREEN|SDL_HWSURFACE);

    if(LARGEUR_MAX > modes[0]->w)
        LARGEUR_MAX = modes[0]->w;

    if(HAUTEUR_MAX > modes[0]->h - BARRE_DES_TACHES_WINDOWS)
        HAUTEUR_MAX = modes[0]->h - BARRE_DES_TACHES_WINDOWS;

    RESOLUTION[0] = modes[0]->w;
    RESOLUTION[1] = modes[0]->h;

    switch(langue)
    {
        case 1:
            /*FranÁais*/
            SDL_WM_SetCaption("The Restorator - Le lecteur de manga concu par Taiki =) - v3.2.1", NULL); //Change windows name
            break;

        case 2:
            /*Anglais*/
            SDL_WM_SetCaption("The Restorator - The best manga reader ever made by Taiki =) - v3.2.1", NULL); //Change windows name
            break;

        case 3:
            /*Italien*/
            SDL_WM_SetCaption("The Restorator - Il miglior lettore manga di sempre sviluppato da Taiki =) - v3.2.1", NULL); //Change windows name
            break;

        case 4:
            /*Allemand*/
            SDL_WM_SetCaption("The Restorator - Der beste Manga Reader aller Zeiten, programmiert von Taiki - v3.2.1", NULL); //Change windows name
            break;

        default:
            /*Français*/
            SDL_WM_SetCaption("The Restorator - Le lecteur de manga concu par Taiki =) - v3.2.1", NULL); //Change windows name
            break;
    }
    char temp[TAILLE_BUFFER];
    crashTemp(temp, TAILLE_BUFFER);
    sprintf(temp, "SDL_VIDEO_WINDOW_POS=%d,25", RESOLUTION[0] / 2 - LARGEUR / 2);
    putenv(temp); //pour placer en x,y (ici 0,0)

    ecran = SDL_SetVideoMode(LARGEUR, HAUTEUR, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    fond = SDL_CreateRGBSurface(SDL_HWSURFACE, LARGEUR, HAUTEUR, 32, 0, 0 , 0, 0); //on initialise le fond
#ifdef MACOS
    SDL_FillRect(fond, NULL, SDL_Swap32(SDL_MapRGB(ecran->format, FOND_R, FOND_G, FOND_B))); //We change background color
#else
    SDL_FillRect(fond, NULL, SDL_MapRGB(ecran->format, FOND_R, FOND_G, FOND_B)); //We change background color
#endif
    SDL_BlitSurface(fond, NULL, ecran, NULL);


    /*Check environnement*/
    i = check();

    restoringState = checkRestore();
    continuer = ecranAccueil();

    if(i == 1 && continuer != -4)
    {
        continuer = changementLangue();
        restartEcran();
        if(continuer == -4)
            removeR("data/langue");
        else
            continuer = showControls();
    }

    while(continuer > -4)
    {
        continuer = -1;

        if(restoringState == 0)
            sectionChoisis = section();

        else
        {
            sectionChoisis = 1;
            restoringState = 0;
        }

        switch(sectionChoisis)
        {
            case -4:
                continuer = sectionChoisis;
                break;

            case 1:
            case 2:
            case 3:
            {
                continuer = mainLecture(sectionChoisis);

                #ifdef UNIX
                /*Si le fork veut se lancer*/
                if(continuer == 1337)
                {
                    mainDL();
                    continuer = -4;
                }
                #endif
                break;
            }

            case 4:
            /*Appel du telechargement de manga*/
            {
                continuer = mainChoixDL();
                break;
            }

            case 5:
                continuer = showControls();
                break;

            case 6:
            {
                test = fopenR("update/import.dat", "r");
                if(test != NULL)
                {
                    fclose(test);
                    removeR("update/import.dat");
                }
                test = fopenR("update/import.dat", "r");
                if(test == NULL)
                {
                    continuer = menuGestion();
                }
                else
                {
                    fclose(test);
                    continuer = interditWhileDL();
                }
                break;
            }

            default:
                break;
        }
    }
}

int mainLecture(int sectionChoisis)
{
    int continuer = -1, buffer = 0, mangaChoisis = 0, chapitreChoisis = -1, retourLecteur = 0, restoringState = 0, i = 0, fullscreen = 0;
    int categorie[NOMBRE_MANGA_MAX], dernierChapitreDispo[NOMBRE_MANGA_MAX], premierChapitreDispo[NOMBRE_MANGA_MAX];
    char mangaDispo[NOMBRE_MANGA_MAX][LONGUEUR_NOM_MANGA_MAX], teamsLong[NOMBRE_MANGA_MAX][LONGUEUR_NOM_MANGA_MAX];
    char mangaDispoCourt[NOMBRE_MANGA_MAX][LONGUEUR_COURT], teamsCourt[NOMBRE_MANGA_MAX][LONGUEUR_COURT];
    FILE* test = NULL;


    restoringState = checkRestore();

    for(i = 0; i < NOMBRE_MANGA_MAX; i++)
    {
        for(buffer = 0; buffer < LONGUEUR_NOM_MANGA_MAX; buffer++)
        {
            mangaDispo[i][buffer] = 0;
            teamsLong[i][buffer] = 0;
        }
        for(buffer = 0; buffer < LONGUEUR_COURT; buffer++)
        {
            mangaDispoCourt[i][buffer] = 0;
            teamsCourt[i][buffer] = 0;
        }
    }
    buffer = 0;

    while(continuer > -3)
    {
        mangaChoisis = 0;
        chapitreChoisis = 0;

        miseEnCache(mangaDispo, mangaDispoCourt, categorie, premierChapitreDispo, dernierChapitreDispo, teamsLong, teamsCourt, 1);

        /*Appel des selectionneurs*/
        if(restoringState == 0)
            mangaChoisis = manga(sectionChoisis, categorie, mangaDispo, 0);

        if(mangaChoisis <= -2)
        {
            if(mangaChoisis == -2)
                continuer = -3;

            else
                continuer = mangaChoisis;
        }
        if(mangaChoisis > -1 || restoringState == 1)
        {
            if(restoringState == 0)
                retourLecteur = checkProjet(mangaDispo[mangaChoisis]);

            else
                retourLecteur = 1;

            if(retourLecteur < -1)
            {
                continuer = retourLecteur;
            }
            else if(retourLecteur == 1)
            {
                chapitreChoisis = -1;
                while(chapitreChoisis > -2 && continuer > -3)
                {
                    if(restoringState == 0)
                        chapitreChoisis = chapitre(teamsLong[mangaChoisis], mangaDispo[mangaChoisis], 1);

                    if (chapitreChoisis <= -2)
                    {
                        continuer = chapitreChoisis;
                    }
                    else
                    {
                        /*Lancement Lecteur*/
                        retourLecteur = 0;
                        while(retourLecteur == 0)
                        {
                            if(restoringState == 1)
                            {
                                char temp[LONGUEUR_NOM_MANGA_MAX];
                                crashTemp(temp, LONGUEUR_NOM_MANGA_MAX);

                                test = fopenR("data/laststate.dat", "r");
                                fscanf(test, "%s %d", temp, &chapitreChoisis);
                                fclose(test);

                                for(mangaChoisis = 0; strcmp(temp, mangaDispo[mangaChoisis]) != 0; mangaChoisis++);

                                restoringState = 0;
                            }
                            chargement();
                            retourLecteur = lecteur(&chapitreChoisis, &fullscreen, mangaDispo[mangaChoisis], teamsCourt[mangaChoisis]);

                            if(retourLecteur != 0)
                                fullscreen = 0;
                        }
                        /*On écrit le dernier chapitre lu*/
                        lastChapitreLu(mangaDispo[mangaChoisis], chapitreChoisis);

                        if(retourLecteur < -2)
                        {
                            continuer = retourLecteur;
                        }
                    }
                }
            }
        }
    }
    return continuer;
}

int mainChoixDL()
{
    int continuer = -1, buffer = 0, mangaChoisis = 0, chapitreChoisis = -1, nombreChapitre = 0, supprUsedInChapitre = 0;
    int categorie[NOMBRE_MANGA_MAX], dernierChapitreDispo[NOMBRE_MANGA_MAX], premierChapitreDispo[NOMBRE_MANGA_MAX], i = 0;
    char mangaDispo[NOMBRE_MANGA_MAX][LONGUEUR_NOM_MANGA_MAX], teamsLong[NOMBRE_MANGA_MAX][LONGUEUR_NOM_MANGA_MAX];
    char mangaDispoCourt[NOMBRE_MANGA_MAX][LONGUEUR_COURT], teamsCourt[NOMBRE_MANGA_MAX][LONGUEUR_COURT];

    FILE* test = NULL;

    for(i = 0; i < NOMBRE_MANGA_MAX; i++)
    {
        for(buffer = 0; buffer < LONGUEUR_NOM_MANGA_MAX; buffer++)
        {
            mangaDispo[i][buffer] = 0;
            teamsLong[i][buffer] = 0;
        }
        for(buffer = 0; buffer < LONGUEUR_COURT; buffer++)
        {
            mangaDispoCourt[i][buffer] = 0;
            teamsCourt[i][buffer] = 0;
        }
    }
    buffer = 0;

    mkdirR("manga");
	mkdirR("update");
    #ifdef WINDOWS
    test = fopenR("update/import.dat", "r");

    if(test != NULL)
    {
        fclose(test);
        removeR("data/coincoin");
        test = fopenR("data/coincoin", "r");
        if(test == NULL)
            removeR("update/import.dat");
    }
    test = fopenR("update/import.dat", "r");
    if(test == NULL)
	#else
	int allowed = 0;
	test = fopenR("data/coincoin", "r");
	if(test != NULL)
    {
        fscanf(test, "%d", &nombreChapitre);
        if(!checkPID(nombreChapitre))
            allowed = 1;
        nombreChapitre = 0;
    }
    if(!allowed)
    #endif
    {
        test = fopenR("update/import.dat", "w+");
        fclose(test);
        test = fopenR("data/coincoin", "w+");
        #ifndef WINDOWS
            fprintf(test, "%d", getpid());
            fclose(test);
        #endif

        SDL_BlitSurface(fond, NULL, ecran, NULL);

        initialisationAffichage();
        if(NETWORK_ACCESS < CONNEXION_DOWN)
        {
            continuer = updateDataBase();
            if(continuer != -4)
            {
                miseEnCache(mangaDispo, mangaDispoCourt, categorie, premierChapitreDispo, dernierChapitreDispo, teamsLong, teamsCourt, 2);
                for(i = 0; mangaDispo[i][0] != 0; i++)
                    categorie[i] = SECTION_DL;

                /*C/C du choix de manga pour le lecteur.*/
                while((continuer > -3 && continuer < 1) && (continuer != -2 || supprUsedInChapitre))
                {
                    mangaChoisis = 0;
                    chapitreChoisis = 0;
                    supprUsedInChapitre = 0;

                    /*Appel des selectionneurs*/
                    mangaChoisis = manga(SECTION_DL, categorie, mangaDispo, nombreChapitre);

                    if(mangaChoisis == -11 || mangaChoisis == -10)
                        continuer = -2;
                    else if(mangaChoisis < -2)
                        continuer = mangaChoisis;
                    else if(mangaChoisis == -2)
                        continuer = -3;
                    else if(mangaChoisis > -1)
                    {
                        chapitreChoisis = -1;
                        continuer = 0;
                        while(chapitreChoisis > -2 && continuer == 0)
                        {
                            chapitreChoisis = chapitre(teamsLong[mangaChoisis], mangaDispo[mangaChoisis], 2);

                            if (chapitreChoisis <= -2)
                            {
                                continuer = chapitreChoisis;
                                if(chapitreChoisis == -2)
                                    supprUsedInChapitre = 1;
                            }

                            else
                            {
                                /*Confirmation */
                                SDL_BlitSurface(fond, NULL, ecran, NULL);
                                continuer = ecritureDansImport(mangaDispo[mangaChoisis], mangaDispoCourt[mangaChoisis], chapitreChoisis, teamsCourt[mangaChoisis]);
								nombreChapitre = nombreChapitre + continuer;
                                continuer = -1;
                            }
                        }
                    }
                }
                removeR("data/coincoin");
                if(continuer == -2 && mangaChoisis == -11)
                {
                    SDL_BlitSurface(fond, NULL, ecran, NULL);
                    affichageLancement();
                    #ifdef WINDOWS
                    lancementModuleDL("The Restorator.exe");
                    #else
                        #ifdef MACOS
                        lancementModuleDL("The Restorator.app");
                        #else
                        i = lancementModuleDL("The Restorator");
                        if(i == -4)
                            continuer = i;
                        #endif
                    #endif
                }
                else
                    removeR("update/import.dat");
            }
        }
        else
            continuer = erreurReseau();
        #ifdef WINDOWS
            fclose(test);
        #endif
    }

    else
    {
        fclose(test);
        /*Fenetre*/
        SDL_FreeSurface(ecran);
        SDL_FreeSurface(fond);
        ecran = SDL_SetVideoMode(LARGEUR, HAUTEUR_INTERDIT_WHILE_DL, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
        fond = SDL_CreateRGBSurface(SDL_HWSURFACE, LARGEUR, HAUTEUR_INTERDIT_WHILE_DL, 32, 0, 0 , 0, 0); //on initialise le fond
#ifdef MACOS
		SDL_FillRect(fond, NULL, SDL_Swap32(SDL_MapRGB(ecran->format, FOND_R, FOND_G, FOND_B))); //We change background color
#else
		SDL_FillRect(fond, NULL, SDL_MapRGB(ecran->format, FOND_R, FOND_G, FOND_B)); //We change background color
#endif
        SDL_BlitSurface(fond, NULL, ecran, NULL);

        continuer = interditWhileDL();
    }
    restartEcran();
    return continuer;
}

void mainDL()
{
    int erreur = 0;
    char temp[TAILLE_BUFFER];
    FILE *BLOQUEUR = NULL;
    SDL_Rect position;

    #ifdef WINDOWS
    BLOQUEUR = fopenR("data/download", "w+");
    #else
    BLOQUEUR = fopenR("data/download", "w+");
    fprintf(BLOQUEUR, "%d", getpid());
    fclose(BLOQUEUR);
    BLOQUEUR = fopenR("data/download", "r");
    #endif

    SDL_Surface *texte = NULL;
    TTF_Font *police = NULL;
    police = TTF_OpenFont(FONTUSED, POLICE_PETIT);
    SDL_Color couleurTexte = {POLICE_R, POLICE_G, POLICE_B};

    /*On affiche la petite fenÍtre*/
    switch(langue)
    {
        case 1:
            /*FranÁais*/
            SDL_WM_SetCaption("The Restorator - Module de telechargement", NULL); //Change windows name
            break;

        case 2:
            /*Anglais*/
            SDL_WM_SetCaption("The Restorator - Download module", NULL); //Change windows name
            break;

        case 3:
            /*Italien*/
            SDL_WM_SetCaption("The Restorator - Download del modulo", NULL); //Change windows name
            break;

        case 4:
            /*Allemand*/
            SDL_WM_SetCaption("The Restorator - Download Modul", NULL); //Change windows name
            break;

        default:
            break;
    }

    putenv("SDL_VIDEO_WINDOW_POS=center"); //pour centrer la fenÍtre

    ecran = SDL_SetVideoMode(LARGEUR, HAUTEUR_FENETRE_DL, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    fond = SDL_CreateRGBSurface(SDL_HWSURFACE, LARGEUR, HAUTEUR_FENETRE_DL, 32, 0, 0 , 0, 0); //on initialise le fond
#ifdef MACOS
	SDL_FillRect(fond, NULL, SDL_Swap32(SDL_MapRGB(ecran->format, FOND_R, FOND_G, FOND_B))); //We change background color
#else
	SDL_FillRect(fond, NULL, SDL_MapRGB(ecran->format, FOND_R, FOND_G, FOND_B)); //We change background color
#endif
    SDL_BlitSurface(fond, NULL, ecran, NULL);

    /*Lancement du module de téléchargement, il est totalement autonome*/
    erreur = DLmanager();

    //Chargement de la traduction
    char texteTrad[7][LONGUEURTEXTE];
    lectureTrad(texteTrad, 16);

    SDL_BlitSurface(fond, NULL, ecran, NULL);
    if(erreur == 0)
    {
        texte = TTF_RenderText_Blended(police, texteTrad[1], couleurTexte);
        position.x = ecran->w / 2 - texte->w / 2;
        position.y = ecran->h / 2 - texte->h / 2 * 3;
        SDL_BlitSurface(texte, NULL, ecran, &position);
        SDL_FreeSurface(texte);

        texte = TTF_RenderText_Blended(police, texteTrad[2], couleurTexte);
        position.x = ecran->w / 2 - texte->w / 2;
        position.y = HAUTEUR_TEXTE_TELECHARGEMENT;
        SDL_BlitSurface(texte, NULL, ecran, &position);
        SDL_FreeSurface(texte);

        SDL_Flip(ecran);
        waitEnter();
    }
    else if (erreur > 0)
    {
        crashTemp(temp, TAILLE_BUFFER);
        sprintf(temp, "%s %d %s", texteTrad[3], erreur, texteTrad[4]);
        texte = TTF_RenderText_Blended(police, temp, couleurTexte);
        position.x = ecran->w / 2 - texte->w / 2;
        position.y = HAUTEUR_TEXTE_TELECHARGEMENT - texte->h - MINIINTERLIGNE;
        SDL_BlitSurface(texte, NULL, ecran, &position);
        SDL_FreeSurface(texte);

        texte = TTF_RenderText_Blended(police, texteTrad[5], couleurTexte);
        position.x = ecran->w / 2 - texte->w / 2;
        position.y = HAUTEUR_TEXTE_TELECHARGEMENT;
        SDL_BlitSurface(texte, NULL, ecran, &position);
        SDL_FreeSurface(texte);

        texte = TTF_RenderText_Blended(police, texteTrad[6], couleurTexte);
        position.x = ecran->w / 2 - texte->w / 2;
        position.y = HAUTEUR_TEXTE_TELECHARGEMENT + texte->h + MINIINTERLIGNE;
        SDL_BlitSurface(texte, NULL, ecran, &position);
        SDL_FreeSurface(texte);

        SDL_Flip(ecran);
        waitEnter();
    }
    TTF_CloseFont(police);

    fclose(BLOQUEUR);
    removeR("data/download");
}

