/*Cette liseuse a été developpé par Taiki, tout droits reservés.
L'utilisation de ce logiciel, sa modification et sa commercialisation sont interdites sans accord de Taiki*/

#include <stdlib.h>
#include <stdio.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL.h>
#include <dir.h>
#include <windows.h>
#include <unistd.h>

/*Général*/
#define BARRE_DES_TACHES_WINDOWS 75
#define LARGEUR 800
#define CURRENTVERSION 200
#define NOTATSCHOOL
//#define VERBOSE
/*Défini la langue*/
#define FRENCH
//#define ENGLISH

/*Pour le lecteur*/
#define BORDURE_HORIZONTALE 40
#define BORDURE_VERTICALE 20
#define DEPLACEMENT 50
#define DEPLACEMENT_ANIMATION 10

/*Pour le selectionneur*/
#define MANGA_MAX 50
#define MANGAPARPAGE 30
#define LARGEURTEXTE 30
#define LONGUEURTEXTE 200
#define INTERLIGNE 35
#define MINIINTERLIGNE 10
#define BORDURESUPSELECTION 150
#define NBRCOLONNES 3
#define BORDURELATSELECTION 35
#define BORDURECOLONNE 65
#define HAUTEURNUMEROTATION 550

/*Pour le module de mise à jour*/
#define DROPBOX_ID_MMT 37363628

extern int HAUTEUR;
extern char REPERTOIREEXECUTION[250];

struct page
{
    int numeroPage;
    char nomPage[50];
    int largeurP;
    int hauteurP;
    int pageTotal;
};

int check(SDL_Surface *ecran, char mangaDispo[MANGA_MAX][30], int dernierChapDispo[MANGA_MAX], int categorie[MANGA_MAX]);
int choixSection(SDL_Surface *ecran);
int choixManga(SDL_Surface *ecran, SDL_Color couleurAccueil, char mangaDispo[MANGA_MAX][30], int categorie[MANGA_MAX], int sectionChoisie);
void crashTemp(char *temp, int longueur);
int choixChapitre(int categorie[MANGA_MAX], int sectionChoisie, int mangaChoisis, int extremesManga[2], char mangaDispo[MANGA_MAX][30], SDL_Color couleurAccueil, SDL_Surface *ecran, int *IDManga);
int lecteur(int *chapitreChoisis, char mangaDispo[MANGA_MAX][30], int IDManga, SDL_Surface *ecran, int extremesManga[2], SDL_Color couleurAccueil);
int check4update(SDL_Surface *ecran);
int donwloadChapter(char mangaDispo[MANGA_MAX][30], int IDManga, int chapitreADL, int extremeManga[2], SDL_Surface *ecran, SDL_Surface *accueil, SDL_Color couleurAccueil);
int addManga(SDL_Surface *ecran, SDL_Surface *accueil, SDL_Color couleurAccueil);

