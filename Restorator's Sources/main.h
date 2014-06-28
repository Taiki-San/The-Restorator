/*********************************************
**                                          **
**           The Restorator v3.2.1          **
**           Released by 29/05/2012         **
**                                          **
**********************************************/

/**OS**/
#define WINDOWS

#ifndef WINDOWS
    #define UNIX
#endif

#ifdef WINDOWS
    #include <dir.h>
    #include <windows.h>
#endif

#ifdef UNIX
    #include <pthread.h>
    #include <unistd.h>
    #define MACOS

    #ifndef MACOS
        #include <sys/stat.h>
        #include <sys/file.h>
        #include <sys/wait.h>
        #include <sys/types.h>
    #else
        #define MACOS_OLD
    #endif
#endif


/*Prototypes*/
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <unistd.h>
#include <curl/curl.h>

#ifdef MACOS
#include <SDL_Image/SDL_image.h>
#include <SDL_TTF/SDL_ttf.h>
#else
#include <SDL_image.h>
#include <SDL_ttf.h>
#endif

/*DEFINES*/

/*Général*/
#define LARGEUR 800 //Largeur par défaut
#define HAUTEUR 730
#define HAUTEUR_INTERDIT_WHILE_DL 400
#define HAUTEUR_MAX_LEGALE 1000
#define BARRE_DES_TACHES_WINDOWS 75
#define CURRENTVERSION 321
#define PERMISSIONS 0755
#define TAILLE_BUFFER 100

/*Réseau*/
#define CONNEXION_OK 0
#define CONNEXION_SERVEUR_DOWN 1
#define CONNEXION_DOWN 2

/*Langue*/
#define NOMBRE_LANGUE 4
#define LANGUE_PAR_DEFAUT 2
#define LARGEUR_LANGUE 800
#define HAUTEUR_LANGUE 400
#define HAUTEUR_MENU_LANGUE 30
#define HAUTEUR_TEXTE_LANGUE 100
#define INTERLIGNE_LANGUE 10

/*Police*/
#define POLICE_TOUT_PETIT 16
#define POLICE_PETIT 18
#define POLICE_MOYEN 22
#define POLICE_GROS 26

/*Couleurs*/
#define POLICE_R 0
#define POLICE_G 0
#define POLICE_B 0
#define POLICE_NEW_R 255
#define POLICE_NEW_G 47
#define POLICE_NEW_B 47
#define FOND_R 214
#define FOND_G 214
#define FOND_B 214

/*Section*/
#define NOMBRESECTION 6
#define SECTION_DL 4
#define BORDURE_SUP_MENU 50
#define INTERLIGNE_MENU 20
#define BORDURE_VERTICALE_SECTION 75
#define BORDURE_SUP_SECTION 150
#define INTERLIGNE_SECTION 25
#define HAUTEUR_FENETRE_SECTION 500

/*Selection manga*/
#define BORDURE_SUP_TITRE_MANGA 30
#define LARGEUR_MOYENNE_MANGA_PETIT 19
#define LARGEUR_MOYENNE_MANGA_GROS 27
#define BORDURE_SUP_SELEC_MANGA 140
#define MANGAPARPAGE 30
#define INTERLIGNE 35
#define MINIINTERLIGNE 15
#define NBRCOLONNES 3
#define LONGUEURTEXTE 100
#define LONGUEURMANGA 200
#define BORDURELATSELECTION 45
#define HAUTEURNUMEROTATION_OLD 50
#define HAUTEURNUMEROTATION_NEW 50
#define BANDEAU_INF_NEW 20
#define BORDURE_INF_NUMEROTATION_NSM 540
#define HAUTEUR_BOUTONS_DL 575
#define NOMBRE_LETTRE_MAX 20

/*Multi Page*/
#define HAUTEUR_CHANGEMENT_PAGE 100
#define LONGUEUR_PRECENDENT 169
#define LONGUEUR_SUIVANT 141

/*Chapitre*/
#define NOMBRE_CHAPITRE_MAX 300
#define BORDURE_SUP_TITRE_CHAPITRE 20
#define BORDURE_SUP_INFOS_MANGA 150
#define BORDURE_SUP_CHAPITRE 225
#define BORDURE_SUP_NUMEROTATION_CHAPITRE 450
#define BORDURE_SUP_INFOS_TEAM_CHAPITRE 60
#define HAUTEUR_BOUTONS_CHAPITRE 585
#define BORDURE_BOUTON_LECTEUR 20
#define SEPARATION_COLONNE_1_CHAPITRE 220
#define SEPARATION_COLONNE_2_CHAPITRE 570
#define CODE_CLIC_LIEN_CHAPITRE 13371337
#define CODE_BOUTON_1_CHAPITRE 13371338
#define CODE_BOUTON_2_CHAPITRE 13371339
#define CODE_BOUTON_3_CHAPITRE 13371340
#define SECTION_CHOISIS_CHAPITRE 7

/**Lecteur**/
#define DEPLACEMENT 50
#define NOMBRE_PAGE_MAX 500
#define LONGUEUR_NOM_PAGE 100
#define BORDURE_LAT_LECTURE 20
#define BORDURE_HOR_LECTURE 40
#define DEPLACEMENT_ANIMATION 20
#define DEPLACEMENT_ANIMATION_FULLSCREEN 35
#define BORDURE_CONTROLE_LECTEUR 100
#define DEPLACEMENT_LATERAL_PAGE 5
#define DEPLACEMENT_HORIZONTAL_PAGE 5

/*Explications*/
#define INTERLIGNE_EXPLICATION 5
#define BORDURE_SUP_EXPLICATION 10
#define LARGEUR_EXPLICATION 250
#define HAUTEUR_EXPLICATION 110
#define DUREE_ARRICHAGE_EXPLCIATION 4000

/*Mangas*/
#define NOMBRE_MANGA_MAX 1000
#define LONGUEUR_NOM_MANGA_MAX 50
#define LONGUEUR_COURT 6
#define LONGUEUR_SITE 100

/*Check environnement*/
#define NOMBRE_DE_FICHIER_A_CHECKER 21
#define LONGUEUR_NOMS_DATA 100

/*Show numéro*/
#define HAUTEUR_BORDURE_AFFICHAGE_NUMERO 50

/*Module Gestion*/
#define NOMBRE_MENU_GESTION 3
#define HAUTEUR_TEXTE 30
#define HAUTEUR_CHOIX 100
#define NOMBRE_MENU 5
#define HAUTEUR_SELECTION_REPO 350

/*Ajout repo*/
#define BUFFER_MAX 100
#define HAUTEUR_MENU_AJOUT_REPO 50
#define HAUTEUR_FENETRE_AJOUT_REPO_INIT 200
#define HAUTEUR_FENETRE_AJOUT_REPO 250
#define HAUTEUR_ID_AJOUT_REPO 150
#define HAUTEUR_TEAM_AJOUT_REPO 175
#define LONGUEUR_ID_MAX 6

/*Suppression repo*/
#define HAUTEUR_DEL_REPO 275
#define HAUTEUR_MENU_CONFIRMATION_SUPPRESSION 50
#define HAUTEUR_TEAM_CONFIRMATION_SUPPRESSION 125
#define HAUTEUR_CONSIGNES_CONFIRMATION_SUPPRESSION 200
#define HAUTEUR_LISTE 100

/*Module update*/
#define HAUTEUR_MESSAGE_INITIALISATION 50
#define HAUTEUR_TEXTE_TELECHARGEMENT 125
#define HAUTEUR_POURCENTAGE 200
#define BORDURE_POURCENTAGE 100
#define HAUTEUR_FENETRE_DL 250
#define HAUTEUR_AFFICHAGE 150
#define LONGUEUR_URL 300

/*Mise à jour*/
#define HAUTEUR_MAJ 250

extern int HAUTEUR_MAX; //Hauteur maxiamle (généralement écran)
extern int LARGEUR_MAX; //Largeur maximale (1000 si je dis pas de bétise)
extern int RESOLUTION[2]; //Résolution
extern int langue; //Langue
extern int NETWORK_ACCESS;
extern int PROXY_LYCEE;
extern char REPERTOIREEXECUTION[350];
extern char BUNDLENAME[100];
extern char FONTUSED[100];
extern char MAIN_SERVER_URL[2][100];
extern char LANGUAGE_PATH[NOMBRE_LANGUE][50];
extern SDL_Surface *ecran;
extern SDL_Surface *fond;

/**Structures**/

typedef struct argmt
{
    char URL[300];
    char repertoireEcriture[100];
    int* status;
} ARGUMENT;

typedef struct infos_Team
{
    char ID[LONGUEUR_ID_MAX];
    char teamLong[LONGUEUR_NOM_MANGA_MAX];
    char teamCourt[LONGUEUR_COURT];
    char mode[BUFFER_MAX];
    char URL[LONGUEUR_URL];
    char site[LONGUEUR_SITE];
} INFOS_TEAMS;

typedef struct data_pour_installation
{
    char teamCourt[LONGUEUR_COURT];
    char mangaCourt[LONGUEUR_COURT];
    int chapitre;
} DATA_INSTALL;

#ifdef UNIX

/**UNIX only**/
#define handle_error_en(en, msg) \
   do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

#define handle_error(msg) \
   do { perror(msg); exit(EXIT_FAILURE); } while (0)

struct thread_info {        /* Used as argument to thread_start() */
pthread_t thread_id;        /* ID returned by pthread_create() */
int       thread_num;       /* Application-defined thread # */
char     *argv_string;      /* From command-line argument */
};
#endif


/**Prototypes**/

/**Main**/
int mainLecture(int sectionChoisis);
int mainChoixDL();
void mainDL();
void mainRestorator();

/**Fonctions principales**/
int ecranAccueil();
int section();
int showControls();
int manga(int sectionChoisis, int sectionManga[NOMBRE_MANGA_MAX], char mangaDispo[NOMBRE_MANGA_MAX][LONGUEUR_NOM_MANGA_MAX], int nombreChapitre);
int chapitre(char team[LONGUEUR_NOM_MANGA_MAX], char mangaSoumis[LONGUEUR_NOM_MANGA_MAX], int mode);
int lecteur(int *chapitreChoisis, int *fullscreen, char mangaDispo[LONGUEUR_NOM_MANGA_MAX], char team[LONGUEUR_NOM_MANGA_MAX]);
int update();
int menuGestion();

/**Fonction secondaires mais majeurs**/
void miseEnCache(char mangaDispo[NOMBRE_MANGA_MAX][LONGUEUR_NOM_MANGA_MAX], char mangaDispoCourt[NOMBRE_MANGA_MAX][LONGUEUR_COURT], int categorie[NOMBRE_MANGA_MAX], int premierChapitreDispo[NOMBRE_MANGA_MAX], int dernierChapitreDispo[NOMBRE_MANGA_MAX], char teamsLong[NOMBRE_MANGA_MAX][LONGUEUR_NOM_MANGA_MAX], char teamsCourt[NOMBRE_MANGA_MAX][LONGUEUR_COURT], int mode);
int mangaSelection(int mode, int tailleTexte[MANGAPARPAGE], int *manuel);
int oldShowManga(TTF_Font *police, char mangaDispo[NOMBRE_MANGA_MAX][LONGUEUR_NOM_MANGA_MAX], int sectionChoisis, int nombreMangaElligible, int mangaElligibles[NOMBRE_MANGA_MAX]);
int newShowManga(char mangaDispo[NOMBRE_MANGA_MAX][LONGUEUR_NOM_MANGA_MAX], int sectionChoisis, int nombreMangaElligible, int mangaElligibles[NOMBRE_MANGA_MAX], int nombreChapitre, int hauteurAffichage);
int checkProjet(char projet[LONGUEUR_NOM_MANGA_MAX]);

/**Section Gestion**/
int confirmation(char team[LONGUEUR_NOM_MANGA_MAX]);
void raffraichissmenent();
int changementLangue();
int deleteManga();

/**Fonctions liées aux repos**/
int ajoutRepo();
int deleteRepo();

/**Fonction du telechargement**/
size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream);
int telechargeur(char *adresse, char *repertoire, int activation);
#ifdef WINDOWS
DWORD WINAPI downloader(LPVOID envoi);
#else
void* downloader(void* envoi);
#endif
int downloadData(void* ptr, double TotalToDownload, double NowDownloaded, double TotalToUpload, double NowUploaded);

/**Module de téléchargement**/
int DLmanager();
#ifdef WINDOWS
DWORD WINAPI installation(LPVOID datas);
DWORD WINAPI telechargement();
#else
void* installation(void* datas);
void* telechargement();
#endif

/**Checks divers**/
int check();
int checkUpdate();
void networkAndVersionTest();
int checkLancementUpdate();
#ifdef UNIX
int checkPID(int PID);
#endif
int updateDataBase();
int checkRestore();

/**Fonctions Utilitaires**/
void crashTemp(char *temp, int longueur);
void conversionAvant(char mangaDispo[NOMBRE_MANGA_MAX][LONGUEUR_NOM_MANGA_MAX]);
void conversionApres(char mangaDispo[NOMBRE_MANGA_MAX][LONGUEUR_NOM_MANGA_MAX]);
int isItNew(char mangaATester[LONGUEUR_NOM_MANGA_MAX]);
int ecritureDansImport(char mangaDispoLong[LONGUEUR_NOM_MANGA_MAX], char mangaDispoCourt[LONGUEUR_COURT], int chapitreChoisis, char teamsCourt[LONGUEUR_COURT]);
int lancementModuleDL(char cheminDAcces[100]);
void removeFolder(char *path, int longueur);
void lectureTrad(char trad[][100], int IDTrad);
void anythingNew(int extremes[2], char mangaChoisis[LONGUEUR_NOM_MANGA_MAX]);
void lastChapitreLu(char nomManga[LONGUEUR_NOM_MANGA_MAX], int dernierChapitre);
int compteNombreDL();
void restartEcran();
int getLetterPushed(SDL_Event event);
void ouvrirSite(char team[LONGUEURMANGA]);
void killswitchEnabled(char nomTeamCourt[5]);
void getBundleName(char *argv);
void unzip(char *path, char *output);

/**Fonctions d'entrées**/
int waitClavier(int nombreMax, char *retour);
int nombreEntree(SDL_Event event);
int waitEnter();

/**Fonction d'affichage**/
void showNumero(TTF_Font *police, int choix, int hauteurNum);
int showError();
void initialisationAffichage();
int erreurReseau();
int affichageMenuGestion();
void affichageLancement();
int affichageRepoIconnue();
void chargement();
int interditWhileDL();
int rienALire();

/**Fonctions remplacant les natives**/
void mkdirR(char *path);
void removeR(char *path);
FILE* fopenR(char *path, char *right);
void renameR(char *initialName, char *newName);

/**Classement alpahbétique**/
int compare (void const *a, void const *b);
