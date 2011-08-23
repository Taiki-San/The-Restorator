/*Cette liseuse a été developpé par Taiki, tout droits reservés.
L'utilisation de ce logiciel, sa modification et sa commercialisation sont interdites sans accord de Taiki*/

#include <stdlib.h>
#include <stdio.h>
#include <SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL.h>
#include <dir.h>

#define BORDURE_HORIZONTALE 20
#define BORDURE_VERTICALE 20
#define MANGA_MAX 30
#define DEPLACEMENT 50
#define BARRE_DES_TACHES_WINDOWS 75

extern int HAUTEUR;

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
void crashTemp(char temp[100]);
int choixChapitre(int categorie[MANGA_MAX], int sectionChoisie, int mangaChoisis, int extremesManga[2], char mangaDispo[MANGA_MAX][30], SDL_Color couleurAccueil, SDL_Surface *ecran, int *IDManga);
int lecteur(int *chapitreChoisis, char mangaDispo[MANGA_MAX][30], int IDManga, SDL_Surface *ecran, int extremesManga[2]);
void miseEnPause();
int check4update(SDL_Surface *ecran);
