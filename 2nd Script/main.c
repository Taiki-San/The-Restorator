#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>

int main(int argc, char *argv[])
{
    SDL_Surface *pageTest = NULL;
    FILE *config = NULL, *transfert = NULL;
    char temp[100] = {0};
    int max = 0, actuel = 0, i = 0, j = 0, largeur = 0, hauteur = 0, wait = 0;

    if(SDL_Init(SDL_INIT_VIDEO) != -1)
    {
        transfert = fopen("config.dat.temp", "w");
        config = fopen("config.dat", "r+");
        fscanf(config, "%d", &max);
        fprintf(transfert, "%d\n", max);
        for(i = 0; actuel < max; i++)
        {
            for(j = 0; j < 100; j++)
            {
                temp[j] = 0;
            }
            fscanf(config, "%d %s", &actuel, temp);
            fprintf(transfert, "%d %s", actuel, temp);
            for(j = 0; j < 100; j++)
            {
                if(temp[j] == '&')
                {
                    temp[j] = ' ';
                }
            }
            pageTest = IMG_Load(temp);
            for(wait = 0; wait < 100; wait++);
            if (pageTest == NULL)
            {
                 fprintf(stderr,"Erreur, image %s non chargée\n", temp);
                 fprintf(transfert, "\n");
                 continue;
            }
            largeur = pageTest->w;
            hauteur = pageTest->h;
            fprintf(transfert, " %d %d\n", largeur, hauteur);
        }
    }
    fclose(config);
    remove("config.dat");
    fclose(transfert);
    rename("config.dat.temp", "config.dat");
    SDL_FreeSurface(pageTest);
    SDL_Quit();

    return 0;
}

