/*********************************************
**                                          **
**           The Restorator v3.2.1          **
**           Released by 29/05/2012         **
**                                          **
**********************************************/

#include "main.h"

static double DOWNLOAD_SPEED;
static double FILE_EXPECTED_SIZE;
static double CURRENT_FILE_SIZE;
static int JUST_BEFORE;
static int alright;

int telechargeur(char *adresse, char *repertoire, int activation)
{
    int status = 1, pourcent = 0;
    char temp[TAILLE_BUFFER];
    SDL_Rect position;
    SDL_Event event;

    DOWNLOAD_SPEED = 0;
    FILE_EXPECTED_SIZE = 0;
    CURRENT_FILE_SIZE = 0;
    alright = 1;

    if(activation == 1)
        JUST_BEFORE = 0;
    else
        JUST_BEFORE = 1337;

    ARGUMENT* envoi = malloc(sizeof(ARGUMENT));
    strcpy(envoi->URL, adresse);
    sprintf(envoi->repertoireEcriture, "%s%s", BUNDLENAME, repertoire);
    envoi->status = &status;

    #ifdef WINDOWS
    /*Initialisation du thread*/
    /*Initialisation du premier argument du thread: Attributs de sÈcuritÈ*/
    SECURITY_ATTRIBUTES attr;
    attr.nLength = sizeof(SECURITY_ATTRIBUTES);
    attr.lpSecurityDescriptor = NULL;
    attr.bInheritHandle = 0;
    /*DeuxiËme argument: Taille de la pile: par dÈfaut avec 0*/
    /*TroisiËme argument: pointeur vers une fonction dÈfinie d'une maniËre particuliËre*/
    /*QuatriËme argument: adresse des arguments, NULL quand il n'y en a pas*/
    /*CinquiËme argument: Ètat du thread au dÈmarrage: 0 pour le lancer directement, CREATE_SUSPENDED pour le mettre en pause au demarrage*/
    /*SixiËme argument: recupËre le PID du thread, NULL pour ne rien renvoyer*/
    CreateThread(&attr, 0, downloader, envoi, 0, NULL);
    #endif
    #ifdef UNIX
    pthread_t thread;

    if (pthread_create(&thread, NULL, downloader, envoi))
    {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }
    #endif
    if(activation == 1)
    {
        char texte[6][100];
        SDL_Surface *pourcentAffiche = NULL;
        TTF_Font *police = TTF_OpenFont(FONTUSED, POLICE_GROS);
        SDL_Color couleur = {POLICE_R, POLICE_G, POLICE_B};

        /*Remplissage des variables*/
        lectureTrad(texte, 20);
        pourcentAffiche = TTF_RenderText_Blended(police, texte[0], couleur);

        position.y = HAUTEUR_POURCENTAGE;
        position.x = 0;
        SDL_BlitSurface(fond, NULL, ecran, &position);
        position.x = ecran->w / 2 - pourcentAffiche->w / 2;
        SDL_BlitSurface(pourcentAffiche, NULL, ecran, &position);
        SDL_FreeSurface(pourcentAffiche);
        SDL_Flip(ecran);

        while(status != 0)
        {
            if(FILE_EXPECTED_SIZE > 0 && alright)
            {
                if(DOWNLOAD_SPEED != 0)
                    pourcent = CURRENT_FILE_SIZE * 100 / FILE_EXPECTED_SIZE;

                /*Code d'affichage du pourcentage*/
                crashTemp(temp, TAILLE_BUFFER);
                sprintf(temp, "%s %d,%d %s - %d%% - %s %d %s", texte[1], (int) FILE_EXPECTED_SIZE / 1024 / 1024 /*Nombre de megaoctets / 1'048'576)*/, (int) FILE_EXPECTED_SIZE / 10240 % 100 /*Nombre de dizaines ko*/ , texte[2], pourcent /*Pourcent*/ , texte[3], (int) DOWNLOAD_SPEED/*DÈbit*/, texte[4]);
                pourcentAffiche = TTF_RenderText_Blended(police, temp, couleur);

                SDL_PollEvent(&event);

                if(alright && event.type == SDL_QUIT)
                    alright = -1;
                if ((KMOD_LMETA & event.key.keysym.mod) && event.key.keysym.sym == SDLK_q && alright == 1)
                    alright = -1;

                position.y = HAUTEUR_POURCENTAGE;
                position.x = 0;
                SDL_BlitSurface(fond, NULL, ecran, &position);
                position.x = BORDURE_POURCENTAGE;
                SDL_BlitSurface(pourcentAffiche, NULL, ecran, &position);
                SDL_FreeSurface(pourcentAffiche);

                if(alright == -1)
                {
                    pourcentAffiche = TTF_RenderText_Blended(police, texte[5], couleur);

                    position.y = ecran->h / 2 - pourcentAffiche->h / 2;
                    position.x = ecran->w / 2 - pourcentAffiche->w / 2;
                    SDL_BlitSurface(fond, NULL, ecran, NULL);
                    SDL_BlitSurface(pourcentAffiche, NULL, ecran, &position);
                    SDL_FreeSurface(pourcentAffiche);
                    alright = 0;
                }

                SDL_Flip(ecran);
                SDL_PollEvent(&event);

                if(alright == 1 && event.type == SDL_QUIT)
                    alright = -1;
                if ((KMOD_LMETA & event.key.keysym.mod) && event.key.keysym.sym == SDLK_q && alright == 1)
                    alright = -1;
            }
            else
                for(pourcent = 0; pourcent < 20; pourcent++); //Sinon, ne marche pas en mode Release. Minimum: pourcent < 17
        }
        TTF_CloseFont(police);
    }
    else
    {
        while(status != 0)
        {
            SDL_PollEvent(&event);
            switch(event.type)
            {
                case SDL_QUIT:
                    alright = 0;
                    break;

                default:
					if ((KMOD_LMETA & event.key.keysym.mod) && event.key.keysym.sym == SDLK_q)
						alright = 0;
					break;
            }
        }
    }
    return alright;
}

#ifdef WINDOWS
DWORD WINAPI downloader(LPVOID envoi)
#else
void* downloader(void* envoi)
#endif
{
    char temp[200];
    CURL *curl;
    FILE* fichier;
    ARGUMENT *valeurs = (ARGUMENT *)envoi;

    crashTemp(temp, 200);
    sprintf(temp, "%s.download", valeurs->repertoireEcriture);
    fichier = fopen(temp, "wb");
    curl = curl_easy_init();
    if(curl && fichier != NULL)
    {
        if(PROXY_LYCEE) /*Proxy lycée: 172.20.0.1:3128*/
            curl_easy_setopt(curl, CURLOPT_PROXY, "http://172.20.0.1:3128");

        curl_easy_setopt(curl, CURLOPT_URL, valeurs->URL);
        #ifdef WINDOWS
			curl_easy_setopt(curl, CURLOPT_USERAGENT, "The_Restorator_WIN32");
        #else
			#ifdef MACOS
				curl_easy_setopt(curl, CURLOPT_USERAGENT, "The_Restorator_OSX");
			#else
				curl_easy_setopt(curl, CURLOPT_USERAGENT, "The_Restorator_UNIX");
			#endif
        #endif
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        if(JUST_BEFORE != 1337)
        {
            curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);
            curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, downloadData);
        }
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fichier);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        fclose(fichier);
    }
    else
        exit(EXIT_FAILURE);

    if(!alright)
        remove(temp);
    else
    {
        remove(valeurs->repertoireEcriture);
        rename(temp, valeurs->repertoireEcriture);
    }

    *valeurs->status = 0;
    free(valeurs);
    return 0;
}

int downloadData(void* ptr, double TotalToDownload, double NowDownloaded, double TotalToUpload, double NowUploaded)
{
    if(!JUST_BEFORE)
        JUST_BEFORE = SDL_GetTicks();

    else if(SDL_GetTicks() - JUST_BEFORE >= 500)
    {
        if(!FILE_EXPECTED_SIZE)
            FILE_EXPECTED_SIZE = TotalToDownload;
        /*taille dl / temps mis x nombre de fois le temps mis dans une sec / 1024 pour retour au ko*/
        DOWNLOAD_SPEED = (NowDownloaded - CURRENT_FILE_SIZE) / 1024;
        CURRENT_FILE_SIZE = NowDownloaded;
        JUST_BEFORE = SDL_GetTicks();
    }
    if(alright == 0)
        return -1;
    return 0;
}


size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    size_t written;
    written = fwrite(ptr, size, nmemb, stream);
    return written;
}

