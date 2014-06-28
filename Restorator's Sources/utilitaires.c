/*********************************************
**           								**
**           The Restorator v3.2.1          **
**     		 Released by 29/05/2012 		**
**           								**
**********************************************/

#include "main.h"

void crashTemp(char *temp, int longueur)
{
    int i = 0;
    for(i = 0; i < longueur; i++)
        temp[i] = 0;
}

int nombreEntree(SDL_Event event)
{
    int choix = 0;
    switch(event.key.keysym.sym)
    {
        case SDLK_0:
        case SDLK_KP0:
#ifdef MACOS
		case SDLK_WORLD_4:
#endif
            choix = choix * 10;
            break;

        case SDLK_1:
        case SDLK_KP1:
#ifdef MACOS
		case SDLK_AMPERSAND:
#endif
            choix = choix * 10 + 1;
            break;

        case SDLK_2:
        case SDLK_KP2:
#ifdef MACOS
		case SDLK_WORLD_0:
#endif
            choix = choix * 10 + 2;
            break;

        case SDLK_3:
        case SDLK_KP3:
#ifdef MACOS
		case SDLK_QUOTEDBL:
#endif
            choix = choix * 10 + 3;
            break;

        case SDLK_4:
        case SDLK_KP4:
#ifdef MACOS
		case SDLK_QUOTE:
#endif
            choix = choix * 10 + 4;
            break;

        case SDLK_5:
        case SDLK_KP5:
#ifdef MACOS
		case SDLK_LEFTPAREN:
#endif
            choix = choix * 10 + 5;
            break;

        case SDLK_6:
        case SDLK_KP6:
#ifdef MACOS
		case SDLK_WORLD_1:
#endif
            choix = choix * 10 + 6;
            break;

        case SDLK_7:
        case SDLK_KP7:
#ifdef MACOS
		case SDLK_WORLD_3:
#endif
            choix = choix * 10 + 7;
            break;

        case SDLK_8:
        case SDLK_KP8:
#ifdef MACOS
		case SDLK_EXCLAIM:
#endif
            choix = choix * 10 + 8;
            break;

        case SDLK_9:
        case SDLK_KP9:
#ifdef MACOS
		case SDLK_WORLD_2:
#endif
            choix = choix * 10 + 9;
            break;

        default:
            choix = -1;
            break;
    }
    return choix;
}

int waitEnter()
{
    int i = 0;
    SDL_Event event;
    while(!i)
    {
        event.type = -1;
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                i = -4;
                break;

            case SDL_KEYDOWN: //If a keyboard letter is pushed
            {
                switch(event.key.keysym.sym)
                {
                    case SDLK_RETURN: //If return
                    case SDLK_KP_ENTER:
                        i = 1; //quit the loop
                        break;

                    case SDLK_ESCAPE:
                        i = -3;
                        break;

                    case SDLK_DELETE:
                        i = -2;
                        break;

                    default: //If other one
                        break;
                }
                break;
            }
            case SDL_MOUSEBUTTONUP:
                i = 1;
                break;

            default:
                if ((KMOD_LMETA & event.key.keysym.mod) && event.key.keysym.sym == SDLK_q)
                    i = -4;
                break;
        }
    }
    return i;
}

void conversionAvant(char mangaDispo[NOMBRE_MANGA_MAX][LONGUEUR_NOM_MANGA_MAX])
{
    int i = 0, j = 0;
    for(i = 0; i < NOMBRE_MANGA_MAX; i++)
    {
        for(j = 0; j < LONGUEUR_NOM_MANGA_MAX; j++)
        {
            if(mangaDispo[i][j] == '_')
                mangaDispo[i][j] = ' ';
        }
    }
}

void conversionApres(char mangaDispo[NOMBRE_MANGA_MAX][LONGUEUR_NOM_MANGA_MAX])
{
    int i = 0, j = 0;
    for(i = 0; i < NOMBRE_MANGA_MAX; i++)
    {
        for(j = 0; j < LONGUEUR_NOM_MANGA_MAX; j++)
        {
            if(mangaDispo[i][j] == ' ')
                mangaDispo[i][j] = '_';
        }
    }
}

int waitClavier(int nombreMax, char *retour)
{
    int i = 0, j = 0;
    char affiche[LONGUEUR_URL + 3];
    SDL_Event event;
    SDL_Surface *numero = NULL;
    SDL_Rect position;
    SDL_Color couleurTexte = {POLICE_R, POLICE_G, POLICE_B};
    TTF_Font *police = NULL;

    for(i = 0; i < nombreMax; i++)
        retour[i] = 0;
    if(nombreMax < 30)
        police = TTF_OpenFont(FONTUSED, POLICE_GROS);
    else
        police = TTF_OpenFont(FONTUSED, POLICE_PETIT);

    for(i = 0; i <= nombreMax; i++)
    {
        if(i == nombreMax)
            i--;
        event.type = -1;
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                SDL_FreeSurface(numero);
                TTF_CloseFont(police);
                return -4;
                break;

            case SDL_KEYDOWN:
            {
                switch(event.key.keysym.sym)
                {
                    case SDLK_RETURN: //If return
                    case SDLK_KP_ENTER:
                        i = nombreMax; //quit the loop
                        break;

                    case SDLK_BACKSPACE:
                        retour[i--] = 0;
                        retour[i--] = 0;
                        break;

                    case SDLK_ESCAPE:
                        TTF_CloseFont(police);
                        return -3;
                        break;

                    case SDLK_DELETE:
                        TTF_CloseFont(police);
                        return -2;
                        break;

                    case SDLK_0:
					case SDLK_KP0:
#ifdef MACOS
					case SDLK_WORLD_4:
#endif
						retour[i] = '0';
						break;

					case SDLK_1:
					case SDLK_KP1:
#ifdef MACOS
					case SDLK_AMPERSAND:
#endif
						retour[i] = '1';
						break;

					case SDLK_2:
					case SDLK_KP2:
#ifdef MACOS
					case SDLK_WORLD_0:
#endif
						retour[i] = '2';
						break;

					case SDLK_3:
					case SDLK_KP3:
#ifdef MACOS
					case SDLK_QUOTEDBL:
#endif
						retour[i] = '3';
						break;

					case SDLK_4:
					case SDLK_KP4:
#ifdef MACOS
					case SDLK_QUOTE:
#endif
						retour[i] = '4';
						break;

					case SDLK_5:
					case SDLK_KP5:
#ifdef MACOS
					case SDLK_LEFTPAREN:
#endif
						retour[i] = '5';
						break;

					case SDLK_6:
					case SDLK_KP6:
#ifdef MACOS
					case SDLK_WORLD_1:
#endif
						retour[i] = '6';
						break;

					case SDLK_7:
					case SDLK_KP7:
#ifdef MACOS
					case SDLK_WORLD_3:
#endif
						retour[i] = '7';
						break;

					case SDLK_8:
					case SDLK_KP8:
#ifdef MACOS
					case SDLK_EXCLAIM:
#endif
						retour[i] = '8';
						break;

					case SDLK_9:
					case SDLK_KP9:
#ifdef MACOS
					case SDLK_WORLD_2:
#endif
						retour[i] = '9';
						break;

                    case SDLK_a:
                        if(langue == 1)
                        {
                            retour[i] = 'Q';
                        }
                        else if (langue == 2)
                        {
                            retour[i] = 'A';
                        }
                        break;

                    case SDLK_b:
                        retour[i] = 'B';
                        break;

                    case SDLK_c:
                        retour[i] = 'C';
                        break;

                    case SDLK_d:
                        retour[i] = 'D';
                        break;

                    case SDLK_e:
                        retour[i] = 'E';
                        break;

                    case SDLK_f:
                        retour[i] = 'F';
                        break;

                    case SDLK_g:
                        retour[i] = 'G';
                        break;

                    case SDLK_h:
                        retour[i] = 'H';
                        break;

                    case SDLK_i:
                        retour[i] = 'I';
                        break;

                    case SDLK_j:
                        retour[i] = 'J';
                        break;

                    case SDLK_k:
                        retour[i] = 'K';
                        break;

                    case SDLK_l:
                        retour[i] = 'L';
                        break;

                    case SDLK_SEMICOLON:
                        if(langue == 1)
                            retour[i] = 'M';
                        break;

                    case SDLK_m:
                        if(langue == 2)
                            retour[i] = 'M';
                        break;

                    case SDLK_n:
                        retour[i] = 'N';
                        break;

                    case SDLK_o:
                        retour[i] = 'O';
                        break;

                    case SDLK_p:
                        retour[i] = 'P';
                        break;

                    case SDLK_q:
                        if(langue == 1)
                            retour[i] = 'A';
                        else
                            retour[i] = 'Q';
                        break;

                    case SDLK_r:
                        retour[i] = 'R';
                        break;

                    case SDLK_s:
                        retour[i] = 'S';
                        break;

                    case SDLK_t:
                        retour[i] = 'T';
                        break;

                    case SDLK_u:
                        retour[i] = 'U';
                        break;

                    case SDLK_v:
                        retour[i] = 'V';
                        break;

                    case SDLK_w:
                        if(langue == 1)
                            retour[i] = 'Z';

                        else
                            retour[i] = 'W';
                        break;

                    case SDLK_x:
                        retour[i] = 'X';
                        break;

                    case SDLK_y:
                        retour[i] = 'Y';
                        break;

                    case SDLK_z:
                        if(langue == 1)
                            retour[i] = 'W';
                        else
                            retour[i] = 'Z';
                        break;

                    default:
                        i--;
                        break;
                }
                break;
            }

            default:
                if ((KMOD_LMETA & event.key.keysym.mod) && event.key.keysym.sym == SDLK_q)
                {
                    SDL_FreeSurface(numero);
                    TTF_CloseFont(police);
                    return -4;
                }
                i--;
                break;
        }
        crashTemp(affiche, LONGUEUR_URL + 2);
        sprintf(affiche, "-> ");
        for(j = 3; j < nombreMax + 3; j++)
        {
            affiche[j] = retour[j - 3];
        }
        for(j = 0; affiche[j] != 0; j++);
        affiche[j] = ' ';
        affiche[j + 1] = '<';
        affiche[j + 2] = '-';
        numero = TTF_RenderText_Blended(police, affiche, couleurTexte);
        position.x = 0;
        position.y = ecran->h / 2;
        SDL_BlitSurface(fond, NULL, ecran, &position);
        position.y = ecran->h / 2;
        position.x = ecran->w / 2 - numero->w / 2;
        SDL_BlitSurface(numero, NULL, ecran, &position);
        SDL_FreeSurface(numero);
        SDL_Flip(ecran);
    }
    TTF_CloseFont(police);
    return 0;
}

int compare(const void *a, const void *b)
{
    const char *s1 = a;
    const char *s2 = b;
    return strcmp(s1, s2);
}

void removeFolder(char *path, int longueur)
{
    int i = 0;
    char temp[500], buffer[500];

    for(i = 0; i < longueur; i++)
        temp[i] = (char) path[i];

    crashTemp(buffer, 500);

    #ifdef WINDOWS
    int  j = 0;
    sprintf(buffer, "rmdir /S /Q \"%s\\%s\"", REPERTOIREEXECUTION, temp);
    crashTemp(temp, 500);

    for(i = 0; buffer[i] != 0; i++)
    {
        if(buffer[i] == '\\')
        {
            temp[j++] = '\\';
        }
        temp[j++] = buffer[i];
    }
    system(temp);
	#else
		#ifdef MACOS
		sprintf(buffer, "rm -rf \"%s/%s/%s\"", REPERTOIREEXECUTION, BUNDLENAME ,temp);
		#else
		sprintf(buffer, "rm -rf \"%s/%s\"", REPERTOIREEXECUTION, temp);
		system(buffer);
		#endif
	#endif
}

void lastChapitreLu(char nomManga[LONGUEUR_NOM_MANGA_MAX], int dernierChapitre)
{
    int i = 0, j = 0;
    char temp[TAILLE_BUFFER];
    FILE* fichier = NULL;

    crashTemp(temp, TAILLE_BUFFER);
    sprintf(temp, "manga/%s/config.dat", nomManga);
    fichier = fopenR(temp, "r");

    fscanf(fichier, "%d %d", &i, &j);

    fclose(fichier);
    fichier = fopenR(temp, "w+");

    fprintf(fichier, "%d %d %d", i, j, dernierChapitre);

    fclose(fichier);
}

int compteNombreDL()
{
    int i = 0, j = 1;
    FILE* test = NULL;

    test = fopenR("import.dat", "r");
    if(test != NULL)
    {
        fgetc(test);
        while(j)
        {
            if(fgetc(test) == '\n')
                i++;
            if(fgetc(test) == EOF)
                j = 0;
            else
                fseek(test, -1, SEEK_CUR);

        }
    }
    return i;
}

void restartEcran()
{
    SDL_FreeSurface(ecran);
    SDL_FreeSurface(fond);
    ecran = SDL_SetVideoMode(LARGEUR, HAUTEUR, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    fond = SDL_CreateRGBSurface(SDL_HWSURFACE, LARGEUR, HAUTEUR, 32, 0, 0 , 0, 0); //on initialise le fond
#ifdef MACOS
    SDL_FillRect(fond, NULL, SDL_Swap32(SDL_MapRGB(ecran->format, FOND_R, FOND_G, FOND_B))); //We change background color
#else
    SDL_FillRect(fond, NULL, SDL_MapRGB(ecran->format, FOND_R, FOND_G, FOND_B)); //We change background color
#endif
    SDL_BlitSurface(fond, NULL, ecran, NULL);
}

void ouvrirSite(char team[LONGUEURMANGA])
{
    int continuer = 1;
    char temp[LONGUEUR_SITE];
    FILE* ressources = NULL;

    ressources = fopenR("repo", "r");
    crashTemp(temp, LONGUEUR_SITE);

    while(fgetc(ressources) != EOF && continuer)
    {
        while(strcmp(temp, team) != 0)
        {
            crashTemp(temp, LONGUEUR_SITE);
            fscanf(ressources, "%s", temp);
        }
        fscanf(ressources, "%s", temp); //On passe le type de dépôt
        fscanf(ressources, "%s", temp); //On passe l'accès
        crashTemp(temp, LONGUEUR_SITE); //On réinitialise
        fscanf(ressources, "%s", temp); //On charge l'URL
        continuer = 0;
    }

    #ifdef WINDOWS
    ShellExecute(NULL, "open", temp, NULL, NULL, SW_SHOWDEFAULT);
    #else
        char superTemp[200];
        crashTemp(superTemp, 200);
        #ifdef MACOS
        sprintf(superTemp, "open %s", temp);
        #else
        sprintf(superTemp, "firefox %s", temp);
        #endif
        system(superTemp);
    #endif
}

void killswitchEnabled(char nomTeamCourt[5])
{
    //Cette fonction est appelé si le killswitch est activé, elle recoit un nom de team, cherche ses mangas dans mangas et fais le ménage
    int i = 1;
    char temp[TAILLE_BUFFER], buffer[TAILLE_BUFFER];
    FILE *mangas = fopenR("mangas", "r");
    FILE *test = NULL;

    if(mangas != NULL)
    {
        while(i)
        {
            fscanf(mangas, "%s %s", buffer, temp);
            if(strcmp(temp, nomTeamCourt))
                while(fgetc(mangas) != '#');
            else
                i = 0;
        }
        crashTemp(temp, TAILLE_BUFFER);
        while(fgetc(mangas)!= '#')
        {
            fseek(mangas, -1, SEEK_CUR);
            fscanf(mangas, "%s %s %d %d %d %d\n", temp, buffer, &i, &i, &i, &i); //Seul temp est utilisé
            crashTemp(buffer, TAILLE_BUFFER);
            sprintf(buffer, "manga/%s/config.dat", temp);
            test = fopenR("buffer", "r");
            if(test != NULL)
            {
                fclose(test);
                crashTemp(buffer, TAILLE_BUFFER);
                sprintf(buffer, "manga/%s", temp);
                removeFolder(buffer, TAILLE_BUFFER);
            }
        }
    }
    fclose(mangas);
}

int getLetterPushed(SDL_Event event)
{
    int i;
    switch(event.key.keysym.sym)
    {
        case SDLK_a:
            if(langue == 1)
            {
                i = 'Q';
            }
            else
            {
                i = 'A';
            }
            break;

        case SDLK_b:
            i = 'B';
            break;

        case SDLK_c:
            i = 'C';
            break;

        case SDLK_d:
            i = 'D';
            break;

        case SDLK_e:
            i = 'E';
            break;

        case SDLK_f:
            i = 'F';
            break;

        case SDLK_g:
            i = 'G';
            break;

        case SDLK_h:
            i = 'H';
            break;

        case SDLK_i:
            i = 'I';
            break;

        case SDLK_j:
            i = 'J';
            break;

        case SDLK_k:
            i = 'K';
            break;

        case SDLK_l:
            i = 'L';
            break;

        case SDLK_SEMICOLON:
            if(langue == 1)
                i = 'M';
            break;

        case SDLK_m:
            if(langue == 2)
                i = 'M';
            break;

        case SDLK_n:
            i = 'N';
            break;

        case SDLK_o:
            i = 'O';
            break;

        case SDLK_p:
            i = 'P';
            break;

        case SDLK_q:
            if(langue == 1)
            {
                i = 'A';
            }
            else
            {
                i = 'Q';
            }
            break;

        case SDLK_r:
            i = 'R';
            break;

        case SDLK_s:
            i = 'S';
            break;

        case SDLK_t:
            i = 'T';
            break;

        case SDLK_u:
            i = 'U';
            break;

        case SDLK_v:
            i = 'V';
            break;

        case SDLK_w:
            if(langue == 1)
            {
                i = 'Z';
            }
            else
            {
                i = 'W';
            }
            break;

        case SDLK_x:
            i = 'X';
            break;

        case SDLK_y:
            i = 'Y';
            break;

        case SDLK_z:
            if(langue == 1)
            {
                i = 'W';
            }
            else
            {
                i = 'Z';
            }
            break;

        default:
            i = 0;
            break;
    }
    return i;
}

void unzip(char *path, char *output)
{
    char superTemp[600];
    #ifdef WINDOWS
    int i = 0, j = 0;
    char directory[400], bufferPath[500], bufferOutput[500];
    crashTemp(directory, 400);
    crashTemp(bufferPath, 500);
    crashTemp(bufferOutput, 500);

    /*On double les backslash en transférant le repertoire d'execution dans buffer1*/
    for(i = 0; REPERTOIREEXECUTION[i] != '\0'; i++)
    {
        if(REPERTOIREEXECUTION[i] == '\\')
            directory[j++] = '\\';

        directory[j++] = REPERTOIREEXECUTION[i];
    }

    j = 0;
    for(i = 0; path[i] != '\0'; i++)
    {
        if(path[i] == '/')
        {
            bufferPath[j++] = '\\';
            bufferPath[j++] = '\\';
        }
        else
            bufferPath[j++] = path[i];
    }

    j = 0;
    for(i = 0; output[i] != '\0'; i++)
    {
        if(output[i] == '/')
        {
            bufferOutput[j++] = '\\';
            bufferOutput[j++] = '\\';
        }
        else
            bufferOutput[j++] = output[i];
    }

    /*Création de la ligne de commande*/
    crashTemp(superTemp, 600);
    /***************************************************
    **        Formatage de la ligne de commande       **
    **                                                **
    **   Tout est entre guillemet puis chaque chemin  **
    **  d'accès est entre guillemet. On appelle 7zip  **
    **en mettant en argument e l'archive et -o (collé)**
    **          le repertoire de décompression        **
    **                                                **
    ***************************************************/
    sprintf(superTemp, "\"\"%s\\\\7za\" e \"%s\\\\%s\" -o\"%s\\\\%s\"\"", directory, directory, bufferPath, directory, bufferOutput);
    #else
    crashTemp(superTemp, 600);
    sprintf(superTemp, "unzip -j \"%s%s\" -d \"%s%s\"", BUNDLENAME, path, REPERTOIREEXECUTION, BUNDLENAME, output);
    #endif

    system(superTemp); //Execution de la ligne de commande
}

FILE* fopenR(char *path, char *right)
{
    char temp[400];
    crashTemp(temp, 400);
    sprintf(temp, "%s%s", BUNDLENAME, path);
    return fopen(temp, right);
}

void removeR(char *path)
{
	#ifdef MACOS
	char temp[300];
	crashTemp(temp, 300);
	sprintf(temp, "%s%s", BUNDLENAME, path);
	remove(temp);
	#else
	remove(path);
	#endif
}

void renameR(char *initialName, char *newName)
{
	#ifdef MACOS
	char temp[300], temp2[300];
	crashTemp(temp, 300);
	crashTemp(temp2, 300);
	sprintf(temp, "%s%s", BUNDLENAME, initialName);
	sprintf(temp2, "%s%s", BUNDLENAME, newName);
	rename(temp, temp2);
	#else
	rename(initialName, newName);
	#endif
}

void mkdirR(char *path)
{
    #ifdef MACOS
    char temp[500];
    crashTemp(temp, 500);
    sprintf(temp, "%s%s", BUNDLENAME, path);
    mkdir(temp, PERMISSIONS);
    #else
    mkdir(path);
    #endif
}

