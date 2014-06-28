/*********************************************
**                                          **
**           The Restorator v3.2.1          **
**           Released by 29/05/2012         **
**                                          **
**********************************************/

#include "main.h"

int menuGestion()
{
    int menu = 0;
    while(menu > -3)
    {
        menu = affichageMenuGestion();
        switch(menu)
        {
            case -2:
                menu = -3;
                break;

            case 1:
                /*Ajouter un dépot*/
                menu = ajoutRepo();

                raffraichissmenent();
                break;

            case 2:
                /*Supprimer un dépot*/
                menu = deleteRepo();

                /*Raffraichissement*/
                raffraichissmenent();
                break;

            case 3:
                /*Supprimer des mangas*/
                menu = deleteManga();

                if(menu == -5)
                    menu = -2;
                break;

            case 4:
                /*RaffraÓchissement de la BDD*/
                raffraichissmenent();
                break;

            case 5:
                /*Changer langue*/
                menu = changementLangue();
                break;

            default:
                break;
        }
    }
    restartEcran();
    return menu;
}

int changementLangue()
{
    int i = 0, j = 0, hauteurTexte = 0, longueur[NOMBRE_LANGUE] = {0};
    char menus[6 + NOMBRE_LANGUE][LONGUEURTEXTE];
    SDL_Surface *texteAAfficher = NULL;
    SDL_Rect position;
    SDL_Color couleurTexte = {POLICE_R, POLICE_G, POLICE_B};
    TTF_Font *police = NULL;
    SDL_Event event;
    FILE* fileLangue = 0;

    police = TTF_OpenFont(FONTUSED, POLICE_MOYEN);

    /*On change la taille de l'écran*/
    SDL_FreeSurface(ecran);
    SDL_FreeSurface(fond);
    ecran = SDL_SetVideoMode(LARGEUR_LANGUE, HAUTEUR_LANGUE, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    fond = SDL_CreateRGBSurface(SDL_HWSURFACE, LARGEUR_LANGUE, HAUTEUR_LANGUE, 32, 0, 0 , 0, 0); //on initialise le fond
#ifdef MACOS
	SDL_FillRect(fond, NULL, SDL_Swap32(SDL_MapRGB(ecran->format, FOND_R, FOND_G, FOND_B))); //We change background color
#else
	SDL_FillRect(fond, NULL, SDL_MapRGB(ecran->format, FOND_R, FOND_G, FOND_B)); //We change background color
#endif

    lectureTrad(menus, 13);


    /*On lance la boucle d'affichage*/
    SDL_BlitSurface(fond, NULL, ecran, NULL);

    texteAAfficher = TTF_RenderText_Blended(police, menus[0], couleurTexte);
    position.x = ecran->w / 2 - texteAAfficher->w / 2;
    position.y = HAUTEUR_MENU_LANGUE;
    SDL_BlitSurface(texteAAfficher, NULL, ecran, &position);
    SDL_FreeSurface(texteAAfficher);

    TTF_SetFontStyle(police, TTF_STYLE_ITALIC);

    texteAAfficher = TTF_RenderText_Blended(police, menus[1], couleurTexte);
    position.x = ecran->w / 2 - texteAAfficher->w / 2;
    position.y = position.y + (LARGEUR_MOYENNE_MANGA_GROS + INTERLIGNE_LANGUE);
    SDL_BlitSurface(texteAAfficher, NULL, ecran, &position);
    SDL_FreeSurface(texteAAfficher);

    /*On prend un point de départ*/
    position.y = HAUTEUR_TEXTE_LANGUE;
    position.x = BORDURE_VERTICALE_SECTION;

    TTF_SetFontStyle(police, TTF_STYLE_UNDERLINE);
    for(i = 2; i < 8; i++)
    {
        /*Si il y a quelque chose a écrire*/
        if(menus[i][0] != 0)
        {
            texteAAfficher = TTF_RenderText_Blended(police, menus[i], couleurTexte);
            position.x = ecran->w / 2 - texteAAfficher->w / 2;
            SDL_BlitSurface(texteAAfficher, NULL, ecran, &position);
            hauteurTexte = texteAAfficher->h;
            longueur[i - 3] = texteAAfficher->w;
            SDL_FreeSurface(texteAAfficher);
        }
        position.y = position.y + (LARGEUR_MOYENNE_MANGA_GROS + INTERLIGNE_LANGUE);
    }
    TTF_SetFontStyle(police, TTF_STYLE_NORMAL);

    for(; i < 6 + NOMBRE_LANGUE; i++)
    {
        /*Si il y a quelque chose a écrire*/
        if(menus[i][0] != 0)
        {
            texteAAfficher = TTF_RenderText_Blended(police, menus[i], couleurTexte);
            position.x = ecran->w / 2 - texteAAfficher->w / 2;
            SDL_BlitSurface(texteAAfficher, NULL, ecran, &position);
            SDL_FreeSurface(texteAAfficher);
        }
        position.y = position.y + (LARGEUR_MOYENNE_MANGA_GROS + INTERLIGNE_LANGUE);
    }

    SDL_Flip(ecran);
    /*On attend enter ou un autre evenement*/

    j = 0;
    while(j == 0)
    {
        event.type = -1;
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                j = -4;
                break;

            case SDL_KEYDOWN: //If a keyboard letter is pushed
            {
                j = nombreEntree(event);
                switch(event.key.keysym.sym)
                {
                    case SDLK_DELETE:
                        j = -2;
                        break;

                    case SDLK_ESCAPE:
                        j = -3;
                        break;

                    default: //If other one
                        break;
                }
                if(j == -1)
                    j = 0;
                break;
            }

            case SDL_MOUSEBUTTONUP:
            {
                i = 0;
                while(((((hauteurTexte + INTERLIGNE_LANGUE) * i + HAUTEUR_TEXTE_LANGUE) > event.button.y) || ((hauteurTexte + INTERLIGNE_LANGUE) * i + hauteurTexte + HAUTEUR_TEXTE_LANGUE) < event.button.y) && i < NOMBRE_LANGUE + 1)
                    i++;

                if(j > NOMBRE_LANGUE)
                    j = 0;
                else if(ecran->w / 2 + longueur[i - 1] / 2 > event.button.x && ecran->w / 2 - longueur[i - 1] / 2 < event.button.x)
                    j = i;
            }

			default:
				if ((KMOD_LMETA & event.key.keysym.mod) && event.key.keysym.sym == SDLK_q)
					j = -4;
				break;
        }
        if(j > NOMBRE_LANGUE)
            j = 0;
    }

    if(j > 0)
    {
        langue = j;
        fileLangue = fopenR("data/langue", "w+");
        fprintf(fileLangue, "%d", langue);
        fclose(fileLangue);
        switch(langue)
        {
            case 1:
                /*Fran¡ais*/
                SDL_WM_SetCaption("The Restorator - Le lecteur de manga code par Taiki =) - v3.2.1", NULL); //Change windows name
                break;

            case 2:
                /*Anglais*/
                SDL_WM_SetCaption("The Restorator - The best manga reader ever coded by Taiki =) - v3.2.1", NULL); //Change windows name
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
                /*Fran¡ais*/
                SDL_WM_SetCaption("The Restorator - Le lecteur de manga code par Taiki =) - v3.2.1", NULL); //Change windows name
                break;
        }
        return 0;
    }
    TTF_CloseFont(police);
    return j;
}

int ajoutRepo()
{
    int i = 0, j = 0, continuer = 0, existant = 0, erreur = 0;
    char teamLong[LONGUEUR_NOM_MANGA_MAX], teamCourt[LONGUEUR_COURT], mode[5], URL[LONGUEUR_URL], ID[LONGUEUR_ID_MAX], temp[TAILLE_BUFFER];
    char site[LONGUEUR_SITE];
    FILE* test = NULL;
    FILE* buffer = NULL;
    SDL_Surface *texte;
    TTF_Font *police = NULL;
    SDL_Rect position;
    police = TTF_OpenFont(FONTUSED, POLICE_GROS);
    SDL_Color couleurTexte = {POLICE_R, POLICE_G, POLICE_NEW_B};

    SDL_FreeSurface(ecran);
    SDL_FreeSurface(fond);
    ecran = SDL_SetVideoMode(LARGEUR, HAUTEUR_FENETRE_AJOUT_REPO_INIT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    fond = SDL_CreateRGBSurface(SDL_HWSURFACE, LARGEUR, HAUTEUR_FENETRE_AJOUT_REPO_INIT, 32, 0, 0 , 0, 0); //on initialise le fond
#ifdef MACOS
	SDL_FillRect(fond, NULL, SDL_Swap32(SDL_MapRGB(ecran->format, FOND_R, FOND_G, FOND_B))); //We change background color
#else
	SDL_FillRect(fond, NULL, SDL_MapRGB(ecran->format, FOND_R, FOND_G, FOND_B)); //We change background color
#endif
	SDL_BlitSurface(fond, NULL, ecran, NULL);

    char texteTrad[5][LONGUEURTEXTE];
    lectureTrad(texteTrad, 14);


    texte = TTF_RenderText_Blended(police, texteTrad[0], couleurTexte);
    position.x = ecran->w / 2 - texte->w / 2;
    position.y = ecran->h / 2 - texte->h / 2;
    SDL_BlitSurface(texte, NULL, ecran, &position);
    SDL_FreeSurface(texte);
    SDL_Flip(ecran);
    if(NETWORK_ACCESS < CONNEXION_DOWN)
    {
        crashTemp(temp, TAILLE_BUFFER);
        /*Lecture du fichier*/
        while(!continuer)
        {
            TTF_CloseFont(police);
            police = TTF_OpenFont(FONTUSED, POLICE_PETIT);

            if(ecran->w != HAUTEUR_FENETRE_AJOUT_REPO_INIT)
            {
                /*Redimension de la fenÍtre, petite optimisation*/
                SDL_FreeSurface(ecran);
                SDL_FreeSurface(fond);
                ecran = SDL_SetVideoMode(LARGEUR, HAUTEUR_FENETRE_AJOUT_REPO_INIT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
                fond = SDL_CreateRGBSurface(SDL_HWSURFACE, LARGEUR, HAUTEUR_FENETRE_AJOUT_REPO_INIT, 32, 0, 0 , 0, 0); //on initialise le fond
    #ifdef MACOS
                SDL_FillRect(fond, NULL, SDL_Swap32(SDL_MapRGB(ecran->format, FOND_R, FOND_G, FOND_B))); //We change background color
    #else
                SDL_FillRect(fond, NULL, SDL_MapRGB(ecran->format, FOND_R, FOND_G, FOND_B)); //We change background color
    #endif
                SDL_BlitSurface(fond, NULL, ecran, NULL);
            }


            /*On affiche l'écran de sélection*/
            texte = TTF_RenderText_Blended(police, texteTrad[1], couleurTexte);
            position.x = ecran->w / 2 - texte->w / 2;
            position.y = HAUTEUR_MENU_AJOUT_REPO;
            SDL_BlitSurface(texte, NULL, ecran, &position);
            SDL_FreeSurface(texte);
            SDL_Flip(ecran);

            crashTemp(mode, 5);
            /*On attend l'ID*/
            while(strcmp(mode, "DB") != 0 && strcmp(mode, "O") != 0 && continuer >= 0)
                continuer = waitClavier(2, mode);
            if(continuer >= 0)
            {
                /*On affiche l'écran de sélection*/
                SDL_BlitSurface(fond, NULL, ecran, NULL);
                texte = TTF_RenderText_Blended(police, texteTrad[2], couleurTexte);
                position.x = ecran->w / 2 - texte->w / 2;
                position.y = HAUTEUR_MENU_AJOUT_REPO;
                SDL_BlitSurface(texte, NULL, ecran, &position);
                SDL_FreeSurface(texte);
                SDL_Flip(ecran);

                crashTemp(URL, LONGUEUR_URL);
                /*On attend l'ID*/
                continuer = waitClavier(LONGUEUR_URL, URL);
                if(!continuer)
                {
                    if(strcmp(mode, "DB") == 0)
                        sprintf(temp, "http://dl.dropbox.com/u/%s/repo", URL);

                    else if(strcmp(mode, "O") == 0)
                        sprintf(temp, "http://%s/repo", URL);

                    telechargeur(temp, "repo.new", 0);
                    test = fopenR("repo.new", "r");
                    for(i = 0; i < 5; i++)
                    {
                        if(fgetc(test) == '<')
                            erreur = 1;
                    }
                    if(!erreur && fgetc(test)!= EOF)
                    {
                        //Si on pointe sur un vrai dépôt
                        fseek(test, 0, SEEK_SET);
                        crashTemp(ID, LONGUEUR_ID_MAX);
                        crashTemp(teamLong, LONGUEUR_NOM_MANGA_MAX);
                        crashTemp(teamCourt, LONGUEUR_COURT);
                        crashTemp(mode, 5);
                        crashTemp(URL, LONGUEUR_URL);
                        crashTemp(site, LONGUEUR_SITE);
                        fscanf(test, "%s %s %s %s %s %s\n", ID, teamLong, teamCourt, mode, URL, site);
                    }
                    else
                    {
                        erreur = 1;
                        continuer = affichageRepoIconnue();
                        if(continuer >= -3)
                            continuer = -1;
                    }

                    fclose(test);
                    removeR("repo.new");

                    if(!erreur)
                    {
                        /*Redimension de la fenÍtre*/
                        SDL_FreeSurface(ecran);
                        SDL_FreeSurface(fond);
                        ecran = SDL_SetVideoMode(LARGEUR, HAUTEUR_FENETRE_AJOUT_REPO, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
                        fond = SDL_CreateRGBSurface(SDL_HWSURFACE, LARGEUR, HAUTEUR_FENETRE_AJOUT_REPO, 32, 0, 0 , 0, 0); //on initialise le fond
                        #ifdef MACOS
                        SDL_FillRect(fond, NULL, SDL_Swap32(SDL_MapRGB(ecran->format, FOND_R, FOND_G, FOND_B))); //We change background color
                        #else
                        SDL_FillRect(fond, NULL, SDL_MapRGB(ecran->format, FOND_R, FOND_G, FOND_B)); //We change background color
                        #endif
                        SDL_BlitSurface(fond, NULL, ecran, NULL);
                        texte = TTF_RenderText_Blended(police, texteTrad[3], couleurTexte);
                        position.x = ecran->w / 2 - texte->w / 2;
                        position.y = BORDURE_SUP_MENU;
                        SDL_BlitSurface(texte, NULL, ecran, &position);
                        SDL_FreeSurface(texte);

                        crashTemp(temp, TAILLE_BUFFER);
                        texte = TTF_RenderText_Blended(police, texteTrad[4], couleurTexte);
                        position.x = ecran->w / 2 - texte->w / 2;
                        position.y = BORDURE_SUP_MENU + texte->h + MINIINTERLIGNE;
                        SDL_BlitSurface(texte, NULL, ecran, &position);
                        SDL_FreeSurface(texte);

                        /*On affiche les infos*/
                        crashTemp(temp, TAILLE_BUFFER);
                        sprintf(temp, "ID: %s", ID);
                        texte = TTF_RenderText_Blended(police, temp, couleurTexte);
                        position.x = ecran->w / 2 - texte->w / 2;
                        position.y = HAUTEUR_ID_AJOUT_REPO;
                        SDL_BlitSurface(texte, NULL, ecran, &position);
                        SDL_FreeSurface(texte);
                        crashTemp(temp, TAILLE_BUFFER);

                        /*On transforme les '_' en ' '*/
                        for(j = 0; j < LONGUEUR_NOM_MANGA_MAX; j++)
                        {
                            if(teamLong[j] == '_')
                                teamLong[j] = ' ';
                        }

                        sprintf(temp, "Team: %s", teamLong);

                        /*On transforme les '_' en ' '*/
                        for(j = 0; j < LONGUEUR_NOM_MANGA_MAX; j++)
                        {
                            if(teamLong[j] == ' ')
                                teamLong[j] = '_';
                        }


                        texte = TTF_RenderText_Blended(police, temp, couleurTexte);
                        position.x = ecran->w / 2 - texte->w / 2;
                        position.y = HAUTEUR_TEAM_AJOUT_REPO;
                        SDL_BlitSurface(texte, NULL, ecran, &position);
                        SDL_FreeSurface(texte);
                        SDL_Flip(ecran);

                        j = waitEnter();
                        if(j == 1)
                        {
                            test = fopenR("repo", "r");
                            buffer = fopenR("repo.tmp", "w+");
                            existant = 0;
                            for(i = 0; fgetc(test) != '#'; i++)
                            {
                                crashTemp(temp, TAILLE_BUFFER);
                                fseek(test, -1, SEEK_CUR);

                                if((i + 1) % 6 == 0)
                                {
                                    fscanf(test, "%s\n", temp);
                                    fprintf(buffer, "%s\n", temp);
                                }
                                else
                                {
                                    fscanf(test, "%s ", temp);
                                    fprintf(buffer, "%s ", temp);
                                }
                                if(strcmp(temp, ID) == 0)
                                    existant = 1;
                            }
                            if(existant == 0)
                                fprintf(buffer, "%s %s %s %s %s %s\n#", ID, teamLong, teamCourt, mode, URL, site);

                            else
                                fprintf(buffer, "#");

                            fclose(test);
                            fclose(buffer);
                            removeR("repo");
                            renameR("repo.tmp", "repo");
                            if(existant == 0)
                            {
                                buffer = fopenR("mangas", "a+");
                                fseek(buffer, -1, SEEK_CUR);
                                fprintf(buffer, "\n%s %s\n#", teamLong, teamCourt);
                                fclose(buffer);
                            }
                            continuer = -1;
                        }
                    }
                }
                else if (continuer > -3)
                    continuer = -1;
            }
        }
    }
    TTF_CloseFont(police);
    return continuer;
}

int deleteRepo()
{
    int i = 0, j = 0, continuer = 0, mangaElligibles[NOMBRE_MANGA_MAX], teamChoisis = 0, nombreTeam = 0;
    int confirme = 0;
    char team[NOMBRE_MANGA_MAX][LONGUEUR_NOM_MANGA_MAX];
    INFOS_TEAMS teams_global[NOMBRE_MANGA_MAX];
    FILE* repo = NULL;
    FILE* repoNew = NULL;

    /*Initialisateurs graphique*/
    SDL_Surface *texteAffiche = NULL;
    SDL_Rect position;
    TTF_Font *police;
    SDL_Color couleur = {POLICE_R, POLICE_G, POLICE_B};
    police = TTF_OpenFont(FONTUSED, POLICE_GROS);

    /**************************************************************
    ***     On va commencer par récuperer les noms de teams     ***
    ***  On ne va pas actualiser (ou alors aprËs et en profiter ***
    ***           pour jarter les mangas de la team)            ***
    **************************************************************/

    repo = fopenR("repo", "r");
    for(i = 0; fgetc(repo) != '#'; i++)
    {
        fseek(repo, -1, SEEK_CUR);
        fscanf(repo, "%s %s %s %s %s %s\n", teams_global[i].ID, teams_global[i].teamLong, teams_global[i].teamCourt, teams_global[i].mode, teams_global[i].URL, teams_global[i].site);
        strcpy(team[i], teams_global[i].teamLong);
    }
    fclose(repo);
    nombreTeam = i;

    for(i = 0; i < NOMBRE_MANGA_MAX; i++)
    {
        for(j = 0; j < LONGUEUR_NOM_MANGA_MAX; j++)
        {
            if(team[i][j] == '_')
                team[i][j] = ' ';
        }
    }

    /*On met 5 pour chaque nom de team puis on complËte avec un -1 (signal de fin)*/
    for(j = 0; j < nombreTeam; j++)
        mangaElligibles[j] = 5;
    mangaElligibles[j] = -1;

    /*On va changer la taille de la fenetre en pompant l'algorithme de la selection de manga*/
    if(nombreTeam <= MANGAPARPAGE)
    {
        i = BORDURE_SUP_SELEC_MANGA + (LARGEUR_MOYENNE_MANGA_PETIT + MINIINTERLIGNE) * (nombreTeam / NBRCOLONNES + 1) + 50;
    }
    else
    {
        i = BORDURE_SUP_SELEC_MANGA + (LARGEUR_MOYENNE_MANGA_PETIT + MINIINTERLIGNE) * (MANGAPARPAGE / NBRCOLONNES + 1) + 50;
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

    char texteTrad[2][LONGUEURTEXTE];
    lectureTrad(texteTrad, 15);

    texteAffiche = TTF_RenderText_Blended(police, texteTrad[0], couleur);
    position.y = HAUTEUR_TEXTE;
    position.x = ecran->w / 2 - texteAffiche->w / 2;
    SDL_BlitSurface(texteAffiche, NULL, ecran, &position);
    SDL_FreeSurface(texteAffiche);
    SDL_Flip(ecran);

    teamChoisis = newShowManga(team, 5, nombreTeam, mangaElligibles, 0, BORDURE_SUP_SELEC_MANGA);

    if(teamChoisis > -3 && team[teamChoisis - 1][0] != 0)
    {
        SDL_FreeSurface(ecran);
        SDL_FreeSurface(fond);
        ecran = SDL_SetVideoMode(LARGEUR, HAUTEUR_DEL_REPO, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
        fond = SDL_CreateRGBSurface(SDL_HWSURFACE, LARGEUR, HAUTEUR_DEL_REPO, 32, 0, 0 , 0, 0); //on initialise le fond
#ifdef MACOS
		SDL_FillRect(fond, NULL, SDL_Swap32(SDL_MapRGB(ecran->format, FOND_R, FOND_G, FOND_B))); //We change background color
#else
		SDL_FillRect(fond, NULL, SDL_MapRGB(ecran->format, FOND_R, FOND_G, FOND_B)); //We change background color
#endif
        SDL_BlitSurface(fond, NULL, ecran, NULL);

        confirme = confirmation(team[teamChoisis - 1]);

        if(confirme)
        {
            repoNew = fopenR("repo.del", "w+");
            for(i = 0; i < nombreTeam; i++)
            {
                if(i != teamChoisis - 1)
                    fprintf(repoNew, "%s %s %s %s %s %s\n", teams_global[i].ID, teams_global[i].teamLong, teams_global[i].teamCourt, teams_global[i].mode, teams_global[i].URL, teams_global[i].site);
            }
            fprintf(repoNew, "#");
            fclose(repoNew);
            removeR("repo");
            renameR("repo.del", "repo");
        }
        else if (confirme == -4)
            return -4;
    }
    else
        continuer = teamChoisis;

    return continuer;
}

int deleteManga()
{
    /*Cette fonction va pomper comme un porc dans le module de selection de manga du lecteur*/
    int continuer = -1, mangaChoisis = 0, chapitreChoisis = -1, retourLecteur = 0, i = 0, j = 0, k = 0, premierChapitreDispo[NOMBRE_MANGA_MAX];
    int categorie[NOMBRE_MANGA_MAX] = {0}, dernierChapitreDispo[NOMBRE_MANGA_MAX] = {0};
    char mangaDispo[NOMBRE_MANGA_MAX][LONGUEUR_NOM_MANGA_MAX], teamsLong[NOMBRE_MANGA_MAX][LONGUEUR_NOM_MANGA_MAX];
    char mangaDispoCourt[NOMBRE_MANGA_MAX][LONGUEUR_COURT], teamsCourt[NOMBRE_MANGA_MAX][LONGUEUR_COURT];
    char temp[TAILLE_BUFFER];
    FILE* test = NULL;
    FILE* GlaDOS = NULL;

    miseEnCache(mangaDispo, mangaDispoCourt, categorie, premierChapitreDispo, dernierChapitreDispo, teamsLong, teamsCourt, 2);
    for(retourLecteur = 0; mangaDispo[retourLecteur][0] != 0; retourLecteur++)
    {
        crashTemp(temp, TAILLE_BUFFER);
        sprintf(temp, "manga/%s/config.dat", mangaDispo[retourLecteur]);
        test = fopenR(temp, "r");
        if(test != NULL)
        {
            fclose(test);
            categorie[retourLecteur] = 3;
        }
        else
            categorie[retourLecteur] = 1;
    }

    /*C/C du choix de manga pour le lecteur.*/
    while((continuer > -2 && continuer < 1)|| continuer == 2 ||continuer == 4)
    {
        retourLecteur = 0;
        mangaChoisis = 0;
        chapitreChoisis = 0;

        /*Appel des selectionneurs*/
        mangaChoisis = manga(3, categorie, mangaDispo, 0);

        if(mangaChoisis <= -2)
            continuer = mangaChoisis;
        if(mangaChoisis > -1)
        {
            retourLecteur = checkProjet(mangaDispo[mangaChoisis]);
            if(retourLecteur < -1)
            {
                continuer = retourLecteur;
            }
            else if(retourLecteur == 1)
            {
                chapitreChoisis = -1;
                continuer = 0;
                while(chapitreChoisis > -2 && continuer == 0)
                {
                    chapitreChoisis = chapitre(teamsLong[mangaChoisis], mangaDispo[mangaChoisis], 3);

                    if (chapitreChoisis <= -2)
                    {
                        continuer = chapitreChoisis;
                    }
                    else if (chapitreChoisis > -1)
                    {
                        if(chapitreChoisis != 0)
                        {
                            sprintf(temp, "manga/%s/config.dat", mangaDispo[mangaChoisis]);
                            test = fopenR(temp, "r");
                            fscanf(test, "%d %d", &i , &j);
                            if(fgetc(test) != EOF)
                                fscanf(test, "%d", &k);
                            fclose(test);

                            /*i == j si il n'y a qu'un seul chapitre donc dans ce cas, on dégage tout*/
                            if(i != j)
                            {
                                test = fopenR(temp, "w+");
                                crashTemp(temp, TAILLE_BUFFER);
                                sprintf(temp, "manga\\%s\\Chapitre_%d", mangaDispo[mangaChoisis], chapitreChoisis);
                                removeFolder(temp, 100);
                                /*On édite le config.dat*/

                                /*Si on enlève le premier chapitre*/
                                if(chapitreChoisis == i)
                                {
                                    sprintf(temp, "manga/%s/Chapitre_%d/config.dat", mangaDispo[mangaChoisis], i);
                                    GlaDOS = fopenR(temp, "r");
                                    for(; i <= j && GlaDOS == NULL; i++)
                                    {
                                        crashTemp(temp, TAILLE_BUFFER);
                                        sprintf(temp, "manga/%s/Chapitre_%d/config.dat", mangaDispo[mangaChoisis], i);
                                        GlaDOS = fopenR(temp, "r");
                                    }
                                    i--;
                                    fclose(GlaDOS);
                                    if(i == j)
                                        fprintf(test, "%d %d", i, j);
                                    else
                                        fprintf(test, "%d %d", i, j);
                                }

                                else if(chapitreChoisis == j)
                                {
                                    sprintf(temp, "manga/%s/Chapitre_%d/config.dat", mangaDispo[mangaChoisis], j);
                                    GlaDOS = fopenR(temp, "r");
                                    for(; i <= j && GlaDOS == NULL; j--)
                                    {
                                        crashTemp(temp, TAILLE_BUFFER);
                                        sprintf(temp, "manga/%s/Chapitre_%d/config.dat", mangaDispo[mangaChoisis], i);
                                        GlaDOS = fopenR(temp, "r");
                                    }
                                    j++;
                                    fclose(GlaDOS);
                                    if(i == j)
                                        fprintf(test, "%d %d", i, j);
                                    else
                                        fprintf(test, "%d %d", i, j);
                                }
                                else
                                {
                                    fprintf(test, "%d %d", i, j);
                                }
                                if(k != 0)
                                    fprintf(test, " %d", k);
                                fclose(test);

                            }

                            else
                            {
                                crashTemp(temp, TAILLE_BUFFER);
                                sprintf(temp, "manga\\%s", mangaDispo[mangaChoisis]);
                                removeFolder(temp, 100);
                            }
                        }

                        else
                        {
                            crashTemp(temp, TAILLE_BUFFER);
                            sprintf(temp, "manga\\%s", mangaDispo[mangaChoisis]);
                            removeFolder(temp, 100);
                        }
                    }
                }
            }
        }

        if(continuer == -2 && chapitreChoisis == -2)
        {
            continuer = chapitreChoisis = -1;
        }
    }
    return continuer;
}

