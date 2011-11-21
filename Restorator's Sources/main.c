#include "main.h"

int HAUTEUR = 700; //Default value for the height of the windows. Can't be #define because must be changed

int main(int argc, char *argv[])
{
    /*Variable have french name but not really problematic... Lot of them have random names*/

    SDL_Surface *ecran = NULL, *fond = NULL, *accueil = NULL;
    SDL_Event event;
    SDL_Rect positionFond, positionPage, positionAffichage, positionAccueil;
    SDL_Color couleurAccueil = {173, 0, 0};
    char mangaDispo[MANGA_MAX][30];
    int IDManga = 0, continuer = 0, i = 0, j = 0, dernierChapDispo[MANGA_MAX] = {0}, categorie[MANGA_MAX] = {0}, mangaChoisis = -2, extremesManga[2], chapitreChoisis = -2, sectionChoisie = -2;

    SDL_Rect** modes; //Use for have the screen resolution

    //Int default value
    positionFond.x = 0;
    positionFond.y = 0;
    positionPage.x = 0;
    positionPage.y = 0;
    positionAccueil.x = 0;
    positionAffichage.x = BORDURE_HORIZONTALE; //BORDURE_HORIZONTALE = horizontal strip who isolate manga page from corner
    positionAffichage.y = BORDURE_VERTICALE; //BORDURE_VERTICAL = vertical strip who isolate manga page from corner

    if (SDL_Init(SDL_INIT_VIDEO) == -1 || TTF_Init() == -1) //launch the SDL and check for failure
    {
        exit(EXIT_FAILURE);
    }

    modes = SDL_ListModes(NULL, SDL_FULLSCREEN|SDL_HWSURFACE); //Find screen resolution
    HAUTEUR = modes[0]->h - BARRE_DES_TACHES_WINDOWS; //Windows height = screen heigh - windows task bar

    //Clean variable
    for(i = 0; i < MANGA_MAX; i++)
    {
        for(j = 0; j < 30; j++)
        {
            mangaDispo[i][j] = 0;
        }
    }
    SDL_WM_SetIcon(SDL_LoadBMP("data/icone.bmp"), NULL); //Int icon for the main window
    ecran = SDL_SetVideoMode(800, HAUTEUR, 32, SDL_HWSURFACE | SDL_DOUBLEBUF); //Create main window
    SDL_WM_SetCaption("The Restorator - Manga reader - Coded by Taiki", NULL); //Change windows name
    fond = SDL_CreateRGBSurface(SDL_HWSURFACE, 800, HAUTEUR, 32, 0, 0 , 0, 0); //on initialise le fond
    SDL_FillRect(fond, NULL, SDL_MapRGB(ecran->format, 248, 205, 126)); //We change background color
    SDL_BlitSurface(fond, NULL, ecran, &positionFond); //Apply colored background on the window
    accueil = IMG_Load("data/acceuil.jpg"); //We load,the first picture
    positionAccueil.y = (HAUTEUR / 2) - (accueil->h / 2); //Center the first picture
    SDL_BlitSurface(accueil, NULL, ecran, &positionAccueil); //Blit first picture on the main window
    SDL_Flip(ecran); //Refresh screen
    while(continuer != -1 && continuer != 1) //While hasn't close main windows & accept to launching the reader
    {
        SDL_WaitEvent(&event); //Wait any user action

        switch(event.type)
        {
            case SDL_QUIT: //If the main window is closed
                continuer = -1;
                break;

            case SDL_KEYDOWN: //If a keyboard letter is pushed
            {
                switch(event.key.keysym.sym)
                {
                    case SDLK_RETURN: //If return
                    case SDLK_KP_ENTER:
                        continuer = 1; //quit the loop
                        break;

                    default: //If other one
                        break;
                }
                break;
            }
        }
    }

    if(continuer == 1) //If return has been pushed
    {
        SDL_BlitSurface(fond, NULL, ecran, &positionFond); //Blit the background for clean the windows
        check4update(ecran); //Launch database update function
        continuer = check(ecran, mangaDispo, dernierChapDispo, categorie); //Check than all files are here
        if(continuer != -1) //If check succed
        {
            while((sectionChoisie != -1 && sectionChoisie != 0) && mangaChoisis != -1 && chapitreChoisis != -1)//Big loop: while user hasn't try to close windows or wanted to up in the "option tree"
            {
                if (mangaChoisis == 0) //if just up in the "tree", restore default value
                    mangaChoisis = -2;
                sectionChoisie = -2;
                SDL_BlitSurface(fond, NULL, ecran, &positionFond);
                sectionChoisie = choixSection(ecran);
                while((sectionChoisie != -1 && sectionChoisie != 0) && (mangaChoisis != -1 && mangaChoisis != 0) && chapitreChoisis != -1)
                {
                    if (chapitreChoisis == 0)
                        chapitreChoisis = -2;
                    mangaChoisis = -2;
                    SDL_BlitSurface(fond, NULL, ecran, &positionFond);
                    mangaChoisis = choixManga(ecran, couleurAccueil, mangaDispo, categorie, sectionChoisie);
                    while((mangaChoisis != -1 && mangaChoisis != 0) && (chapitreChoisis != -1 && chapitreChoisis != 0))
                    {
                        SDL_BlitSurface(fond, NULL, ecran, &positionFond);
                        chapitreChoisis = choixChapitre(categorie, sectionChoisie, mangaChoisis, extremesManga, mangaDispo, couleurAccueil, ecran, &IDManga);
                        while(chapitreChoisis != -1 && chapitreChoisis != -2 && chapitreChoisis != 0)
                        {
                            SDL_BlitSurface(fond, NULL, ecran, NULL);
                            lecteur(&chapitreChoisis, mangaDispo, IDManga, ecran, extremesManga);
                        }
                    }
                }
            }
        }
    }

    SDL_FreeSurface(accueil);
    SDL_FreeSurface(fond);
    SDL_Quit();
    return 0;
}
