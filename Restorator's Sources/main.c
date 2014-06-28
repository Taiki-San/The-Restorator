/*********************************************
**                                          **
**           The Restorator v3.2.1          **
**           Released by 29/05/2012         **
**                                          **
**********************************************/

#include "main.h"

int HAUTEUR_MAX = HAUTEUR_MAX_LEGALE;
int LARGEUR_MAX = 1260;
int langue = 0;
int RESOLUTION[2];
int PROXY_LYCEE = 0;
int NETWORK_ACCESS = CONNEXION_OK;
char REPERTOIREEXECUTION[350] = {0};
char BUNDLENAME[100] = {0}; //Si sous MACOS, les dossier sont dans le .app, le path est donc à modifier
char FONTUSED[100] = "data/font.ttf";
char MAIN_SERVER_URL[2][100];
char LANGUAGE_PATH[NOMBRE_LANGUE][50];
SDL_Surface *ecran = NULL;
SDL_Surface *fond = NULL;

int main(int argc, char *argv[])
{
    sprintf(MAIN_SERVER_URL[0], "the-restorator.com");
    sprintf(MAIN_SERVER_URL[1], "http://www.google.com/Restorator");

    sprintf(LANGUAGE_PATH[0], "french");
    sprintf(LANGUAGE_PATH[1], "english");
    sprintf(LANGUAGE_PATH[2], "italian");
    sprintf(LANGUAGE_PATH[3], "german");

    int i = 0;
    FILE* test = NULL;

    getBundleName(argv[0]);
    getcwd(REPERTOIREEXECUTION, sizeof(REPERTOIREEXECUTION));

    i = checkLancementUpdate();

    test = fopenR("data/langue", "r");
    if(test != NULL)
    {
        fscanf(test, "%d", &langue);
        fclose(test);
    }

    /*Launching SDL & SDL_TTF*/
    if(SDL_Init(SDL_INIT_VIDEO) == -1) //launch the SDL and check for failure
        exit(EXIT_FAILURE);
    if(TTF_Init() == -1)
        exit(EXIT_FAILURE);

    #ifdef MACOS
	char temp[100];
    sprintf(temp, "%sdata/icone.png", BUNDLENAME);
    SDL_WM_SetIcon(IMG_Load(temp), NULL); //Int icon for the main window
    #else
    SDL_WM_SetIcon(IMG_Load("data/icone.png"), NULL); //Int icon for the main window
    #endif

    if(!i) //Si il n'y a pas d'installation a faire ou qu'elle est en cours.
    {
        if(!checkUpdate()) //Vérifie si MaJ à lancer que si on est pas en train de lancer un DL de mangas
            mainRestorator();
    }

    else if (i) //Si il faut lancer le DL
        /**Tout le controleur du DL est concentré ici**/
        mainDL();

    SDL_FreeSurface(fond);
    SDL_FreeSurface(ecran);
    SDL_Quit();
    TTF_Quit();
    return 0;
}

