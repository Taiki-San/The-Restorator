/*********************************************
**                                          **
**           The Restorator v3.2.1          **
**           Released by 29/05/2012         **
**                                          **
**********************************************/

#include "main.h"

static int status;
static int error;

int DLmanager()
{
    SDL_Event event;
    #ifdef WINDOWS
    /*Initialisation du thread*/
    /*Initialisation du premier argument du thread: Attributs de s»curit»*/
    SECURITY_ATTRIBUTES attr;
    attr.nLength = sizeof(SECURITY_ATTRIBUTES);
    attr.lpSecurityDescriptor = NULL;
    attr.bInheritHandle = 0;
    /*DeuxiÀme argument: Taille de la pile: par d»faut avec 0*/
    /*TroisiÀme argument: pointeur vers une fonction d»finie d'une maniÀre particuliÀre*/
    /*QuatriÀme argument: adresse des arguments, NULL quand il n'y en a pas*/
    /*CinquiÀme argument: »tat du thread au d»marrage: 0 pour le lancer directement, CREATE_SUSPENDED pour le mettre en pause au demarrage*/
    /*SixiÀme argument: recupÀre le PID du thread, NULL pour ne rien renvoyer*/
    CreateThread(&attr, 0, telechargement, "", 0, NULL);
    #endif
    #ifdef UNIX
    pthread_t thread;

    if (pthread_create(&thread, NULL, telechargement, NULL))
    {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }
    #endif
    status = 1;
    while(status != 0)
        SDL_PollEvent(&event);
    return error;
}

#ifdef WINDOWS
DWORD WINAPI telechargement()
#else
void* telechargement()
#endif
{
    int i = 0, j = 0, k = 0, l = 0, chapitre = 0, mangaActuel = 0, mangaTotal = 0, pourcentage = 0, glados = 1;
    char buffer[LONGUEUR_NOM_MANGA_MAX], teamCourt[LONGUEUR_COURT], temp[200], mangaLong[LONGUEUR_NOM_MANGA_MAX], historiqueTeam[1000][LONGUEUR_COURT];
    char URL[200], type[5], superTemp[400], trad[5][100], site[LONGUEUR_SITE];
    FILE* fichier = NULL;
    FILE* ressources = NULL;
    FILE* test = NULL;
    SDL_Surface *texte;
    TTF_Font *police = NULL;
    SDL_Rect position;

    police = TTF_OpenFont(FONTUSED, POLICE_PETIT);
    SDL_Color couleurTexte = {POLICE_R, POLICE_G, POLICE_B};

    fichier = fopenR("update/import.dat", "r");

    /*On compte le nombre de mangas*/
    for(mangaTotal = 0; fgetc(fichier) != EOF; mangaTotal++)
    {
        fseek(fichier, -1, SEEK_CUR);
        /*Variable sans interet*/
        fscanf(fichier, "%s %s %d\n", buffer, temp, &i);
    }
    rewind(fichier);

    for(i = 0; i < 5; i++)
    {
        for(j = 0; j < LONGUEUR_COURT; j++)
            trad[i][j] = 0;
    }
    for(i = 0; i < 1000; i++)
    {
        for(j = 100; j < 100; j++)
            historiqueTeam[i][j] = 0;
    }
    lectureTrad(trad, 22);

    while(fgetc(fichier) != EOF && glados)
    {
        mangaActuel++;
        pourcentage = mangaActuel * 100 / mangaTotal;
        /*Extraction du chapitre*/
        fseek(fichier, -1, SEEK_CUR);
        crashTemp(buffer, LONGUEUR_NOM_MANGA_MAX);
        crashTemp(teamCourt, LONGUEUR_COURT);
        fscanf(fichier, "%s %s %d", teamCourt, buffer, &chapitre);

        /*RÈcuperer les donnÈes relative a la team*/
        ressources = fopenR("repo", "r");
        crashTemp(temp, 200);
        while(fgetc(ressources) != EOF && strcmp(temp, teamCourt))
        {
            fseek(ressources, -1, SEEK_CUR);
            crashTemp(temp, 200);
            fscanf(ressources, "%s", temp);
        }
        if(strcmp(temp, teamCourt) == 0)
        {
            crashTemp(URL, 200);
            crashTemp(type, 5);
            crashTemp(site, LONGUEUR_SITE);
            fscanf(ressources, "%s %s %s", type, URL, site);
            fclose(ressources);

            /*Etablir relation entre le nom du manga et son nomComplet*/
            ressources = fopenR("mangas", "r");
            crashTemp(temp, 200);
            /*On se position dans l'espace team*/
            while(fgetc(ressources) != EOF && strcmp(temp, teamCourt))
            {
                fseek(ressources, -1, SEEK_CUR);
                crashTemp(temp, 200);
                fscanf(ressources, "%s", temp);
            }
            /*On lit les mangas*/
            while(fgetc(ressources) != '#' && strcmp(temp, buffer))
            {
                fseek(ressources, -1, SEEK_CUR);
                crashTemp(temp, 200);
                crashTemp(mangaLong, LONGUEUR_NOM_MANGA_MAX);
                fscanf(ressources, "%s %s %d %d %d %d", mangaLong, temp, &l, &i, &j, &k);
            }
            fclose(ressources);

            if(strcmp(temp, buffer) == 0)
            {
                /*Ouverture du site de la team*/
                for(i = 0; i < 1000 && strcmp(teamCourt, historiqueTeam[i]) != 0 && historiqueTeam[i][0] != 0; i++);
                if(i < 1000 && historiqueTeam[i][0] == 0) //Si pas dÈj‡ installÈ
                {
                    strcpy(historiqueTeam[i], teamCourt);
                    ouvrirSite(teamCourt);
                }

                /*Si on a bien tout extrait*/
                crashTemp(superTemp, 400);
                /*GÈnÈration de l'URL*/
                if(strcmp(type, "DB") == 0)
                    sprintf(superTemp, "http://dl-web.dropbox.com/u/%s/%s/%s_Chapitre_%d.zip", URL, mangaLong, buffer, chapitre);

                else if (strcmp(type, "O") == 0)
                    sprintf(superTemp, "http://%s/%s/%s_Chapitre_%d.zip", type, mangaLong, buffer, chapitre);

                else
                {
                    printf("URL non gÈrÈe: %s\n", type);
                    continue;
                }

                /*Affichage du DL*/
                crashTemp(temp, 200);
                for(i = 0; i < LONGUEUR_NOM_MANGA_MAX; i++)
                {
                    if(mangaLong[i] == '_')
                        mangaLong[i] = ' ';
                }

                sprintf(temp, "%s %s %s %d %s %s (%d%% %s)", trad[0], mangaLong, trad[1], chapitre, trad[2], teamCourt, pourcentage, trad[3]);

                for(i = 0; i < LONGUEUR_NOM_MANGA_MAX; i++)
                {
                    if(mangaLong[i] == ' ')
                        mangaLong[i] = '_';
                }
                //On remplis la fenÕtre
                SDL_BlitSurface(fond, NULL, ecran, NULL);
                texte = TTF_RenderText_Blended(police, trad[4], couleurTexte);
                position.x = ecran->w / 2 - texte->w / 2;
                position.y = HAUTEUR_MESSAGE_INITIALISATION;
                SDL_BlitSurface(texte, NULL, ecran, &position);
                SDL_FreeSurface(texte);
                texte = TTF_RenderText_Blended(police, temp, couleurTexte);
                position.y = HAUTEUR_TEXTE_TELECHARGEMENT;
                position.x = ecran->w / 2 - texte->w / 2;
                SDL_BlitSurface(texte, NULL, ecran, &position);
                SDL_FreeSurface(texte);
                SDL_Flip(ecran);
                for(j = 0; j < 10000; j++);

                /*TÈlÈchargement*/
                /*On teste si le chapitre est dÈj‡ installÈ*/
                crashTemp(temp, 200);
                sprintf(temp, "manga/%s/Chapitre_%d/config.dat", mangaLong, chapitre);
                test = fopenR(temp, "r");

                crashTemp(temp, 200);
                sprintf(temp, "update/[%s]%s_Chapitre_%d.zip", teamCourt, buffer, chapitre);
                ressources = fopenR(temp, "r");

                if(ressources == NULL && test == NULL)
                {
                    glados = telechargeur(superTemp, temp, 1);
                    ressources = fopenR(temp, "r");
                    if(fgetc(ressources) == '<')
                    {
                        fclose(ressources);
                        ressources = fopenR("erreur.txt", "a+");
                        fprintf(ressources, "Erreur dans le telechargement d'un chapitre: Dropbox indisponible. Team: %s\n", teamCourt);
                        fclose(ressources);
                        remove(temp);
                    }
                    else
                        fclose(ressources);
                }
                else
                {
                    if(ressources != NULL)
                        fclose(ressources);
                    if(test != NULL)
                        fclose(test);
                }
                if(glados) //Si on demande la fermeture, on ouvre pas le fichier donc pas d'installation
                {
                    ressources = fopenR(temp, "r");
                    if(ressources != NULL)
                    {
                        fclose(ressources);
                        /**Installation**/
                        DATA_INSTALL* datas = malloc(sizeof(DATA_INSTALL));
                        strcpy(datas->teamCourt, teamCourt);
                        strcpy(datas->mangaCourt, buffer);
                        datas->chapitre = chapitre;
                        #ifdef WINDOWS
                        /*Initialisation du thread*/
                        /*Initialisation du premier argument du thread: Attributs de s»curit»*/
                        SECURITY_ATTRIBUTES attr;
                        attr.nLength = sizeof(SECURITY_ATTRIBUTES);
                        attr.lpSecurityDescriptor = NULL;
                        attr.bInheritHandle = 0;
                        /*DeuxiÀme argument: Taille de la pile: par d»faut avec 0*/
                        /*TroisiÀme argument: pointeur vers une fonction d»finie d'une maniÀre particuliÀre*/
                        /*QuatriÀme argument: adresse des arguments, NULL quand il n'y en a pas*/
                        /*CinquiÀme argument: »tat du thread au d»marrage: 0 pour le lancer directement, CREATE_SUSPENDED pour le mettre en pause au demarrage*/
                        /*SixiÀme argument: recupÀre le PID du thread, NULL pour ne rien renvoyer*/
                        CreateThread(&attr, 0, installation, datas, 0, NULL);
                        #endif
                        #ifdef UNIX
                        pthread_t thread;

                        if (pthread_create(&thread, NULL, installation, datas))
                        {
                            perror("pthread_create");
                            exit(EXIT_FAILURE);
                        }
                        #endif
                    }
                }
                crashTemp(temp, 200);
                sprintf(temp, "manga/%s/infos.png", mangaLong);
                test = fopenR(temp, "r");
                if(test == NULL && k == 1)
                {
                    crashTemp(temp, TAILLE_BUFFER);
                    sprintf(temp, "manga/%s/config.dat", mangaLong);
                    test = fopenR(temp, "r");
                    if(test == NULL)
                    {
                        crashTemp(temp, TAILLE_BUFFER);
                        sprintf(temp, "manga/%s", mangaLong);
                        #ifdef WINDOWS
                        mkdirR(temp);
                        #else
                        mkdir(temp, PERMISSIONS);
                        #endif
                    }
                    else
                        fclose(test);

                    crashTemp(superTemp, 400);
                    /*GÈnÈration de l'URL*/
                    if(strcmp(type, "DB") == 0)
                    {
                        sprintf(superTemp, "http://dl-web.dropbox.com/u/%s/%s/infos.png", URL, mangaLong);
                    }
                    else if (strcmp(type, "O") == 0)
                    {
                        sprintf(superTemp, "http://%s/%s/infos.png", type, mangaLong);
                    }
                    else
                    {
                        printf("URL non gÈrÈe: %s\n", type);
                    }

                    crashTemp(temp, TAILLE_BUFFER);
                    sprintf(temp, "manga/%s/infos.png", mangaLong);
                    telechargeur(superTemp, temp, 0);
                }
                else if(test != NULL)
                    fclose(test);
            }
        }
    }
    fclose(fichier);
    if(glados)
        removeR("update/import.dat");
    char texteTrad[7][LONGUEURTEXTE];
    lectureTrad(texteTrad, 16);

    SDL_BlitSurface(fond, NULL, ecran, NULL);
    texte = TTF_RenderText_Blended(police, texteTrad[0], couleurTexte);
    position.x = ecran->w / 2 - texte->w / 2;
    position.y = ecran->h / 2 - texte->h / 2;
    SDL_BlitSurface(texte, NULL, ecran, &position);
    SDL_FreeSurface(texte);
    SDL_Flip(ecran);
    TTF_CloseFont(police);
    while(status != 1);
    status = 0;
    return 0;
}

#ifdef WINDOWS
DWORD WINAPI installation(LPVOID datas)
#else
void* installation(void* datas)
#endif
{
    int nouveauDossier = 0, i = 0, j = 0, k = 0, l = 0, extremes[2], erreurs = 0, dernierLu = 0, chapitre = 0;
    char temp[TAILLE_BUFFER], buffer1[200], mangaLong[LONGUEUR_NOM_MANGA_MAX];
    char teamCourt[LONGUEUR_COURT], mangaCourt[LONGUEUR_COURT];
    FILE* ressources = NULL;
    FILE* test = NULL;

    status++; //On signale le lancement d'une installation

    /*RÈcupÈration des valeurs envoyÈs*/
    DATA_INSTALL *valeurs = (DATA_INSTALL*)datas;
    strcpy(teamCourt, valeurs->teamCourt);
    strcpy(mangaCourt, valeurs->mangaCourt);
    chapitre = valeurs->chapitre;

    /*Lecture du fichier*/
    nouveauDossier = 0;
    crashTemp(temp, TAILLE_BUFFER);
    sprintf(temp, "update/[%s]%s_Chapitre_%d.zip", teamCourt, mangaCourt, chapitre);
    test = fopenR(temp, "r");

    if(test != NULL)
    {
        fclose(test);
        /*On rÈcupËre les donnÈes de manga relative a ce manga*/
        /*Etablir relation entre le nom du manga et son nomComplet*/
        ressources = fopenR("mangas", "r");
        crashTemp(temp, TAILLE_BUFFER);
        /*On se position dans l'espace team*/
        while(fgetc(ressources) != EOF && strcmp(temp, teamCourt))
        {
            fseek(ressources, -1, SEEK_CUR);
            crashTemp(temp, TAILLE_BUFFER);
            fscanf(ressources, "%s", temp);
        }

        /*On lit les mangas*/
        while(fgetc(ressources) != '#' && strcmp(temp, mangaCourt))
        {
            fseek(ressources, -1, SEEK_CUR);
            crashTemp(temp, TAILLE_BUFFER);
            crashTemp(mangaLong, LONGUEUR_NOM_MANGA_MAX);
            i = 0;
            j = 0;
            k = 0;
            fscanf(ressources, "%s %s %d %d %d %d", mangaLong, temp, &l, &i, &j, &k);
        }
        fclose(ressources);

        if(strcmp(temp, mangaCourt) == 0)
        {
            /*On continue si tout vas bien*/
            crashTemp(temp, TAILLE_BUFFER);
            /*VÈrification d'existance*/
            sprintf(temp, "manga/%s/Chapitre_%d/config.dat", mangaLong, chapitre);
            test = fopenR(temp, "r");
            if(test == NULL)
            {
                /*Si le chapitre n'est pas dÈj‡ installÈ*/
                crashTemp(temp, TAILLE_BUFFER);
                sprintf(temp, "manga/%s/config.dat", mangaLong);
                test = fopenR(temp, "r");
                if(test == NULL)
                {
                    /*Si le dossier du manga n'existe pas*/
                    crashTemp(temp, TAILLE_BUFFER);
                    sprintf(temp, "%smanga/%s", BUNDLENAME, mangaLong);
                    #ifdef WINDOWS
                    mkdir(temp);
                    #endif
                    #ifdef UNIX
                    mkdir(temp, PERMISSIONS);
                    #endif

                    /*On signale le nouveau dossier*/
                    nouveauDossier = 1;
                }
                else
                    fclose(test);

                /**DÈcompression dans le repertoire de destination**/

                /*CrÈation du rÈpertoire de destination*/
                crashTemp(temp, TAILLE_BUFFER);
                sprintf(temp, "%smanga/%s/Chapitre_%d", BUNDLENAME, mangaLong, chapitre);
                mkdirR(temp);

                crashTemp(temp, TAILLE_BUFFER);
                crashTemp(buffer1, 200);

                sprintf(buffer1, "update/[%s]%s_Chapitre_%d.zip", teamCourt, mangaCourt, chapitre);
                sprintf(temp, "manga/%s/Chapitre_%d", mangaLong, chapitre);

                unzip(buffer1, temp);

                /*Si c'est pas un nouveau dossier, on modifie config.dat du manga*/
                crashTemp(temp, TAILLE_BUFFER);
                sprintf(temp, "manga/%s/Chapitre_%d/config.dat", mangaLong, chapitre);
                ressources = fopenR(temp, "r");
                if(nouveauDossier == 0 && ressources != NULL)
                {
                    fclose(ressources);
                    crashTemp(temp, TAILLE_BUFFER);
                    sprintf(temp, "manga/%s/config.dat", mangaLong);
                    ressources = fopenR(temp, "r+");
                    fscanf(ressources, "%d %d", &extremes[0], &extremes[1]);
                    if(fgetc(ressources) != EOF)
                        fscanf(ressources, "%d", &dernierLu);
                    else
                        dernierLu = -1;
                    fclose(ressources);
                    ressources = fopenR(temp, "w+");
                    if(extremes[0] > chapitre)
                        fprintf(ressources, "%d %d", chapitre, extremes[1]);

                    else if(extremes[1] < chapitre)
                        fprintf(ressources, "%d %d", extremes[0], chapitre);

                    else
                        fprintf(ressources, "%d %d", extremes[0], extremes[1]);
                    if(dernierLu != -1)
                        fprintf(ressources, " %d", dernierLu);

                    fclose(ressources);
                }

                else if(ressources != NULL)
                {
                    fclose(ressources);
                    /*CrÈation du config.dat du nouveau manga*/
                    crashTemp(temp, TAILLE_BUFFER);
                    sprintf(temp, "manga/%s/config.dat", mangaLong);
                    ressources = fopenR(temp, "w+");
                    fprintf(ressources, "%d %d", chapitre, chapitre);
                    fclose(ressources);
                }
                else
                {
                    ressources = fopenR("erreur.txt", "a+");
                    crashTemp(temp, TAILLE_BUFFER);
                    sprintf(temp, "manga/%s/Chapitre_%d", mangaLong, chapitre);
                    fprintf(ressources, "Archive Corrompue: %s - %d\n", mangaLong, chapitre);
                    removeFolder(temp, 100);
                    erreurs = 1;
                }
                /*On supprime l'archive*/
                crashTemp(temp, TAILLE_BUFFER);
                sprintf(temp, "update/[%s]%s_Chapitre_%d.zip", teamCourt, mangaCourt, chapitre);
                removeR(temp);
            }
            else
            {
                fclose(test);
                crashTemp(temp, TAILLE_BUFFER);
                sprintf(temp, "update/[%s]%s_Chapitre_%d.zip", teamCourt, mangaCourt, chapitre);
                removeR(temp);
            }
        }
    }
    if(erreurs)
        error++; //On note si le chapitre a posÈ un problËme

    free(valeurs);
    status--; //On signale la fin de l'installation
    return 0;
}

