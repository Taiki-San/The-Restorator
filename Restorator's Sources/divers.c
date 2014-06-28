/*********************************************
**                                          **
**           The Restorator v3.2.1          **
**           Released by 29/05/2012         **
**                                          **
**********************************************/

#include "main.h"

void miseEnCache(char mangaDispo[NOMBRE_MANGA_MAX][LONGUEUR_NOM_MANGA_MAX], char mangaDispoCourt[NOMBRE_MANGA_MAX][LONGUEUR_COURT], int categorie[NOMBRE_MANGA_MAX], int premierChapitreDispo[NOMBRE_MANGA_MAX], int dernierChapitreDispo[NOMBRE_MANGA_MAX], char teamsLong[NOMBRE_MANGA_MAX][LONGUEUR_NOM_MANGA_MAX], char teamsCourt[NOMBRE_MANGA_MAX][LONGUEUR_COURT], int mode)
{
    FILE* config = NULL;
    FILE* test = NULL;
    int i = 0, j = 0, numeroManga = 0, nombreManga = 0;
    int newTeam = 0;
    char temp[TAILLE_BUFFER];
    config = fopenR("mangas", "r");

    /*Réinitialisation des variables*/
    for(i = 0; i < NOMBRE_MANGA_MAX; i++)
    {
        for(newTeam = 0; newTeam < LONGUEUR_NOM_MANGA_MAX; newTeam++)
        {
            mangaDispo[i][newTeam] = 0;
        }

        for(newTeam = 0; newTeam < LONGUEUR_COURT; newTeam++)
        {
            mangaDispoCourt[i][newTeam] = 0;
        }

        categorie[i] = 0;
        dernierChapitreDispo[i] = 0;
    }

    if(config != NULL)
    {
        fscanf(config,"\n%s %s\n", teamsLong[0], teamsCourt[0]);
        newTeam = 1;
        for(i = 0; (fgetc(config) != EOF && i <= NOMBRE_MANGA_MAX); i++)
        {
            fseek(config, -1, SEEK_CUR);
            if(fgetc(config) == '#')
            {
                fscanf(config, "\n%s %s\n", teamsLong[numeroManga], teamsCourt[numeroManga]);
                newTeam = 1;
            }
            else
            {
                fseek(config, -1, SEEK_CUR);
                fscanf(config, "%s %s %d %d %d %d\n", mangaDispo[numeroManga], mangaDispoCourt[numeroManga], &premierChapitreDispo[numeroManga], &dernierChapitreDispo[numeroManga], &categorie[numeroManga], &j); //j est l‡ car cette valeur n'est pas utilisé pour le moment
                crashTemp(temp, TAILLE_BUFFER);
                sprintf(temp, "manga/%s/config.dat", mangaDispo[numeroManga]);

                if(newTeam == 0)
                {
                    strcpy(teamsLong[numeroManga], teamsLong[numeroManga - 1]);
                    strcpy(teamsCourt[numeroManga], teamsCourt[numeroManga - 1]);
                }
                test = fopenR(temp, "r");
                if(test != NULL || mode == 2)
                {
                    if(test != NULL)
                        fclose(test);
                    numeroManga++;
                    newTeam = 0;
                }
                else
                {
                    for(j = 0; j < LONGUEUR_NOM_MANGA_MAX; j++)
                        mangaDispo[numeroManga][j] = 0;
                    for(j = 0; j < LONGUEUR_COURT; j++)
                        mangaDispoCourt[numeroManga][j] = 0;
                    dernierChapitreDispo[numeroManga] = 0;
                    categorie[numeroManga] = 0;
                    newTeam = 1;
                }
            }
        }
        fclose(config);
    }

    char bufferMangaDispo[NOMBRE_MANGA_MAX][LONGUEUR_NOM_MANGA_MAX];
    char bufferMangaDispoCourt[NOMBRE_MANGA_MAX][LONGUEUR_COURT];
    int bufferCategorie[NOMBRE_MANGA_MAX];
    int bufferDernierChapitreDispo[NOMBRE_MANGA_MAX];
    int bufferPremierChapitreDispo[NOMBRE_MANGA_MAX];
    char bufferTeamsLong[NOMBRE_MANGA_MAX][LONGUEUR_NOM_MANGA_MAX];
    char bufferTeamsCourt[NOMBRE_MANGA_MAX][LONGUEUR_COURT];
    for(i = 0; i < NOMBRE_MANGA_MAX; i++)
    {
        strcpy(bufferMangaDispo[i], mangaDispo[i]);
        strcpy(bufferMangaDispoCourt[i], mangaDispoCourt[i]);
        strcpy(bufferTeamsLong[i], teamsLong[i]);
        strcpy(bufferTeamsCourt[i], teamsCourt[i]);
        bufferCategorie[i] = categorie[i];
        bufferPremierChapitreDispo[i] = premierChapitreDispo[i];
        bufferDernierChapitreDispo[i] = dernierChapitreDispo[i];
    }
    /**Classement**/
    for(nombreManga = 0; mangaDispo[nombreManga][0] != 0; nombreManga++);

    qsort(mangaDispo, nombreManga, sizeof(mangaDispo[0]), compare);

    for(i = 0; i < nombreManga; i++)
    {
        for(j = 0; j < NOMBRE_MANGA_MAX; j++)
        {
            if(strcmp(mangaDispo[i], bufferMangaDispo[j]) == 0)
            {
                /*Copier les buffers dans les cases normales*/
                strcpy(mangaDispoCourt[i], bufferMangaDispoCourt[j]);
                strcpy(teamsLong[i], bufferTeamsLong[j]);
                strcpy(teamsCourt[i], bufferTeamsCourt[j]);
                categorie[i] = bufferCategorie[j];
                premierChapitreDispo[i] = bufferPremierChapitreDispo[j];
                dernierChapitreDispo[i] = bufferDernierChapitreDispo[j];
                j = NOMBRE_MANGA_MAX;
            }
        }
    }
}

int isItNew(char mangaATester[LONGUEUR_NOM_MANGA_MAX])
{
    int dernierChapDispo = 0, i = 0, j = 0;
    char buffer[100];
    FILE* test = NULL;

    test = fopenR("mangas", "r");
    crashTemp(buffer, 100);

    /*On convertit mangaATester*/
    for(i = 0; i < LONGUEUR_NOM_MANGA_MAX; i++)
    {
        if(mangaATester[i] == ' ')
            mangaATester[i] = '_';
    }

    /*Recherche la ligne puis le dernier chapitre disponible du manga*/
    while(strcmp(buffer, mangaATester))
    {
        crashTemp(buffer, 100);
        fscanf(test, "%s", buffer);
    }
    fscanf(test, "%s %d %d %d", buffer, &j, &dernierChapDispo, &i);

    fclose(test);
    crashTemp(buffer, 100);
    sprintf(buffer, "manga/%s/Chapitre_%d/config.dat", mangaATester, dernierChapDispo);
    test = fopenR(buffer, "r");
    if(test == NULL)
    {
        sprintf(buffer, "manga/%s/Chapitre_%d.zip", mangaATester, dernierChapDispo);
        test = fopenR(buffer, "r");
        if(test == NULL)
        {
            /*On convertit mangaATester*/
            for(i = 0; i < LONGUEUR_NOM_MANGA_MAX; i++)
            {
                if(mangaATester[i] == '_')
                    mangaATester[i] = ' ';
            }
            return 1;
        }
    }
    else
        fclose(test);

    /*On convertit mangaATester*/
    for(i = 0; i < LONGUEUR_NOM_MANGA_MAX; i++)
    {
        if(mangaATester[i] == '_')
            mangaATester[i] = ' ';
    }
    return 0;
}

int mangaSelection(int mode, int tailleTexte[MANGAPARPAGE], int *manuel)
{
    /*Initialisations*/
    int i = 0, nombreManga = 0, mangaChoisis = 0, choix = 0, buffer = 0, hauteurBandeau = 0, chapitreMax = 0;
    SDL_Event event;
    TTF_Font *police = NULL;
    police = TTF_OpenFont(FONTUSED, POLICE_GROS);

    for(nombreManga = 0; tailleTexte[nombreManga] != 0; nombreManga++);

    if(mode == 3)
    {
        chapitreMax = *manuel;
        *manuel = 0;
    }

    /*On vois quelle est la forme de la fenetre*/
    switch(mode)
    {
        case 1:
        {
            while(mangaChoisis == 0)
            {
                event.type = -1;
                SDL_WaitEvent(&event);
                switch(event.type)
                {
                    case SDL_QUIT:
                        mangaChoisis = -4;
                        break;

                    case SDL_KEYDOWN:
                    {
                        buffer = nombreEntree(event);
                        if((buffer + choix * 10) <= nombreManga && buffer != -1)
                            choix = choix * 10 + buffer;
                        switch(event.key.keysym.sym)
                        {
                            case SDLK_RETURN:
                            case SDLK_KP_ENTER:
                                if(choix != 0)
                                {
                                    mangaChoisis = choix;
                                    *manuel = 1;
                                }
                                break;

                            case SDLK_BACKSPACE:
                                choix = choix / 10;
                                break;

                            case SDLK_ESCAPE:
                                mangaChoisis = -3;
                                break;

                            case SDLK_DELETE:
                                mangaChoisis = -2;
                                break;

                            default:
                                break;
                        }
                        break;
                    }
                    case SDL_MOUSEBUTTONUP:
                    {
                        for(i = 0; (((BORDURE_SUP_SELEC_MANGA + (LARGEUR_MOYENNE_MANGA_GROS + MINIINTERLIGNE) * i) > event.button.y || (BORDURE_SUP_SELEC_MANGA + (LARGEUR_MOYENNE_MANGA_GROS + MINIINTERLIGNE) * i + LARGEUR_MOYENNE_MANGA_GROS) < event.button.y) && i < NOMBRE_MANGA_MAX); i++);
                        if(i < NOMBRE_MANGA_MAX && tailleTexte[i] != 0)
                        {
                            if(event.button.x > (LARGEUR / 2) - (tailleTexte[i] / 2) && event.button.x < (LARGEUR / 2) + (tailleTexte[i] / 2))
                                mangaChoisis = i + 1;
                        }
                        break;
                    }
					default:
						if ((KMOD_LMETA & event.key.keysym.mod) && event.key.keysym.sym == SDLK_q)
							mangaChoisis = -4;
						break;
                }
            }
            break;
        }

        /*Si c'est via la nouvelle interface*/
        case 2:
        case 3: // Appelé par l'afficheur de chapitre, réagir au lien
        {
            while(mangaChoisis == 0)
            {
                if(mode == 3)
                    showNumero(police, choix, BORDURE_INF_NUMEROTATION_NSM);
                event.type = -1;
                SDL_WaitEvent(&event);
                switch(event.type)
                {
                    case SDL_QUIT:
                        mangaChoisis = -4;
                        break;

                    case SDL_KEYDOWN:
                    {
                        if(mode == 3)
                        {
                            buffer = nombreEntree(event);
                            if((((buffer + choix * 10) <= nombreManga && mode == 2) || ((buffer + choix * 10) <= chapitreMax && mode == 3)) && buffer != -1)
                                choix = choix * 10 + buffer;
                        }
                        else
                        {
                            //get letter pushed to sort
                            i = getLetterPushed(event);
                            if(i >= 'A' && i <= 'Z')
                                mangaChoisis = i * -1;

                        }
                        switch(event.key.keysym.sym)
                        {
                            case SDLK_BACKSPACE:
                                if(mode == 3)
                                    choix = choix / 10;
                                else
                                    mangaChoisis = ('A' - 1) * -1;
                                break;

                            case SDLK_RETURN:
                            case SDLK_KP_ENTER:
                                if(choix != 0 && mode == 3)
                                {
                                    mangaChoisis = choix;
                                    *manuel = 1;
                                }
                                break;

                            case SDLK_ESCAPE:
                                mangaChoisis = -3;
                                break;

                            case SDLK_DELETE:
                                mangaChoisis = -2;
                                break;

                            case SDLK_LEFT:
                                mangaChoisis = -7;
                                break;

                            case SDLK_RIGHT:
                                mangaChoisis = -6;
                                break;

                            default:
                                break;
                        }
                        break;
                    }
                    case SDL_MOUSEBUTTONUP:
                    {
                        if(event.button.y > BORDURE_SUP_SELEC_MANGA && event.button.y < BORDURE_SUP_SELEC_MANGA + LARGEUR_MOYENNE_MANGA_PETIT)
                            i = 0;

                        else
                            for(i = 0; (((BORDURE_SUP_SELEC_MANGA + (LARGEUR_MOYENNE_MANGA_PETIT + MINIINTERLIGNE) * i) > event.button.y || (BORDURE_SUP_SELEC_MANGA + (LARGEUR_MOYENNE_MANGA_PETIT + MINIINTERLIGNE) * i + LARGEUR_MOYENNE_MANGA_PETIT) < event.button.y) && i < NOMBRE_MANGA_MAX); i++);

                        /*Si appuis sur un bouton pour changer de page*/
                        if(event.button.y > HAUTEUR_CHANGEMENT_PAGE && (event.button.y < HAUTEUR_CHANGEMENT_PAGE + LARGEUR_MOYENNE_MANGA_PETIT ))
                        {
                            if(event.button.x > BORDURE_LAT_LECTURE && event.button.x < BORDURE_LAT_LECTURE + LONGUEUR_PRECENDENT) //Précédent
                            {
                                mangaChoisis = -7; // Page Précédente
                            }

                            else if(event.button.x > ecran->w - BORDURE_LAT_LECTURE - LONGUEUR_SUIVANT && event.button.x < ecran->w - BORDURE_LAT_LECTURE) //Suivant
                            {
                                mangaChoisis = -6; // Page Suivante
                            }
                        }

                        /*Si on choisis un chapitre*/
                        else if(i != 500 && tailleTexte[i * NBRCOLONNES] != 0)
                        {
                            /*Nombre Colonne*/
                            for(buffer = 0; ((BORDURELATSELECTION + (BORDURELATSELECTION + LONGUEURMANGA) * buffer) > event.button.x || (BORDURELATSELECTION + (BORDURELATSELECTION + LONGUEURMANGA) * buffer + LONGUEURMANGA) < event.button.x) && buffer < NBRCOLONNES; buffer++);
                            mangaChoisis = buffer * 100 + (i + 1);
                        }

                        /*Check si clique sur bouton de DL*/
                        hauteurBandeau = BORDURE_SUP_SELEC_MANGA + (LARGEUR_MOYENNE_MANGA_PETIT + MINIINTERLIGNE) * (nombreManga / NBRCOLONNES + 1);
                        if(event.button.y > hauteurBandeau && event.button.y < hauteurBandeau + LARGEUR_MOYENNE_MANGA_GROS && mode == 2)
                        {
                            if(event.button.x > ecran->w / 2)
                                mangaChoisis = -10;
                            else
                                mangaChoisis = -11;
                        }

                        /*Checks si clic sur zones cliquable pour le chapitre*/
                        if(mode == 3)
                        {
                            if(event.button.y >= BORDURE_SUP_INFOS_TEAM_CHAPITRE - 5 && event.button.y <= BORDURE_SUP_INFOS_TEAM_CHAPITRE + LARGEUR_MOYENNE_MANGA_GROS + 5 && event.button.x > 50 && event.button.x < ecran->w - 50)//Tolérance de 5 pxl
                                mangaChoisis = CODE_CLIC_LIEN_CHAPITRE; //Clic sur nom team -> lien

                            else if(event.button.y >= HAUTEUR_BOUTONS_CHAPITRE - 5 && event.button.y <= HAUTEUR_BOUTONS_CHAPITRE + LARGEUR_MOYENNE_MANGA_GROS + 5)
                            {
                                if(event.button.x < SEPARATION_COLONNE_1_CHAPITRE) //Premier chapitre
                                    mangaChoisis = CODE_BOUTON_1_CHAPITRE;
                                else if(event.button.x > SEPARATION_COLONNE_2_CHAPITRE) //Dernier chapitre
                                    mangaChoisis = CODE_BOUTON_3_CHAPITRE;
                                else
                                    mangaChoisis = CODE_BOUTON_2_CHAPITRE; //Bouton central, dernier chapitre choisis
                            }
                        }
                        break;
                    }

					default:
						if ((KMOD_LMETA & event.key.keysym.mod) && event.key.keysym.sym == SDLK_q)
							mangaChoisis = -4;
						break;
                }
            }
            break;
        }

        default:
            break;
    }
    TTF_CloseFont(police);
    return mangaChoisis;
}

int oldShowManga(TTF_Font *police, char mangaDispo[NOMBRE_MANGA_MAX][LONGUEUR_NOM_MANGA_MAX], int sectionChoisis, int nombreMangaElligible, int mangaElligibles[NOMBRE_MANGA_MAX])
{
    int i = 0, tailleTexte[NOMBRE_MANGA_MAX] = {0}, mangaChoisis = 0;
    char temp[TAILLE_BUFFER] = {0};
    SDL_Surface *texte = NULL;
    SDL_Rect position;
    SDL_Color couleurTexte = {POLICE_R, POLICE_G, POLICE_B}, couleurNew = {POLICE_NEW_R, POLICE_NEW_G, POLICE_NEW_B};

    TTF_SetFontStyle(police, TTF_STYLE_UNDERLINE);
    /*Affichage sur une seule colonne*/
    for(i = 0; i < nombreMangaElligible; i++)
    {
        crashTemp(temp, TAILLE_BUFFER);
        sprintf(temp, "%s", mangaDispo[mangaElligibles[i]]);
        /*Définis la couleur*/
        SDL_FreeSurface(texte);
        if(sectionChoisis == 4 && isItNew(mangaDispo[mangaElligibles[i]]))
            texte = TTF_RenderText_Blended(police, temp, couleurNew);
        else
            texte = TTF_RenderText_Blended(police, temp, couleurTexte);

        /*Affiche et enregistre des valeurs*/
        tailleTexte[i] = texte->w;
        position.x = (ecran->w / 2) - (texte->w / 2);
        position.y = BORDURE_SUP_SELEC_MANGA + (texte->h + MINIINTERLIGNE) * i;
        SDL_BlitSurface(texte, NULL, ecran, &position);
    }
    SDL_Flip(ecran);
    TTF_SetFontStyle(police, TTF_STYLE_NORMAL);

    mangaChoisis = mangaSelection(1, tailleTexte, &i) - 1;
    if(mangaChoisis < -2)
        mangaChoisis++;
    SDL_FreeSurface(texte);
    if(mangaChoisis >= 0)
        return mangaElligibles[mangaChoisis];
    return mangaChoisis;
}

int newShowManga(char mangaDispo[NOMBRE_MANGA_MAX][LONGUEUR_NOM_MANGA_MAX], int sectionChoisis, int nombreMangaElligible, int mangaElligibles[NOMBRE_MANGA_MAX], int nombreChapitre, int hauteurAffichage)
{
    /*Initialisation*/
    int pageSelection = 0, pageTotale = 0, mangaParColonne = 0, excedent = 0, i = 0, mangaColonne[NBRCOLONNES], mangaChoisis = 0, changementDePage = 0, limitationLettre = 0;
    int j = 0, tailleTexte[NOMBRE_MANGA_MAX] = {0}, manuel = 0, mode = 2, chapitreMax = 0;
    char temp[TAILLE_BUFFER] = {0};
    SDL_Surface *texte = NULL;
    SDL_Rect position;
    TTF_Font *police = NULL;
    SDL_Color couleurTexte = {POLICE_R, POLICE_G, POLICE_B}, couleurNew = {POLICE_NEW_R, POLICE_NEW_G, POLICE_NEW_B};

    police = TTF_OpenFont(FONTUSED, POLICE_PETIT);

    if(sectionChoisis == SECTION_CHOISIS_CHAPITRE) //Si c'est l'afficheur de chapitre qui appel, on réagira aux clics sur le lien
    {
        //On récupère le chapitre max
        chapitreMax = nombreChapitre;
        nombreChapitre = 0;
        mode = 3;
    }

    /*Multi-Page*/
    if (nombreMangaElligible > MANGAPARPAGE)
    {
        pageTotale = nombreMangaElligible / MANGAPARPAGE;
        if(nombreMangaElligible % MANGAPARPAGE > 0)
            pageTotale++;
        pageSelection = 1;
    }
    else
    {
        pageSelection = 1;
        pageTotale = 1;
    }

    if(sectionChoisis == SECTION_CHOISIS_CHAPITRE)
    {
        //On change la taille du fond pour pas écraser les boutons inférieurs
        SDL_FreeSurface(fond);
        fond = SDL_CreateRGBSurface(SDL_HWSURFACE, LARGEUR, HAUTEUR_BOUTONS_CHAPITRE - HAUTEUR_CHANGEMENT_PAGE, 32, 0, 0 , 0, 0); //on initialise le fond
#ifdef MACOS
        SDL_FillRect(fond, NULL, SDL_Swap32(SDL_MapRGB(ecran->format, FOND_R, FOND_G, FOND_B))); //We change background color
#else
        SDL_FillRect(fond, NULL, SDL_MapRGB(ecran->format, FOND_R, FOND_G, FOND_B)); //We change background color
#endif
    }

    TTF_SetFontStyle(police, TTF_STYLE_UNDERLINE);
    do
    {
        position.x = 0;
        position.y = HAUTEUR_CHANGEMENT_PAGE;
        SDL_BlitSurface(fond, NULL, ecran, &position);

        changementDePage = 0;
        mangaChoisis = 0;
        if(nombreMangaElligible < MANGAPARPAGE || pageSelection == pageTotale)
        {
            mangaParColonne = ((nombreMangaElligible % MANGAPARPAGE) / NBRCOLONNES);
            excedent = (nombreMangaElligible % MANGAPARPAGE) % NBRCOLONNES;

            for(i = 0; i < NBRCOLONNES; i++)
                mangaColonne[i] = mangaParColonne * (i + 1);

            if(!mangaParColonne && sectionChoisis != SECTION_CHOISIS_CHAPITRE)
            {
                mangaColonne[0] = 10;
                mangaColonne[1] = 20;
                mangaColonne[2] = 30;
            }

            if(excedent != 0)
            {
                for(i = 0; i < excedent; i++)
                    mangaColonne[i] = mangaColonne[i] + (i + 1);

                while(i < NBRCOLONNES)
                {
                    mangaColonne[i] = mangaColonne[i] + i;
                    i++;
                }
            }
        }
        else
        {
            mangaColonne[0] = 10;
            mangaColonne[1] = 20;
            mangaColonne[2] = 30;
        }

        j = 0;
        //mangaElligibles[i] = -1 quand il n'y a plus rien dans mangaElligibles, c'est la valeur par défaut

        //Moteur central: il écrit les nom des mangas
        for(i = (pageSelection - 1) * MANGAPARPAGE; (i < (pageSelection * MANGAPARPAGE) && mangaElligibles[i] != -1); i++)
        {
            if(limitationLettre == 0 || (sectionChoisis == 5 && mangaDispo[i][0] == limitationLettre) || (sectionChoisis != 5 && mangaDispo[mangaElligibles[i]][0] == limitationLettre))
            {
                crashTemp(temp, TAILLE_BUFFER);
                if(sectionChoisis == 5)
                    sprintf(temp, "%s", mangaDispo[i]);
                else
                    sprintf(temp, "%s", mangaDispo[mangaElligibles[i]]);

                if(sectionChoisis == SECTION_DL && mangaElligibles[i] != -1)
                {
                    if(isItNew(mangaDispo[mangaElligibles[i]]))
                        texte = TTF_RenderText_Blended(police, temp, couleurNew);
                    else
                        texte = TTF_RenderText_Blended(police, temp, couleurTexte);
                }
                else
                    texte = TTF_RenderText_Blended(police, temp, couleurTexte);

                /*Définis la position du texte en fonction de sa colonne*/
                if(j < mangaColonne[0])
                {
                    position.x = BORDURELATSELECTION;
                    position.y = hauteurAffichage + ((texte->h + MINIINTERLIGNE) * (j % mangaColonne[0]));
                }
                else if(j < mangaColonne[1])
                {
                    position.x = BORDURELATSELECTION + (BORDURELATSELECTION + LONGUEURMANGA);
                    position.y = hauteurAffichage + ((texte->h + MINIINTERLIGNE) * ((j - mangaColonne[0]) % mangaColonne[1]));
                }
                else if(j < mangaColonne[2])
                {
                    position.x = BORDURELATSELECTION + (2 * (BORDURELATSELECTION + LONGUEURMANGA));
                    position.y = hauteurAffichage + ((texte->h + MINIINTERLIGNE) * ((j - mangaColonne[1]) % mangaColonne[2]));
                }
                SDL_BlitSurface(texte , NULL, ecran, &position);
                tailleTexte[j++] = texte->w;
                SDL_FreeSurface(texte);
            }
        }

        char texte_Trad[5][100];
        lectureTrad(texte_Trad, 11);

        if(pageTotale != 1)
        {
            switch(langue)
            {
                case 1:
                    /*FranÁais*/
                    crashTemp(temp, TAILLE_BUFFER);
                    sprintf(temp, "Page %d sur %d", pageSelection, pageTotale);
                    break;

                case 2:
                    /*Anglais*/
                    crashTemp(temp, TAILLE_BUFFER);
                    sprintf(temp, "Page %d on %d", pageSelection, pageTotale);
                    break;

                case 3:
                    /*Italien*/
                    crashTemp(temp, TAILLE_BUFFER);
                    sprintf(temp, "Pagina %d di %d", pageSelection, pageTotale);
                    break;

				case 4:
                    /*Allemand*/
                    crashTemp(temp, TAILLE_BUFFER);
                    sprintf(temp, "Seite %d von %d", pageSelection, pageTotale);
                    break;

                default:
                    break;
            }

            position.y = HAUTEUR_CHANGEMENT_PAGE;
            texte = TTF_RenderText_Blended(police, texte_Trad[0], couleurTexte);
            position.x = BORDURE_LAT_LECTURE;
            SDL_BlitSurface(texte, NULL, ecran, &position);
            SDL_FreeSurface(texte);

            texte = TTF_RenderText_Blended(police, texte_Trad[1], couleurTexte);
            position.x = ecran->w - BORDURE_LAT_LECTURE - texte->w;
            SDL_BlitSurface(texte, NULL, ecran, &position);
            SDL_FreeSurface(texte);

            TTF_SetFontStyle(police, TTF_STYLE_NORMAL);
            texte = TTF_RenderText_Blended(police, temp, couleurTexte);
            position.x = ecran->w / 2 - texte->w / 2;
            SDL_BlitSurface(texte, NULL, ecran, &position);
            SDL_FreeSurface(texte);
            TTF_SetFontStyle(police, TTF_STYLE_UNDERLINE);
        }

        if(sectionChoisis == SECTION_DL)
        {
            /**On affiche, si on dl, les boutons de DL/Annulation**/
            if(nombreMangaElligible > MANGAPARPAGE)
                i = MANGAPARPAGE;
            else
                i = nombreMangaElligible;
            position.y = BORDURE_SUP_SELEC_MANGA + (LARGEUR_MOYENNE_MANGA_PETIT + MINIINTERLIGNE) * (i / NBRCOLONNES + 1);

            TTF_SetFontStyle(police, TTF_STYLE_NORMAL);
            crashTemp(temp, TAILLE_BUFFER);
            if(nombreChapitre < 2)
                sprintf(temp, "%d %s", nombreChapitre, texte_Trad[4]);
            else
                sprintf(temp, "%d %ss", nombreChapitre, texte_Trad[4]);
            texte = TTF_RenderText_Blended(police, temp, couleurTexte);
            position.x = ecran->w / 2;
            SDL_BlitSurface(texte, NULL, ecran, &position);
            SDL_FreeSurface(texte);

            TTF_SetFontStyle(police, TTF_STYLE_UNDERLINE);
            texte = TTF_RenderText_Blended(police, texte_Trad[2], couleurTexte);
            position.x = 50;
            SDL_BlitSurface(texte, NULL, ecran, &position);
            SDL_FreeSurface(texte);

            texte = TTF_RenderText_Blended(police, texte_Trad[3], couleurTexte);
            position.x = ecran->w - texte->w - 50;
            SDL_BlitSurface(texte, NULL, ecran, &position);
            SDL_FreeSurface(texte);
        }

        SDL_Flip(ecran);
        while(mangaChoisis == 0)
        {
            if(sectionChoisis != SECTION_CHOISIS_CHAPITRE)
                manuel = 0;
            else
                manuel = chapitreMax;

            //Manuel => si le nombre a été entré a la main
            do
            {
                mangaChoisis = mangaSelection(mode, tailleTexte, &manuel);
            }while((mangaChoisis <= -10 && sectionChoisis != SECTION_DL) && (mangaChoisis >= 'A' - 1 && mangaChoisis <= 'Z' && sectionChoisis <= 3) );

            if(sectionChoisis == SECTION_DL && mangaChoisis == -1)
                    mangaChoisis = -3;

            else if(mangaChoisis < -5 && mangaChoisis > -10)
            {
                if(pageTotale != 1)
                {
                    if(mangaChoisis == -6 && pageSelection < pageTotale) // Page Suivante
                    {
                        pageSelection++;
                        changementDePage = 1;
                        mangaChoisis = -1;
                    }

                    else if(mangaChoisis == -7 && pageSelection > 1) //Page précédente
                    {
                        pageSelection--;
                        changementDePage = 1;
                        mangaChoisis = -1;
                    }
                    else
                        mangaChoisis = 0;
                }
                else
                    mangaChoisis = 0;
            }

            else if(mangaChoisis > 0 && manuel == 0)
            {
                /*Si 1ere colonne*/
                if(mangaChoisis / 100 == 0)
                {
                    if (mangaColonne[0] < mangaChoisis % 100)
                        mangaChoisis = 0;
                    if (mangaColonne[0] >= mangaChoisis % 100)
                        mangaChoisis = mangaChoisis + (pageSelection - 1) * MANGAPARPAGE;
                }
                else if(mangaChoisis / 100 == 1)
                {
                    if(mangaColonne[1] - mangaColonne[0] < (mangaChoisis % 100) - 1)
                        mangaChoisis = 0;
                    if(mangaColonne[1] - mangaColonne[0] >= (mangaChoisis % 100) - 1)
                        mangaChoisis = (mangaColonne[mangaChoisis / 100 - 1] + (mangaChoisis % 100)) + (pageSelection - 1) * MANGAPARPAGE;
                }
                else if(mangaChoisis / 100 == 2)
                {
                    if (mangaColonne[2] - mangaColonne[1] < (mangaChoisis % 100) - 1)
                        mangaChoisis = 0;
                    if (mangaColonne[2] - mangaColonne[1] >= (mangaChoisis % 100) - 1)
                        mangaChoisis = (mangaColonne[mangaChoisis / 100 - 1] + (mangaChoisis % 100)) + (pageSelection - 1) * MANGAPARPAGE;
                }
            }

            else if(mangaChoisis > 0 && manuel == 1)
            {
                if(sectionChoisis != SECTION_CHOISIS_CHAPITRE)
                    mangaChoisis = mangaChoisis + MANGAPARPAGE * (pageSelection - 1);
                else
                    mangaElligibles[0] = -1;
            }

            else if(mangaChoisis * -1 >= 'A' - 1 && mangaChoisis * -1 <= 'Z')
            {
                if(mangaChoisis * -1 == 'A' - 1)
                    limitationLettre = 0;
                else
                    limitationLettre = mangaChoisis * -1;
                changementDePage = 1;
                mangaChoisis = -1;
            }
        }
    }while(changementDePage);
    TTF_CloseFont(police);

    if(sectionChoisis == SECTION_CHOISIS_CHAPITRE)
    {
        //On restaure la taille du fond
        SDL_FreeSurface(fond);
        fond = SDL_CreateRGBSurface(SDL_HWSURFACE, ecran->w, ecran->h - HAUTEUR_CHANGEMENT_PAGE, 32, 0, 0 , 0, 0); //on initialise le fond
#ifdef MACOS
        SDL_FillRect(fond, NULL, SDL_Swap32(SDL_MapRGB(ecran->format, FOND_R, FOND_G, FOND_B))); //We change background color
#else
        SDL_FillRect(fond, NULL, SDL_MapRGB(ecran->format, FOND_R, FOND_G, FOND_B)); //We change background color
#endif
    }
    else if(limitationLettre != 0)
    {
        int j = 0;
        for(i = (pageSelection - 1) * MANGAPARPAGE; i < (pageSelection * MANGAPARPAGE) && mangaChoisis != j; i++)
        {
            if(limitationLettre == 0 || (sectionChoisis == 5 && mangaDispo[i][0] == limitationLettre) || (sectionChoisis != 5 && mangaDispo[mangaElligibles[i]][0] == limitationLettre))
                j++;
        }
        mangaChoisis = i;
    }

    if(mangaChoisis > 0 && sectionChoisis != 5 && sectionChoisis != SECTION_CHOISIS_CHAPITRE)
        return mangaElligibles[mangaChoisis - 1];

    return mangaChoisis;
}

int lancementModuleDL(char cheminDAcces[100])
{
    char temp[250], superTemp[400];
    crashTemp(temp, 250);
    crashTemp(superTemp, 400);

    #ifdef WINDOWS
    int i = 0, j = 0;

    for(i = 0; REPERTOIREEXECUTION[i] != 0; i++)
    {
        if(REPERTOIREEXECUTION[i] == '\\')
        {
            temp[j++] = '\\';
        }
        temp[j++] = REPERTOIREEXECUTION[i];
    }
    sprintf(superTemp, "\"%s\\\\%s\"", temp, cheminDAcces);
    ShellExecute(NULL, "open", superTemp, "", "", SW_SHOWDEFAULT);
    #endif
    #ifdef UNIX
        #ifdef MACOS
        sprintf(superTemp, "open -n \"%s/%s\"", REPERTOIREEXECUTION, cheminDAcces);
        system(superTemp);
        #else
        /************************************************************
        *** Pourquoi faire simple quand on peut faire compliqué?  ***
        *** On va créer un frok puis le faire ce réinitialiser... ***
        ************************************************************/
        pid_t  child;

        child = fork();
        if (child == (pid_t)-1)
            exit(1);

        if (child)
            /*Père*/
            return 0;
        else
        {
            /*Fils*/
            mainDL();
            return -4;
        }
        #endif
    #endif
    return 0;
}

void lectureTrad(char trad[][100], int IDTrad)
{
    int i = 0, j = 0;

    FILE* fichierTrad = NULL;

    sprintf(trad[0], "data/%s/localization", LANGUAGE_PATH[langue - 1]);
    fichierTrad = fopenR(trad[0], "r");

    rewind(fichierTrad);
    fscanf(fichierTrad, "%d", &i);
    while(i != IDTrad)
    {
        while(fgetc(fichierTrad) != ']');
        fscanf(fichierTrad, "%d", &i);
    }

    for(i = 0; fgetc(fichierTrad) != ']' ; i++)
    {
        for(j = 0; j < 100; j++)
            trad[i][j] = 0;
#ifdef UNIX
		fseek(fichierTrad, 1, SEEK_CUR);
#endif

        for(j = 0; fgetc(fichierTrad) != '&' && j < 100; j++)
        {
            fseek(fichierTrad, -1, SEEK_CUR);
            if(j == 0)
            {
                if(fgetc(fichierTrad) == '#')
                {
                    for(j = 0; j < 100; j++)
                        trad[i][j] = 0;
                    fseek(fichierTrad, -1, SEEK_CUR);
                }
                else
                {
                    fseek(fichierTrad, -1, SEEK_CUR);
                    trad[i][j] = fgetc(fichierTrad);
                }
            }
            else
                trad[i][j] = fgetc(fichierTrad);
        }
    }
    fclose(fichierTrad);
}

void getBundleName(char *argv)
{
    #ifdef MACOS
    int i = 0, j = 0;
    for(; argv[i] != 0; i++);
    for(; argv[i] != 'p' && i > 0; argv[i--] = 0);
    for(; argv[i] != '/' && i > 0; i--);
    i++;
    for(; argv[i] != 0; BUNDLENAME[j++] = argv[i++]);
    BUNDLENAME[j] = '/';

    sprintf(FONTUSED, "%sdata/font.ttf", BUNDLENAME);

    #else
    int i = 0;
    for(; i < 100; BUNDLENAME[i++] = 0);
    #endif
}
