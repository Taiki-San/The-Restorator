/*********************************************
**                                          **
**           The Restorator v3.2.1          **
**           Released by 29/05/2012         **
**                                          **
**********************************************/

#include "main.h"

int showError()
{
    int i = 0;
    char texte[7][100];
    SDL_Surface *texteAAfficher = NULL;
    SDL_Rect position;
    SDL_Color couleurTexte = {POLICE_R, POLICE_G, POLICE_B};
    TTF_Font *police = NULL;

    police = TTF_OpenFont(FONTUSED, POLICE_GROS);

    restartEcran();

    SDL_BlitSurface(fond, NULL, ecran, NULL);

    position.y = ecran->h / 2 - (MINIINTERLIGNE + LARGEUR_MOYENNE_MANGA_GROS) * 2 - (MINIINTERLIGNE + LARGEUR_MOYENNE_MANGA_GROS) / 2 - 50;

    /*Remplissage des variables*/
    lectureTrad(texte, 1);

    for(i = 0; i < 7; i++)
    {
        position.y = position.y + (LARGEUR_MOYENNE_MANGA_GROS + MINIINTERLIGNE);
        if(texte[i][0] != 0)
        {
            texteAAfficher = TTF_RenderText_Blended(police, texte[i], couleurTexte);
            position.x = (ecran->w / 2) - (texteAAfficher->w / 2);
            SDL_BlitSurface(texteAAfficher, NULL, ecran, &position);
            SDL_FreeSurface(texteAAfficher);
        }
    }

    TTF_CloseFont(police);
    SDL_Flip(ecran);
    return waitEnter();
}

int showControls()
{
    SDL_Surface *controls = NULL;
    int retour = 0;
    char temp[TAILLE_BUFFER];

    crashTemp(temp, TAILLE_BUFFER);
    sprintf(temp, "%sdata/%s/controls.png", BUNDLENAME, LANGUAGE_PATH[langue - 1]);
    controls = IMG_Load(temp);
    SDL_BlitSurface(fond, NULL, ecran, NULL);
    SDL_BlitSurface(controls, NULL, ecran, NULL);
    SDL_Flip(ecran);

    SDL_Event event;
    while(!retour)
    {
        event.type = -1;
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                retour = -4;
                break;

            case SDL_KEYDOWN: //If a keyboard letter is pushed
            {
                switch(event.key.keysym.sym)
                {
                    case SDLK_RETURN: //If return
                    case SDLK_KP_ENTER:
                        retour = 1; //quit the loop
                        break;

                    case SDLK_ESCAPE:
                        retour = -3;
                        break;

                    case SDLK_DELETE:
                        retour = -2;
                        break;

                    default: //If other one
                        break;
                }
                break;
            }
            case SDL_MOUSEBUTTONUP:
                if(event.button.x > ecran->w / 2 && event.button.y > ecran->h / 2)
                    #ifdef WINDOWS
                    ShellExecute(NULL, "open", "http://www.the-restorator.com/?page_id=19", NULL, NULL, SW_SHOWDEFAULT);
                    #else
                    {
                        char superTemp[200];
                        crashTemp(superTemp, 200);
                        #ifdef MACOS
                        sprintf(superTemp, "open http://www.the-restorator.com/?page_id=19");
                        #else
                        sprintf(superTemp, "/etc/alternatives/x-www-browser %s", "http://www.the-restorator.com/?page_id=19");
                        #endif
                        system(superTemp);
                    }
                    #endif
                else
                   retour = 1;
                break;

            default:
                if ((KMOD_LMETA & event.key.keysym.mod) && event.key.keysym.sym == SDLK_q)
                    retour = -4;
                break;
        }
    }
    return retour;
}

void showNumero(TTF_Font *police, int choix, int hauteurNum)
{
    SDL_Surface *numero = NULL, *background = NULL;
    SDL_Color couleur = {POLICE_R, POLICE_G, POLICE_B};
    SDL_Rect position;
    char buffer[5] = {0};

    sprintf(buffer, "%d", choix);
    numero = TTF_RenderText_Blended(police, buffer, couleur);

    background = SDL_CreateRGBSurface(SDL_HWSURFACE, LARGEUR, HAUTEUR_BORDURE_AFFICHAGE_NUMERO, 32, 0, 0 , 0, 0); //on initialise le fond
#ifdef MACOS
	SDL_FillRect(background, NULL, SDL_Swap32(SDL_MapRGB(ecran->format, FOND_R, FOND_G, FOND_B))); //We change background color
#else
	SDL_FillRect(background, NULL, SDL_MapRGB(ecran->format, FOND_R, FOND_G, FOND_B)); //We change background color
#endif

    position.x = 0;
    position.y = hauteurNum - (numero->h / 2);
    SDL_BlitSurface(background, NULL, ecran, &position);
    SDL_FreeSurface(background);

    position.x = (ecran->w / 2) - (numero->w / 2);
    position.y = hauteurNum - (numero->h / 2);
    SDL_BlitSurface(numero, NULL, ecran, &position);
    SDL_FreeSurface(numero);

    SDL_Flip(ecran);
}

void initialisationAffichage()
{
    int i = 0, j = 0;
    char texteAAfficher[8][100];
    SDL_Surface *texte = NULL;
    SDL_Rect position;
    SDL_Color couleurTexte = {POLICE_R, POLICE_G, POLICE_B};
    TTF_Font *police = NULL;

    SDL_BlitSurface(fond, NULL, ecran, NULL);

    for(i = 0; i < 8; i++)
    {
        for(j = 0; j < 100; j++)
            texteAAfficher[i][j] = 0;
    }

    lectureTrad(texteAAfficher, 2);

    position.y = HAUTEUR_AFFICHAGE;

    police = TTF_OpenFont(FONTUSED, POLICE_GROS);

    for(i = 0; i < 8; i++)
    {
        SDL_FreeSurface(texte);
        texte = TTF_RenderText_Blended(police, texteAAfficher[i], couleurTexte);
        if(texteAAfficher[i][0] != 0)
            position.x = (ecran->w / 2) - (texte->w / 2);
        position.y = position.y + (LARGEUR_MOYENNE_MANGA_GROS + MINIINTERLIGNE);
        SDL_BlitSurface(texte, NULL, ecran, &position);
    }
    SDL_Flip(ecran);
    SDL_FreeSurface(texte);
    TTF_CloseFont(police);
}

int erreurReseau()
{
    int i = 0, j = 0;
    char texte[2][100];
    SDL_Surface *texteAAfficher = NULL;
    SDL_Rect position;
    SDL_Color couleurTexte = {POLICE_R, POLICE_G, POLICE_B};
    TTF_Font *police = NULL;
    SDL_Event event;

    restartEcran();

    police = TTF_OpenFont(FONTUSED, POLICE_GROS);

    /*Remplissage des variables*/
    lectureTrad(texte, 24);

    /*On prend un point de départ*/
    position.y = ecran->h / 2 - 50;

    /*On lance la boucle d'affichage*/
    for(i = 0; i < 2; i++)
    {
        SDL_FreeSurface(texteAAfficher);
        texteAAfficher = TTF_RenderText_Blended(police, texte[i], couleurTexte);
        position.x = (ecran->w / 2) - (texteAAfficher->w / 2);
        position.y = position.y + (LARGEUR_MOYENNE_MANGA_GROS + MINIINTERLIGNE);
        SDL_BlitSurface(texteAAfficher, NULL, ecran, &position);
    }

    TTF_CloseFont(police);

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

            case SDL_KEYDOWN:
                j = 1;
                break;

            case SDL_MOUSEBUTTONDOWN:
                j = 1;
                break;

			default:
				if ((KMOD_LMETA & event.key.keysym.mod) && event.key.keysym.sym == SDLK_q)
					j = -4;
				break;
        }
    }
    return j;
}

int affichageMenuGestion()
{
    int i = 0, j = 0, longueur[NOMBRE_MENU] = {0};
    char menus[8][LONGUEURTEXTE];
    SDL_Event event;
    SDL_Surface *texteAffiche = NULL;
    SDL_Rect position;
    TTF_Font *police;
    SDL_Color couleur = {POLICE_R, POLICE_G, POLICE_B};
    police = TTF_OpenFont(FONTUSED, POLICE_MOYEN);

    SDL_FreeSurface(ecran);
    SDL_FreeSurface(fond);
    ecran = SDL_SetVideoMode(LARGEUR, HAUTEUR_SELECTION_REPO, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    fond = SDL_CreateRGBSurface(SDL_HWSURFACE, LARGEUR, HAUTEUR_SELECTION_REPO, 32, 0, 0 , 0, 0); //on initialise le fond
#ifdef MACOS
	SDL_FillRect(fond, NULL, SDL_Swap32(SDL_MapRGB(ecran->format, FOND_R, FOND_G, FOND_B))); //We change background color
#else
	SDL_FillRect(fond, NULL, SDL_MapRGB(ecran->format, FOND_R, FOND_G, FOND_B)); //We change background color
#endif

    for(i = 0; i < 8; i++)
    {
        for(j = 0; j < LONGUEURTEXTE; j++)
            menus[i][j] = 0;
    }

    /*Remplissage des variables*/
    lectureTrad(menus, 3);

    SDL_BlitSurface(fond, NULL, ecran, NULL);
    position.y = HAUTEUR_TEXTE;
    TTF_SetFontStyle(police, TTF_STYLE_ITALIC);
    for(i = 0; i < 7; i++)
    {
        if(i == 2)
        {
            TTF_CloseFont(police);
            police = TTF_OpenFont(FONTUSED, POLICE_GROS);
            TTF_SetFontStyle(police, TTF_STYLE_UNDERLINE);
        }
        if(menus[i][0] != 0)
        {
            texteAffiche = TTF_RenderText_Blended(police, menus[i], couleur);

            /*On centre le menu*/
            position.x = ecran->w / 2 - texteAffiche->w / 2;

            if(i > 1)
                longueur[i - 2] = texteAffiche->w;
            SDL_BlitSurface(texteAffiche, NULL, ecran, &position);
            SDL_FreeSurface(texteAffiche);

            /*On remet la valeur normale*/
            if(!i)
                position.y = HAUTEUR_CHOIX;
            else
                position.y = position.y + LARGEUR_MOYENNE_MANGA_GROS + MINIINTERLIGNE;
        }
    }
    TTF_CloseFont(police);
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
                break;
            }

            case SDL_MOUSEBUTTONUP:
            {
                i = 0;
                while(((((LARGEUR_MOYENNE_MANGA_GROS + MINIINTERLIGNE) * i + HAUTEUR_CHOIX) > event.button.y) || ((LARGEUR_MOYENNE_MANGA_GROS + MINIINTERLIGNE) * i + LARGEUR_MOYENNE_MANGA_GROS + HAUTEUR_CHOIX) < event.button.y) && i < NOMBRE_MENU)
                {
                    i++;
                }
                if((ecran->w / 2 - longueur[i] / 2) < event.button.x && (ecran->w / 2 + longueur[i] / 2) > event.button.x)
                    j = i + 1;
            }

			default:
				if ((KMOD_LMETA & event.key.keysym.mod) && event.key.keysym.sym == SDLK_q)
					j = -4;
				break;
        }
        if(j > NOMBRE_MENU)
            j = 0;
    }
    return j;
}

int confirmation(char team[LONGUEUR_NOM_MANGA_MAX])
{
    int confirme = 0;
    /*Initialisateurs graphique*/
    SDL_Surface *texteAffiche = NULL;
    SDL_Rect position;
    TTF_Font *police;
    SDL_Color couleur = {POLICE_R, POLICE_G, POLICE_B};
    police = NULL;

    police = TTF_OpenFont(FONTUSED, POLICE_MOYEN);
    char texte[3][100];

    /*Remplissage des variables*/
    lectureTrad(texte, 4);

    texteAffiche = TTF_RenderText_Blended(police, texte[0], couleur);
    position.x = ecran->w / 2 - texteAffiche->w / 2;
    position.y = HAUTEUR_MENU_CONFIRMATION_SUPPRESSION;
    SDL_BlitSurface(texteAffiche, NULL, ecran, &position);
    SDL_FreeSurface(texteAffiche);

    texteAffiche = TTF_RenderText_Blended(police, team, couleur);
    position.x = ecran->w / 2 - texteAffiche->w / 2;
    position.y = HAUTEUR_TEAM_CONFIRMATION_SUPPRESSION;
    SDL_BlitSurface(texteAffiche, NULL, ecran, &position);
    SDL_FreeSurface(texteAffiche);

    texteAffiche = TTF_RenderText_Blended(police, texte[2], couleur);
    position.x = ecran->w / 2 - texteAffiche->w / 2;
    position.y = HAUTEUR_CONSIGNES_CONFIRMATION_SUPPRESSION;
    SDL_BlitSurface(texteAffiche, NULL, ecran, &position);
    SDL_FreeSurface(texteAffiche);

    SDL_Flip(ecran);

    confirme = waitEnter();

    if(confirme == 1)
        /*Confirmé*/
        return 1;
    else if (confirme == -3)
        return 0;

    else if(confirme == -4)
        return -4;

    TTF_CloseFont(police);
    return 0;
}

int ecranAccueil()
{
    SDL_Surface *accueil = NULL;
    SDL_Rect position;
    int retour = 0;

    position.x = 0;
    SDL_BlitSurface(fond, NULL, ecran, NULL); //Apply colored background on the window

    char temp[TAILLE_BUFFER];
    crashTemp(temp, TAILLE_BUFFER);
    sprintf(temp, "%sdata/%s/acceuil.png", BUNDLENAME, LANGUAGE_PATH[langue - 1]);
    accueil = IMG_Load(temp);

    position.y = (HAUTEUR / 2) - (accueil->h / 2); //Center the first picture
    SDL_BlitSurface(accueil, NULL, ecran, &position); //Blit first picture on the main window
    SDL_FreeSurface(accueil);
    SDL_Flip(ecran); //Refresh screen
    networkAndVersionTest();
    retour = waitEnter();
    return retour;
}

void raffraichissmenent()
{
    /*Initialisateurs graphique*/
    SDL_Surface *texteAffiche = NULL;
    SDL_Rect position;
    TTF_Font *police;
    SDL_Color couleur = {POLICE_R, POLICE_G, POLICE_B};
    police = TTF_OpenFont(FONTUSED, POLICE_GROS);

    char texte[2][100]; // Il faut forcement un tableau en 2D
    lectureTrad(texte, 5);

    texteAffiche = TTF_RenderText_Blended(police, texte[0], couleur);

    position.x = ecran->w / 2 - texteAffiche->w / 2;
    position.y = ecran->h / 2 - texteAffiche->h / 2;
    SDL_BlitSurface(fond, NULL, ecran, NULL);
    SDL_BlitSurface(texteAffiche, NULL, ecran, &position);
    SDL_FreeSurface(texteAffiche);
    SDL_Flip(ecran);

    updateDataBase();

    TTF_CloseFont(police);
}

void affichageLancement()
{
    /*Initialisateurs graphique*/
    SDL_Surface *texteAffiche = NULL;
    SDL_Rect position;
    TTF_Font *police;
    SDL_Color couleur = {POLICE_R, POLICE_G, POLICE_B};
    police = TTF_OpenFont(FONTUSED, POLICE_GROS);

    char texte[2][100]; // Il faut forcement un tableau en 2D
    lectureTrad(texte, 6);

    texteAffiche = TTF_RenderText_Blended(police, texte[0], couleur);
    position.x = ecran->w / 2 - texteAffiche->w / 2;
    position.y = ecran->h / 2 - texteAffiche->h / 2;
    SDL_BlitSurface(fond, NULL, ecran, NULL);
    SDL_BlitSurface(texteAffiche, NULL, ecran, &position);
    SDL_FreeSurface(texteAffiche);
    SDL_Flip(ecran);

    TTF_CloseFont(police);
}

int affichageRepoIconnue()
{
    /*Initialisateurs graphique*/
    SDL_Surface *texteAffiche = NULL;
    SDL_Rect position;
    TTF_Font *police;
    SDL_Color couleur = {POLICE_R, POLICE_G, POLICE_B};
    police = TTF_OpenFont(FONTUSED, POLICE_GROS);

    char texte[2][100];
    lectureTrad(texte, 7);


    texteAffiche = TTF_RenderText_Blended(police, texte[0], couleur);

    position.x = ecran->w / 2 - texteAffiche->w / 2;
    position.y = ecran->h / 2 - texteAffiche->h;
    SDL_BlitSurface(fond, NULL, ecran, NULL);
    SDL_BlitSurface(texteAffiche, NULL, ecran, &position);
    SDL_FreeSurface(texteAffiche);

    texteAffiche = TTF_RenderText_Blended(police, texte[1], couleur);

    position.x = ecran->w / 2 - texteAffiche->w / 2;
    position.y = ecran->h / 2 + texteAffiche->h;
    SDL_BlitSurface(texteAffiche, NULL, ecran, &position);
    SDL_FreeSurface(texteAffiche);
    SDL_Flip(ecran);

    TTF_CloseFont(police);

    return waitEnter();
}

void chargement()
{
    /*Initialisateurs graphique*/
    SDL_Surface *texteAffiche = NULL;
    SDL_Rect position;
    TTF_Font *police;
    SDL_Color couleur = {POLICE_R, POLICE_G, POLICE_B};
    police = TTF_OpenFont(FONTUSED, POLICE_GROS);

    SDL_BlitSurface(fond, NULL, ecran, NULL);

    char texte[2][100];
    lectureTrad(texte, 8);

    texteAffiche = TTF_RenderText_Blended(police, texte[0], couleur);

    position.x = ecran->w / 2 - texteAffiche->w / 2;
    position.y = ecran->h / 2 - texteAffiche->h;
    SDL_BlitSurface(texteAffiche, NULL, ecran, &position);
    SDL_FreeSurface(texteAffiche);
    TTF_CloseFont(police);
    SDL_Flip(ecran);
}

int interditWhileDL()
{
    /*Initialisateurs graphique*/
    SDL_Surface *texteAffiche = NULL;
    SDL_Rect position;
    TTF_Font *police;
    SDL_Color couleur = {POLICE_R, POLICE_G, POLICE_B};
    police = TTF_OpenFont(FONTUSED, POLICE_GROS);

    int w = ecran->w, h = ecran->h, i = 0;

    SDL_FreeSurface(ecran);
    SDL_FreeSurface(fond);
    /*Fenetre*/
    ecran = SDL_SetVideoMode(LARGEUR, HAUTEUR_INTERDIT_WHILE_DL, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    fond = SDL_CreateRGBSurface(SDL_HWSURFACE, LARGEUR, HAUTEUR_INTERDIT_WHILE_DL, 32, 0, 0 , 0, 0); //on initialise le fond
#ifdef MACOS
    SDL_FillRect(fond, NULL, SDL_Swap32(SDL_MapRGB(ecran->format, FOND_R, FOND_G, FOND_B))); //We change background color
#else
    SDL_FillRect(fond, NULL, SDL_MapRGB(ecran->format, FOND_R, FOND_G, FOND_B)); //We change background color
#endif
    SDL_BlitSurface(fond, NULL, ecran, NULL);

    char texte[2][100];
    lectureTrad(texte, 9);

    texteAffiche = TTF_RenderText_Blended(police, texte[0], couleur);

    position.x = ecran->w / 2 - texteAffiche->w / 2;
    position.y = ecran->h / 2 - texteAffiche->h;
    SDL_BlitSurface(texteAffiche, NULL, ecran, &position);
    SDL_FreeSurface(texteAffiche);

    texteAffiche = TTF_RenderText_Blended(police, texte[1], couleur);

    position.x = ecran->w / 2 - texteAffiche->w / 2;
    position.y = ecran->h / 2 + texteAffiche->h;
    SDL_BlitSurface(texteAffiche, NULL, ecran, &position);
    SDL_FreeSurface(texteAffiche);
    SDL_Flip(ecran);

    TTF_CloseFont(police);

    i = waitEnter();

    SDL_FreeSurface(ecran);
    SDL_FreeSurface(fond);
    /*Fenetre*/
    ecran = SDL_SetVideoMode(w, h, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    fond = SDL_CreateRGBSurface(SDL_HWSURFACE, w, h, 32, 0, 0 , 0, 0); //on initialise le fond
#ifdef MACOS
    SDL_FillRect(fond, NULL, SDL_Swap32(SDL_MapRGB(ecran->format, FOND_R, FOND_G, FOND_B))); //We change background color
#else
    SDL_FillRect(fond, NULL, SDL_MapRGB(ecran->format, FOND_R, FOND_G, FOND_B)); //We change background color
#endif
    SDL_BlitSurface(fond, NULL, ecran, NULL);

    return i;
}

int rienALire()
{
    SDL_Surface *texteAffiche = NULL;
    SDL_Rect position;
    TTF_Font *police;
    SDL_Color couleur = {POLICE_R, POLICE_G, POLICE_B};
    police = TTF_OpenFont(FONTUSED, POLICE_GROS);

    SDL_BlitSurface(fond, NULL, ecran, NULL);

    char texte[2][100];
    lectureTrad(texte, 23);

    texteAffiche = TTF_RenderText_Blended(police, texte[0], couleur);

    position.x = ecran->w / 2 - texteAffiche->w / 2;
    position.y = ecran->h / 2 - texteAffiche->h;
    SDL_BlitSurface(texteAffiche, NULL, ecran, &position);
    SDL_FreeSurface(texteAffiche);

    texteAffiche = TTF_RenderText_Blended(police, texte[1], couleur);

    position.x = ecran->w / 2 - texteAffiche->w / 2;
    position.y = ecran->h / 2 + texteAffiche->h;
    SDL_BlitSurface(texteAffiche, NULL, ecran, &position);
    SDL_FreeSurface(texteAffiche);
    SDL_Flip(ecran);

    TTF_CloseFont(police);

    return waitEnter();
}

