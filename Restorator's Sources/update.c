#include "main.h"

#include <curl/curl.h>


size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    size_t written;
    written = fwrite(ptr, size, nmemb, stream);
    return written;
}

int addManga(SDL_Surface *ecran, SDL_Surface *accueil, SDL_Color couleurAccueil)
{
    int i = 0, j = 0, numerotation = 0, extremeManga[MANGA_MAX] = {0}, appelSelectionChapitre[2] = {1}, appelSelectionManga1[MANGA_MAX], appelSelectionManga2 = 4, mangaChoisis = -3, dbDown = 1, unUsed = 0, chapitreChoisis = -3;
    int continuer = 1, buffer = 0;
    char mangaCourt[MANGA_MAX][5], mangaDispo[MANGA_MAX][30];
    FILE *config = NULL;
    SDL_Surface *init = NULL;
    SDL_Rect position, positionTexte;
    SDL_Event event;
    TTF_Font *police = NULL;
    position.x = 0;
    position.y = (HAUTEUR / 2) - (accueil->h / 2);

    police = TTF_OpenFont("data/Timoteo.ttf", 30);

    for(i = 0; i < MANGA_MAX; i++)
    {
        appelSelectionManga1[i] = 4;
        for(j = 0; j < 30; j++)
        {
            mangaDispo[i][j] = 0;
        }
    }

    init = IMG_Load("data/initialisation.jpg");
    position.y = (HAUTEUR / 2) - (init->h / 2);
    SDL_BlitSurface(init, NULL, ecran, &position);
    SDL_Flip(ecran);

    #ifdef NOTATSCHOOL
    CURL *curl;
    char temp[100] = {0};

    crashTemp(temp, 100);
    sprintf(temp, "http://dl.dropbox.com/u/%d/config.dat", DROPBOX_ID_MMT);
    config = fopen("config.dat", "wb");
    CURLcode res;
    curl = curl_easy_init();
    if(curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, temp);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, config);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        fclose(config);
    }
    else
    {
        SDL_FreeSurface(init);
        return(EXIT_FAILURE);
    }
    #endif
    config = fopen("config.dat", "r");
    dbDown = 1;
    if(config != NULL)
    {
        if(fgetc(config) == EOF)
            dbDown = 0;
        else
        {
            fseek(config, -1, SEEK_CUR);
            while(fgetc(config) != '#')
            {
                fseek(config, -1, SEEK_CUR);
                /*i = 0;
                while(i < 5)
                {
                    mangaCourt[numerotation][i] = 0;
                    i++;
                }*/
                for(i = 0; i < 5; i++)
                    mangaCourt[numerotation][i] = 0;
                crashTemp(temp, 100);
                temp[0] = fgetc(config);
                fseek(config, -1, SEEK_CUR);
                fscanf(config, "%s %s %d\n", mangaDispo[numerotation], mangaCourt[numerotation], &extremeManga[numerotation]);
                numerotation++;
            }
        }
    }
    else
        dbDown = 0;

    if(dbDown == 0)
    {
        init = IMG_Load("data/dropboxDown.jpg");
        position.y = (HAUTEUR / 2) - (init->h / 2);
        SDL_BlitSurface(init, NULL, ecran, &position);
        SDL_Flip(ecran);
        while(dbDown == 0)
        {
            SDL_WaitEvent(&event);
            switch(event.type)
            {
                case SDL_QUIT:
                    dbDown = -1;
                    break;

                case SDL_KEYDOWN:
                {
                    switch(event.key.keysym.sym)
                    {
                        case SDLK_ESCAPE:
                        case SDLK_RETURN:
                        case SDLK_KP_ENTER:
                            dbDown = -1;
                            break;

                        default:
                            break;

                    }
                    break;
                }
            }

        }
        SDL_FreeSurface(init);
        return 1;
    }

    else
    {
        #ifdef NOTATSCHOOL
        sprintf(temp, "http://dl.dropbox.com/u/%d/Logiciel/actuel.txt", DROPBOX_ID_MMT);
        config = fopen("maj.dat", "wb");
        curl = curl_easy_init();
        if(curl)
        {
            curl_easy_setopt(curl, CURLOPT_URL, temp);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, config);
            res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);
            fclose(config);
        }
        else
        {
            SDL_FreeSurface(init);
            return(EXIT_FAILURE);
        }
        config = fopen("maj.dat", "r");
        fscanf(config, "%d", &buffer);

        if(buffer > CURRENTVERSION)
        {
            sprintf(temp, "http://dl.dropbox.com/u/%d/Logiciel/update.exe", DROPBOX_ID_MMT);
            config = fopen("update/update.exe", "wb");
            curl = curl_easy_init();
            if(curl)
            {
                curl_easy_setopt(curl, CURLOPT_URL, temp);
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, config);
                res = curl_easy_perform(curl);
                curl_easy_cleanup(curl);
                fclose(config);
            }
            else
            {
                SDL_FreeSurface(init);
                return(EXIT_FAILURE);
            }
        }
        #endif
        while(continuer != 0)
        {
            chapitreChoisis = -3;
            mangaChoisis = -3;
            continuer = 1;
            while((mangaChoisis != -1 && mangaChoisis != 0) && chapitreChoisis != -1)
            {
                if(chapitreChoisis == -2)
                    chapitreChoisis = -3;
                mangaChoisis = choixManga(ecran, couleurAccueil, mangaDispo, appelSelectionManga1, appelSelectionManga2);
                while((mangaChoisis != -1 && mangaChoisis != 0) && (chapitreChoisis != -1 && chapitreChoisis != -2))
                {
                    appelSelectionChapitre[0] = 1;
                    appelSelectionChapitre[1] = extremeManga[mangaChoisis - 1];
                    chapitreChoisis = choixChapitre(appelSelectionManga1, appelSelectionManga2, mangaChoisis, appelSelectionChapitre, mangaDispo, couleurAccueil, ecran, &unUsed);
                    if(chapitreChoisis != -1 && chapitreChoisis != -2)
                    {
                        donwloadChapter(mangaDispo, mangaChoisis, chapitreChoisis, appelSelectionChapitre, ecran, accueil, couleurAccueil);
                        chapitreChoisis = -1;
                    }
                }
            }
            position.y = 0;
            SDL_BlitSurface(accueil, NULL, ecran, &position);
            #ifdef FRENCH
            init = TTF_RenderText_Blended(police, "Voulez-vous telecharger d'autres manga?", couleurAccueil);
            positionTexte.y = (HAUTEUR / 2) - (init->h + 50);
            positionTexte.x = (ecran->w / 2) - (init->w / 2);
            SDL_BlitSurface(init, NULL, ecran, &positionTexte);
            init = TTF_RenderText_Blended(police, "Faites enter pour en retelecharger,", couleurAccueil);
            positionTexte.y = (HAUTEUR / 2) - (init->h / 2);
            positionTexte.x = (ecran->w / 2) - (init->w / 2);
            SDL_BlitSurface(init, NULL, ecran, &positionTexte);
            init = TTF_RenderText_Blended(police, "une autre touche pour quitter", couleurAccueil);
            positionTexte.y = (HAUTEUR / 2) - (init->h / 2 - 50);
            positionTexte.x = (ecran->w / 2) - (init->w / 2);
            SDL_BlitSurface(init, NULL, ecran, &positionTexte);
            #endif
            #ifdef ENGLISH
            init = TTF_RenderText_Blended(police, "Do you want to download more mangas?", couleurAccueil);
            positionTexte.y = (HAUTEUR / 2) - (init->h + 50);
            positionTexte.x = (ecran->w / 2) - (init->w / 2);
            SDL_BlitSurface(init, NULL, ecran, &positionTexte);
            init = TTF_RenderText_Blended(police, "Press enter for download others,", couleurAccueil);
            positionTexte.y = (HAUTEUR / 2) - (init->h / 2);
            positionTexte.x = (ecran->w / 2) - (init->w / 2);
            SDL_BlitSurface(init, NULL, ecran, &positionTexte);
            init = TTF_RenderText_Blended(police, "another key for quit.", couleurAccueil);
            positionTexte.y = (HAUTEUR / 2) - (init->h / 2 - 50);
            positionTexte.x = (ecran->w / 2) - (init->w / 2);
            SDL_BlitSurface(init, NULL, ecran, &positionTexte);
            #endif
            SDL_Flip(ecran);
            while(continuer == 1)
            {
                SDL_WaitEvent(&event);
                switch(event.type)
                {
                    case SDL_QUIT:
                        continuer = -1;
                        SDL_FreeSurface(init);
                        return 0;
                        break;

                    case SDL_KEYDOWN:
                    {
                        switch(event.key.keysym.sym)
                        {
                            case SDLK_RETURN:
                            case SDLK_KP_ENTER:
                                continuer = 2;
                                break;

                            default:
                                continuer = 0;
                                break;

                        }
                        break;
                    }
                }
            }
        }
    }
    SDL_FreeSurface(init);
    return 0;
}

int donwloadChapter(char mangaDispo[MANGA_MAX][30], int IDManga, int chapitreADL, int extremeManga[2], SDL_Surface *ecran, SDL_Surface *accueil, SDL_Color couleurAccueil)
{
    CURL *curl;
    FILE *ressources;
    FILE *chapitre;
    FILE *backup;
    #ifdef NOTATSCHOOL
    CURLcode res;
    #endif
    SDL_Surface *erreur = NULL, *affichage = NULL, *chapitreDL = NULL;
    SDL_Rect position, positionInfo;
    SDL_Event event;
    TTF_Font *police;
    char nomMangaCourt[5] = {0}, nomMangaComplet[30] = {0}, nomMangaComplet2[30] = {0}, temp[100] = {0}, nomAccesDropbox[30] = {0}, repertoireEnregistrement[50] = {0}, infos[100] = {0}, buffer1[100] = {0}, superBuffer[400] = {0};
    char bufferLoad = 0;
    int classe = 0, reinit = 0, buffer = 0, all = 0, dernierChapitreDispo = 0, j = 0, i = 0;

    police = TTF_OpenFont("data/Timoteo.ttf", 25);

    position.x = 0;
    position.y = (HAUTEUR / 2) - (accueil->h / 2);

    #ifdef NOTATSCHOOL
    ressources = fopen("mangadisponible", "wb+");

    curl = curl_easy_init();
    if(curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, "http://dl.dropbox.com/u/37363628/Logiciel/mangadisponible");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, ressources);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        fclose(ressources);
    }
    else
    {
        erreur = IMG_Load("data/erreur DL.jpg");
        position.y = (HAUTEUR / 2) - (erreur->h / 2);
        SDL_BlitSurface(erreur, NULL, ecran, &position);
        while(buffer == 0)
        {
            SDL_WaitEvent(&event);
            switch(event.type)
            {
                case SDL_QUIT:
                    buffer = -1;
                    break;

                case SDL_KEYDOWN:
                {
                    switch(event.key.keysym.sym)
                    {
                        case SDLK_ESCAPE:
                        case SDLK_RETURN:
                        case SDLK_KP_ENTER:
                            buffer = -1;
                            break;

                        default:
                            break;

                    }
                    break;
                }
            }

        }
        SDL_FreeSurface(erreur);
        SDL_FreeSurface(chapitreDL);
        SDL_FreeSurface(affichage);
        TTF_CloseFont(police);
        return(EXIT_FAILURE);
    }

    #endif

    ressources = fopen("mangadisponible", "r");
    if(fgetc(ressources) == EOF)
    {
        fclose(ressources);
        ressources = fopen("mangadisponible", "w+");
        backup = fopen("mangadisponible.backup", "r");
        while(fgetc(backup) != EOF)
        {
            fseek(backup, -1, SEEK_CUR);
            buffer = fgetc(backup);
            fputc(buffer, ressources);
        }
        fclose(ressources);
        fclose(backup);
    }
    else
    {
        fseek(ressources, -1, SEEK_CUR);
        backup = fopen("mangadisponible.backup", "w+");
        while(fgetc(ressources) != EOF)
        {
            fseek(ressources, -1, SEEK_CUR);
            buffer = fgetc(ressources);
            fputc(buffer, backup);
        }
        fclose(ressources);
        fclose(backup);
    }

    ressources = fopen("mangadisponible", "r");

    while(strcmp(mangaDispo[IDManga - 1], nomMangaComplet) != 0)
    {
        for(reinit = 0; reinit < 5; reinit++)
        {
            nomMangaCourt[reinit] = 0;
        }
        for(reinit = 0; reinit < 30; reinit++)
        {
            nomMangaComplet[reinit] = 0;
        }
        fscanf(ressources, "%s %s %d\n", nomMangaComplet, nomMangaCourt, &classe);
    }
    fclose(ressources);

    ressources = fopen("config.dat", "r");
    while(strcmp(nomMangaCourt, nomMangaComplet2) != 0)
    {
        for(reinit = 0; reinit < 30; reinit++)
        {
            nomAccesDropbox[reinit] = 0;
            nomMangaComplet2[reinit] = 0;
        }
        fscanf(ressources, "%s %s %d\n", nomAccesDropbox, nomMangaComplet2, &dernierChapitreDispo); //Ici, nomMangaComplet est uniquement la comme variable temporaire
    }
    fclose(ressources);
    crashTemp(temp, 100);

    /*Fin de la configuration, début du module de téléchargement a proprement parle.*/

    if(chapitreADL != 0)
    {
        sprintf(temp, "manga/%s/Chapitre_%d/config.dat", nomMangaComplet, chapitreADL);
        chapitre = fopen(temp, "r");
    }
    crashTemp(temp, 100);
    if(chapitre == NULL || chapitreADL == 0)
    {
        if(chapitreADL == 0 || all)
        {
            while(all != 2 && chapitreADL < dernierChapitreDispo)
            {
                chapitreADL++;
                sprintf(temp, "manga/%s/Chapitre_%d/config.dat", nomMangaComplet, chapitreADL);
                ressources = fopen(temp, "r");
                if(ressources == 0)
                    all = 2;
                else
                    fclose(ressources);
            }
            if(all != 2 && chapitreADL >= dernierChapitreDispo)
            {
                /*Rien de nouveau*/
                all = -1;
            }
        }
        if(all != -1)
        {
            do
            {
                sprintf(temp, "http://dl.dropbox.com/u/%d/%s/%s_Chapitre_%d.zip", DROPBOX_ID_MMT, nomAccesDropbox, nomMangaCourt, chapitreADL);
                sprintf(repertoireEnregistrement, "update/%s_Chapitre_%d.zip", nomMangaCourt, chapitreADL);
                #ifdef NOTATSCHOOL
                chapitre = fopen(repertoireEnregistrement, "wb");
                #endif
                curl = curl_easy_init();
                if(curl)
                {
                    affichage = IMG_Load("data/telechargementInProgress.jpg");
                    position.y = 0;
                    SDL_BlitSurface(affichage, NULL, ecran, &position);
                    positionInfo.y = 575;
                    crashTemp(infos, 100);
                    for(buffer = 0; buffer < 30; buffer++)
                    {
                        if(nomMangaComplet[buffer] == ' ')
                            nomMangaComplet[buffer] = '_';
                    }
                    #ifdef FRENCH
                    sprintf(infos, "Telechargement de %s chapitre #%d en cours...", nomMangaComplet, chapitreADL);
                    #endif
                    #ifdef ENGLISH
                    sprintf(infos, "Download of %s chapter #%d in progress...", nomMangaComplet, chapitreADL);
                    #endif
                    SDL_FreeSurface(chapitreDL);
                    SDL_Surface *chapitreDL = NULL;
                    chapitreDL = TTF_RenderText_Blended(police, infos, couleurAccueil);
                    positionInfo.x = (ecran->w / 2) - (chapitreDL->w /2);
                    SDL_BlitSurface(chapitreDL, NULL, ecran, &positionInfo);
                    SDL_Flip(ecran);
                    #ifdef NOTATSCHOOL
                    curl_easy_setopt(curl, CURLOPT_URL, temp);
                    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
                    curl_easy_setopt(curl, CURLOPT_WRITEDATA, chapitre);
                    res = curl_easy_perform(curl);
                    fclose(chapitre);
                    #endif
                    curl_easy_cleanup(curl);
                    crashTemp(repertoireEnregistrement, 50);
                    sprintf(repertoireEnregistrement, "update/%s_Chapitre_%d.zip", nomMangaCourt, chapitreADL);
                    chapitre = fopen(repertoireEnregistrement, "r");
                    if(chapitre != NULL)
                    {
                        if(fgetc(chapitre) != EOF)
                        {
                            fclose(chapitre);
                            crashTemp(temp, 100);
                            crashTemp(buffer1, 100);
                            j = 0;
                            for(i = 0; REPERTOIREEXECUTION[i] != 0; i++)
                            {
                                if(REPERTOIREEXECUTION[i] == '\\')
                                {
                                    buffer1[j] = '\\';
                                    j++;
                                }
                                buffer1[j] = REPERTOIREEXECUTION[i];
                                j++;
                            }
                            crashTemp(superBuffer, 400);
                            affichage = IMG_Load("data/instalation.jpg");
                            position.y = (HAUTEUR / 2) - (affichage->h / 2);
                            SDL_BlitSurface(affichage, NULL, ecran, &position);
                            SDL_Flip(ecran);
                            sprintf(temp, "update/%s_Chapitre_%d", nomMangaCourt, chapitreADL);
                            mkdir(temp);
                            sprintf(superBuffer, "\"\"%s\\\\7za\" e \"%s\\\\Update\\\\%s_Chapitre_%d.zip\" -o\"%s\\\\Update\\\\%s_Chapitre_%d\"\"", buffer1, buffer1, nomMangaCourt, chapitreADL, buffer1, nomMangaCourt, chapitreADL);
                            system(superBuffer);
                            crashTemp(temp, 100);
                            sprintf(temp, "update/%s_Chapitre_%d/config.dat", nomMangaCourt, chapitreADL);
                            chapitre = fopen(temp, "r");
                            if(chapitre != NULL)
                            {
                                fclose(chapitre);
                                crashTemp(temp, 100);
                                sprintf(temp, "update/%s_Chapitre_%d.zip", nomMangaCourt, chapitreADL);
                                remove(temp);
                            }
                            /*else
                                erreur lors de la décompression*/

                            chapitre = fopen("update/toimport.dat", "r");
                            backup = fopen("update/toimport.tmp", "w+");
                            if(chapitre != NULL)
                            {
                                while(fgetc(chapitre) != '<')
                                {
                                    fseek(chapitre, -1, SEEK_CUR);
                                    bufferLoad = fgetc(chapitre);
                                    fprintf(backup, "%c", bufferLoad);
                                }
                                fclose(chapitre);
                                remove("update/toimport.dat");
                            }
                            fprintf(backup, "%s %d\n<-", nomMangaCourt, chapitreADL);
                            fclose(backup);
                            rename("update/toimport.tmp", "update/toimport.dat");
                        }
                    }
                    else
                        printf("Erreur lors du téléchargement de %s chapitre %d\nLien: %s\n", nomMangaComplet, chapitreADL, temp);
                        /*Erreur lors du téléchargement*/
                }
                else
                {
                    erreur = IMG_Load("data/erreur DL.jpg");
                    position.y = (HAUTEUR / 2) - (erreur->h / 2);
                    SDL_BlitSurface(erreur, NULL, ecran, &position);
                    while(buffer == 0)
                    {
                        SDL_WaitEvent(&event);
                        switch(event.type)
                        {
                            case SDL_QUIT:
                                buffer = -1;
                                break;

                            case SDL_KEYDOWN:
                            {
                                switch(event.key.keysym.sym)
                                {
                                    case SDLK_ESCAPE:
                                    case SDLK_RETURN:
                                    case SDLK_KP_ENTER:
                                        buffer = -1;
                                        break;

                                    default:
                                        break;

                                }
                                break;
                            }
                        }

                    }
                    SDL_FreeSurface(erreur);
                    SDL_FreeSurface(chapitreDL);
                    SDL_FreeSurface(affichage);
                    TTF_CloseFont(police);
                    return(EXIT_FAILURE);
                }

                if(all == 2)
                {
                    all = 1;
                    while(all == 1)
                    {
                        chapitreADL++;
                        if(chapitreADL > dernierChapitreDispo)
                        {
                            /*Rien de nouveau*/
                            all = -1;
                        }
                        if(chapitreADL <= dernierChapitreDispo)
                        {
                            all = 1;
                            sprintf(temp, "manga/%s/Chapitre_%d/config.dat", nomMangaComplet, chapitreADL);
                            ressources = fopen(temp, "r");
                            if(ressources == 0)
                                all = 2;
                            else
                                fclose(ressources);
                        }
                    }
                }
                else
                    all = -1;

            } while(all != -1);
            check4update(ecran);

        }
    }
    SDL_FreeSurface(erreur);
    SDL_FreeSurface(affichage);
    SDL_FreeSurface(chapitreDL);
    TTF_CloseFont(police);
    return 0;
}

