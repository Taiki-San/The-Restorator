/*********************************************
**                                          **
**           The Restorator v3.2.1          **
**           Released by 29/05/2012         **
**                                          **
**********************************************/

#include "main.h"

int checkUpdate()
{
    FILE* test = NULL;
#ifdef MACOS
	if((test = fopen("uptodate", "r")) != NULL)
	{
		fclose(test);
		remove("uptodate");
	}
#endif

	test = fopenR("update/update", "r");

    if(test != NULL)
    {
        /*Application de la MaJ*/

        int i = 0, j = 0, ligne = 0;
        char files[TAILLE_BUFFER][TAILLE_BUFFER], trad[4][100];

        for(; i < TAILLE_BUFFER; i++)
        {
            for(j = 0; j < TAILLE_BUFFER; j++)
                files[i][j] = 0;
        }

        //Lecture du fichier de MaJ, protection contre les overflow
        for(; fgetc(test) != EOF && ligne < 100; ligne++)
        {
            fseek(test, -1, SEEK_CUR);
            for(i = 0; i < 100 && (j = fgetc(test)) != '\n' && j != EOF; i++)
                files[ligne][i] = j;
        }
        fclose(test);
        renameR("update/update", "update/apply"); //Evite des tÈlÈchargements en parallËle de l'update

        /*Initialisation Ècran*/
        SDL_Surface *infosAvancement = NULL;
        SDL_Rect position;
        SDL_Color couleurTexte = {POLICE_R, POLICE_G, POLICE_B};
        TTF_Font *police = NULL;
        police = TTF_OpenFont(FONTUSED, POLICE_MOYEN);

        ecran = SDL_SetVideoMode(LARGEUR, HAUTEUR_MAJ, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
        fond = SDL_CreateRGBSurface(SDL_HWSURFACE, LARGEUR, HAUTEUR_MAJ, 32, 0, 0 , 0, 0); //On initialise le fond
        #ifdef MACOS
            SDL_FillRect(fond, NULL, SDL_Swap32(SDL_MapRGB(ecran->format, FOND_R, FOND_G, FOND_B))); //We change background color
        #else
            SDL_FillRect(fond, NULL, SDL_MapRGB(ecran->format, FOND_R, FOND_G, FOND_B)); //We change background color
        #endif
        SDL_BlitSurface(fond, NULL, ecran, NULL);

        lectureTrad(trad, 12); //Chargement du texte puis Ècriture
        infosAvancement = TTF_RenderText_Blended(police, trad[0], couleurTexte);
        position.x = (ecran->w / 2) - (infosAvancement->w / 2);
        position.y = 20;
        SDL_BlitSurface(infosAvancement, NULL, ecran, &position);
        SDL_FreeSurface(infosAvancement);
        infosAvancement = TTF_RenderText_Blended(police, trad[1], couleurTexte);
        position.x = (ecran->w / 2) - (infosAvancement->w / 2);
        position.y = 20 + infosAvancement->h + MINIINTERLIGNE;
        SDL_BlitSurface(infosAvancement, NULL, ecran, &position);
        SDL_FreeSurface(infosAvancement);
        TTF_CloseFont(police);
        police = TTF_OpenFont(FONTUSED, POLICE_GROS);

        char temp[100], URL[300];

        for(i = 0; files[i][0] != 0 && i < ligne; i++)
        {
            /*TÈlÈchargement et affichage des informations*/
            crashTemp(temp, 100);
            position.y = 150;
            position.x = 0;
            SDL_BlitSurface(fond, NULL, ecran, &position);
            sprintf(temp, "%s %d %s %d", trad[2], i + 1, trad[3], ligne);
            infosAvancement = TTF_RenderText_Blended(police, temp, couleurTexte);
            position.x = (ecran->w / 2) - (infosAvancement->w / 2);
            SDL_BlitSurface(infosAvancement, NULL, ecran, &position);
            SDL_FreeSurface(infosAvancement);
            SDL_Flip(ecran);

            /*DÈfinition de l'URL*/
            crashTemp(URL, 300);
            #ifdef WINDOWS
            sprintf(URL, "http://www.%s/update/win32/%s", MAIN_SERVER_URL[0], files[i]);
            #else
                #ifdef MACOS
                    #ifdef MACOS_OLD
                    sprintf(URL, "http://www.%s/update/OSX5/%s", MAIN_SERVER_URL[0], files[i]);
                    #else
                    sprintf(URL, "http://www.%s/update/OSX6/%s", MAIN_SERVER_URL[0], files[i]);
                    #endif
                #else
                sprintf(URL, "http://www.%s/update/linux/%s", MAIN_SERVER_URL[0], files[i]);
                #endif
            #endif
            crashTemp(temp, 100);
            sprintf(temp, "%s.temp", files[i]);
            telechargeur(URL, temp, 0);
        }

        /*Application des modifications*/
        lancementModuleDL(files[ligne - 1]);
        return 1;
    }
    return 0;
}

int check()
{
    int i = 0, j = 0, cantwrite = 0, fichiersADL[NOMBRE_DE_FICHIER_A_CHECKER+1];
    char nomsATest[NOMBRE_DE_FICHIER_A_CHECKER][LONGUEUR_NOMS_DATA];
    FILE *test = NULL;

    for(; i < NOMBRE_DE_FICHIER_A_CHECKER; i++)
    {
        for(j = 0; j < LONGUEUR_NOMS_DATA; j++)
            nomsATest[i][j] = 0;
        fichiersADL[i] = 0;
    }

    /*On injecte dans nomsATest la liste de tous les fichiers a tester*/
    sprintf(nomsATest[0], "data/font.ttf");
    #ifdef WINDOWS
    sprintf(nomsATest[1], "7za.exe");
    #else
    sprintf(nomsATest[1], "data/font.ttf");
    #endif
    sprintf(nomsATest[2], "data/french/acceuil.png");
    sprintf(nomsATest[3], "data/french/bandeau.png");
    sprintf(nomsATest[4], "data/french/controls.png");
    sprintf(nomsATest[5], "data/french/localization");
    sprintf(nomsATest[6], "data/english/acceuil.png");
    sprintf(nomsATest[7], "data/english/bandeau.png");
    sprintf(nomsATest[8], "data/english/controls.png");
    sprintf(nomsATest[9], "data/english/localization");
    sprintf(nomsATest[10], "data/italian/acceuil.png");
    sprintf(nomsATest[11], "data/italian/bandeau.png");
    sprintf(nomsATest[12], "data/italian/controls.png");
    sprintf(nomsATest[13], "data/italian/localization");
    sprintf(nomsATest[14], "data/german/acceuil.png");
    sprintf(nomsATest[15], "data/german/bandeau.png");
    sprintf(nomsATest[16], "data/german/controls.png");
    sprintf(nomsATest[17], "data/german/localization");
    sprintf(nomsATest[18], "data/icone.png");
    sprintf(nomsATest[19], "repo");
    sprintf(nomsATest[20], "mangas");

    /*On test l'existance de tous les fichiers*/
    j = 0;
    for(i = 0; (i < NOMBRE_DE_FICHIER_A_CHECKER); i++)
    {
        test = fopenR(nomsATest[i], "r");
        if(test != NULL)
            fclose(test);
        else
        {
            if(!i)
                cantwrite = 1;
            else
                fichiersADL[j++] = i;
        }
    }

    if(j)
    {
        networkAndVersionTest();

        char temp[200];
        SDL_Surface *message = NULL;
        SDL_Rect position;
        SDL_Color couleur = {POLICE_R, POLICE_G, POLICE_B};
        TTF_Font *police = NULL;

        mkdirR("data");
        mkdirR("data/english");
        mkdirR("data/french");
        mkdirR("data/german");
        mkdirR("data/italian");

        /*On vas écrire un message annonçant qu'on va restaurer l'environnement
        On ne va pas utiliser les fichiers de trad car ils peuvent être corrompus*/

        if(cantwrite) //Si police absente
        {
            crashTemp(temp, 200);
            sprintf(temp, "http://www.%s/Recover/%s", MAIN_SERVER_URL[0], nomsATest[0]);
            telechargeur(temp, nomsATest[0], 0);
        }

        police = TTF_OpenFont(FONTUSED, POLICE_MOYEN);

        crashTemp(temp, 200);
        sprintf(temp, "Environement corrompu, veuillez patienter (%d fichier(s)).", j);
        message = TTF_RenderText_Blended(police, temp, couleur);
        position.x = ecran->w / 2 - message->w / 2;
        position.y = ecran->h / 2 - message->h;
        SDL_BlitSurface(message, NULL, ecran, &position);
        SDL_FreeSurface(message);

        crashTemp(temp, 200);
        sprintf(temp, "Environment corrupted, please wait (%d file(s)).", j);
        message = TTF_RenderText_Blended(police, temp, couleur);
        position.x = ecran->w / 2 - message->w / 2;
        position.y = ecran->h / 2 + message->h;
        SDL_BlitSurface(message, NULL, ecran, &position);
        SDL_FreeSurface(message);

        SDL_Flip(ecran);

        for(i = 0; i < j; i++)
        {
            if((test = fopenR(nomsATest[fichiersADL[i]], "r")) == NULL) //On confirme que le fichier est absent
            {
                crashTemp(temp, 200);
                if(fichiersADL[i] == 5 || fichiersADL[i] == 9 || fichiersADL[i] == 13 || fichiersADL[i] == 17 || fichiersADL[i] == 19 || fichiersADL[i] == 20) //Si c'est un fichier de localization
                {
                    sprintf(temp, "http://www.%s/Recover/%s.zip", MAIN_SERVER_URL[0], nomsATest[fichiersADL[i]]);
                    telechargeur(temp, nomsATest[fichiersADL[i]], 0);
                    crashTemp(temp, 200);
                    sprintf(temp, "%s.zip", nomsATest[fichiersADL[i]]);
                    renameR(nomsATest[fichiersADL[i]], temp);
                    for(cantwrite = LONGUEUR_NOMS_DATA - 1; cantwrite > 0 && nomsATest[fichiersADL[i]][cantwrite] != '/'; cantwrite--);
                    if(cantwrite > 0)
						for(cantwrite = LONGUEUR_NOMS_DATA - 2; cantwrite > 0 && nomsATest[fichiersADL[i]][cantwrite] != '/'; nomsATest[fichiersADL[i]][cantwrite--] = 0);
					nomsATest[fichiersADL[i]][cantwrite] = 0;
                    unzip(temp, nomsATest[fichiersADL[i]]);
                    removeR(temp);
                }
                else
                {
                    sprintf(temp, "http://www.%s/Recover/%s", MAIN_SERVER_URL[0], nomsATest[fichiersADL[i]]);
                    telechargeur(temp, nomsATest[fichiersADL[i]], 0);
                }
            }
            else
                fclose(test);
        }
    }

    if((test = fopenR("data/langue", "r")) == NULL)
    {
        langue = LANGUE_PAR_DEFAUT;
        test = fopenR("data/langue", "w+");
        fprintf(test, "%d", langue);
        fclose(test);
        return 1;
    }
    else
        fclose(test);
    return 0;
}

int checkProjet(char projet[LONGUEUR_NOM_MANGA_MAX])
{
    int retour = 0;
    char temp[TAILLE_BUFFER];
    SDL_Surface *image = NULL;
    SDL_Event event;
    SDL_Rect position;
    SDL_Color couleur = {POLICE_R, POLICE_G, POLICE_B};
    TTF_Font *police = NULL;
    FILE* test = NULL;

    police = TTF_OpenFont(FONTUSED, POLICE_PETIT);
    TTF_SetFontStyle(police, TTF_STYLE_UNDERLINE);

    /*Chargement arborescence*/
    crashTemp(temp, TAILLE_BUFFER);
    sprintf(temp, "manga/%s/infos.png", projet);

    test = fopenR(temp, "r");

    SDL_BlitSurface(fond, NULL, ecran, NULL);

    if(test != NULL)
    {
        fclose(test);
        /*Affichage consigne*/
        char texte[2][100];
        lectureTrad(texte, 10);

        image = TTF_RenderText_Blended(police, texte[0], couleur);
        position.x = LARGEUR / 2 - image->w / 2;
        position.y = BORDURE_HOR_LECTURE / 2 - image->h / 2;
        SDL_BlitSurface(image, NULL, ecran, &position);
        SDL_Flip(ecran);

        /*Chargement arborescence*/
        crashTemp(temp, TAILLE_BUFFER);
        sprintf(temp, "%smanga/%s/infos.png", BUNDLENAME, projet);

        image = IMG_Load(temp);
        position.x = 0;
        position.y = BORDURE_HOR_LECTURE;
        SDL_BlitSurface(image, NULL, ecran, &position);
        SDL_Flip(ecran);
        SDL_FreeSurface(image);
        TTF_CloseFont(police);

        /*Ca ressemble beaucoup au code de waitEnter et c'est normal car c'est un C/C mais un peu modifiÈ
        J'ai ÈtÈ forcÈ de le faire car il fallait qu'appuyer sur n'importe quelle touche renvoie quelque chose*/

        while(retour == 0)
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
                        case SDLK_ESCAPE:
                            retour = -3;
                            break;

                        case SDLK_DELETE:
                            retour = -2;
                            break;

                        default: //If other one
                            retour = 1;
                            break;
                    }
                    break;
                }
                case SDL_MOUSEBUTTONUP:
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
    return 1;
}

int checkLancementUpdate()
{
    FILE* test = NULL;
    test = fopenR("update/import.dat", "r");
    if(test != NULL)
    {
        if(fgetc(test) != EOF)
        {
            fclose(test);
            #ifdef WINDOWS
            removeR("data/download");
            test = fopenR("data/download", "r");

            /*Si un dl est disponible mais qu'aucune instance ne DL*/
            if(test == NULL)
                return 1;
            else
                fclose(test);
            #else
            int i = 0;
            FILE *temp = NULL;
            temp = fopenR("data/download", "r"); //Si fichier n'existe pas, test != NULL
            if(temp != NULL)
            {
                fscanf(temp, "%d", &i);
                fclose(temp);
                if(!checkPID(i))
                    return 0;
                else
                    return 1;
            }
			else
				return 1;
            #endif
        }
        else
            fclose(test);
    }
    #ifdef WINDOWS
    else
    {
        test = fopenR("data/download", "r");
        if(test != NULL)
        {
            fclose(test);
            removeR("data/download");
        }
    }
    #endif
    return 0;
}

void networkAndVersionTest()
{
    /*Cette fonction va vÈrifier si le logiciel est a jour*/

    /*Initialisation*/
    int version = 0, i = 0;
    char temp[TAILLE_BUFFER];
    FILE *test = NULL;

    /*Chargement de l'URL*/
#ifdef WINDOWS
    sprintf(temp, "http://www.%s/System/actuel.win", MAIN_SERVER_URL[0]); /*Appel sur mon serveur*/
#else
    sprintf(temp, "http://www.%s/System/actuel.unix", MAIN_SERVER_URL[0]); /*Appel sur mon serveur*/
#endif
    telechargeur(temp, "config.dat", 0);

    /*Si fichier téléchargé, on test son intÈgritÈ.
    Le fichier est sensÈ contenir 3 caractËres.
    On va donc vÈrifier. Si ce n'est pas le cas,
    il y a un problËme avec le serveur*/
    crashTemp(temp, TAILLE_BUFFER);
    test = fopenR("config.dat", "r");
    if(test != NULL)
    {
        for(i = 0; fgetc(test) != EOF && i < 15; i++);

        if(i != 3) //Pas le fichier attendu
        {
            fclose(test);
            telechargeur(MAIN_SERVER_URL[1], "config.dat", 0); //On fais un test avec google.com
            test = fopenR("config.dat", "r");
            if(test != NULL)
            {
                if(fgetc(test) != EOF)
                    NETWORK_ACCESS = CONNEXION_SERVEUR_DOWN;
                else //Si on a rien récupéré du tout (eg: pas de connexion du tout)
                    NETWORK_ACCESS = CONNEXION_DOWN;

                fclose(test);
            }
            else
                NETWORK_ACCESS = CONNEXION_DOWN;
        }
        if(i == 3) //Pas un else au cas où le proxy ai été necessaire
        {
            rewind(test);
            fscanf(test, "%d", &version);
            fclose(test);
            if(version > CURRENTVERSION)
            {
                #ifdef WINDOWS
                sprintf(temp, "http://www.%s/update/win32/%d", MAIN_SERVER_URL[0], CURRENTVERSION);
                telechargeur(temp, "uptodate.exe", 0);
                #else
                    #ifdef MACOS
                        #ifdef MACOS_OLD
                        sprintf(temp, "http://www.%s/update/OSX5/%d", MAIN_SERVER_URL[0], CURRENTVERSION);
                        #else
                        sprintf(temp, "http://www.%s/update/OSX6/%d", MAIN_SERVER_URL[0], CURRENTVERSION);
                        #endif
                    #else
                    sprintf(temp, "http://www.%s/update/linux/%d", MAIN_SERVER_URL[0], CURRENTVERSION);
                    #endif
                telechargeur(temp, "update/update", 0);
                #endif
            }
            NETWORK_ACCESS = CONNEXION_OK;
        }
        removeR("config.dat");
    }
}

int checkRestore()
{
    FILE *test = NULL;
    test = fopenR("data/laststate.dat", "r");
    if(test == NULL)
        return 0;
    fclose(test);
    return 1;
}

void anythingNew(int extremes[2], char mangaChoisis[LONGUEUR_NOM_MANGA_MAX])
{
    char temp[TAILLE_BUFFER];
    FILE* test = NULL;

    crashTemp(temp, TAILLE_BUFFER);
    sprintf(temp, "manga/%s/config.dat", mangaChoisis);
    test = fopenR(temp, "r");
    fscanf(test, "%d %d", &extremes[0], &extremes[1]);
    fclose(test);
}

int checkPID(int PID)
{
    int i = 0;
    char temp[TAILLE_BUFFER];
    FILE *test = NULL;

    crashTemp(temp, TAILLE_BUFFER);

    if(!PID)
        return 1;
    #ifdef MACOS
    sprintf(temp, "ps %d > \"%s/%sget.dat\"", PID, REPERTOIREEXECUTION, BUNDLENAME);
    system(temp);
    test = fopenR("get.dat", "r");
    if(test != NULL)
    {
        while(fgetc(test) != '\n');
        for(i = 0; i < 10 && fgetc(test) != EOF; i++);
        fclose(test);
		removeR("get.dat");
        if(i == 10) //Si le PID existe
            return 0;
        else
            return 1;
    }
    #else
	int j = 0;
    sprintf(temp, "proc/%d/cwd", PID);
    test = fopenR(temp, "r");
    if(test != NULL) //Si le PID existe
    {
        crashTemp(temp, TAILLE_BUFFER);
        for(j = 0; (i = fgetc(test)) != EOF && j < TAILLE_BUFFER; j++)
            temp[j] = i;
        fclose(test);
        if(!strcmp(temp, REPERTOIREEXECUTION))
            return 0;
    }
    else //Sinon
        return 1;
    #endif
    return 0;
}

