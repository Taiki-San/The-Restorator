/*********************************************
**           								**
**           The Restorator v3.2.1          **
**     		 Released by 29/05/2012 		**
**           								**
**********************************************/

#include "main.h"

int updateDataBase()
{
    if(NETWORK_ACCESS == CONNEXION_DOWN)
        return -1;

    int i = 0, j = 0, k = 0, l = 0, buffer = 0, glad0s = 0, killswitche = 0;
    char teamLong[100], teamCourt[5], mode[5], URL[LONGUEUR_URL], temp[350], ID[10], siteTeam[LONGUEUR_SITE];
    char killswitch[100][100], buffer1[100], buffer2[100];
    FILE* DB = NULL;
    FILE* telechargement = NULL;
    FILE* mangaDispo = NULL;
    FILE* mangaDispo2 = NULL;
    FILE* repo2 = NULL;

    /*Module de censure... J'en suis pas fier mais necessaire.*/
    for(i = 0; i < 100; i++)
    {
        for(j = 0; j < 100; j++)
        {
            killswitch[i][j] = 0;
        }
    }
    mangaDispo = fopenR("mangas", "r");
    mangaDispo2 = fopenR("mangas.temp", "w+");
    repo2 = fopenR("repo.tmp", "w+");
    crashTemp(temp, 350);

    if(!NETWORK_ACCESS)
    {
        sprintf(temp, "http://www.%s/System/killswitch", MAIN_SERVER_URL[0]);
        telechargeur(temp, "config.dat", 0);
        telechargement = fopenR("config.dat", "r");
        fscanf(telechargement, "%d\n", &i);
        for(j = 0; j < i; j++)
            fscanf(telechargement, "%s\n", killswitch[j]);

        fclose(telechargement);
        removeR("config.dat");
    }

    /*Mise a jour de la BDD*/
    DB = fopenR("repo", "r");

	while((i = fgetc(DB)) != EOF && i != '#' && ID[0] != '#')
    {
        killswitche = 0;
        crashTemp(ID, 10);
        crashTemp(teamLong, 100);
        crashTemp(teamCourt, 5);
        crashTemp(mode, 5);
        crashTemp(URL, LONGUEUR_URL);
        crashTemp(siteTeam, LONGUEUR_SITE);

		fseek(DB, -1, SEEK_CUR);
        fscanf(DB, "%s %s %s %s %s", ID, teamLong, teamCourt, mode, URL);

        while((i = fgetc(DB)) != 'h' && i != '\n');
        if(i == 'h')
        {
            fseek(DB, -1, SEEK_CUR);
            fscanf(DB, "%s\n", siteTeam);
        }
#ifdef UNIX
		else if (ID[0] == '#' && ID[1] == 0)
			break;
#endif
        else
            while(fgetc(DB) != '\n');

        if(!NETWORK_ACCESS)
            for(i = 0; strcmp(killswitch[i], ID) != 0 && i < 100; i++);
        else
            i = 100;

        if(i != 100)
            killswitche = 1;

#ifdef UNIX
        if(!killswitche && ID[0] != '#') //Si le killswitch ne pose pas de problème
#else
        if(!killswitche)
#endif
        {
            /*On va mettre a jour le fichier repo et la base de données de mangas*/

            /*Téléchargement de repo*/
            crashTemp(temp, 350);
            if(strcmp(mode, "DB") == 0)
            {
                sprintf(temp, "http://dl.dropbox.com/u/%s/repo", URL);
            }
            else if(strcmp(mode, "O") == 0)
            {
                sprintf(temp, "http://%s/repo", URL);
            }
            telechargeur(temp, "config.dat", 0);

            /*Test du fichier*/
            telechargement = fopenR("config.dat", "r");
			if(fgetc(telechargement) != '<' && fgetc(telechargement) != EOF)
				i = 1;
			else
				i = 0;
            if(!i)
            {
                /*Si fichier corrompue, on récupère l'ancienne version*/
                /*On met le nom de la team*/

                if(siteTeam[0] != 0)
					fprintf(repo2, "%s %s %s %s %s %s\n", ID, teamLong, teamCourt, mode, URL, siteTeam);
				else
					fprintf(repo2, "%s %s %s %s %s\n", ID, teamLong, teamCourt, mode, URL);
            }
            else
            {
                /*Si le fichier est complet*/
                rewind(telechargement);

                /*On écrase les variables obsolètes*/
                crashTemp(temp, TAILLE_BUFFER);
                strcpy(temp, ID);
                crashTemp(ID, 10);
                crashTemp(teamLong, 100);
                crashTemp(teamCourt, 5);
                crashTemp(mode, 5);
                crashTemp(URL, LONGUEUR_URL);

                /*On récupère les infos*/
                fscanf(telechargement, "%s %s %s %s %s", ID, teamLong, teamCourt, mode, URL);
				fseek(telechargement, 1, SEEK_CUR);
				if(fgetc(telechargement) == 'h')
                {
                    fseek(telechargement, -1, SEEK_CUR);
                    fscanf(telechargement, " %s\n", siteTeam);
                }
                if(siteTeam[0] == 0 && strcmp(temp, ID) == 0)
                    fprintf(repo2, "%s %s %s %s %s\n", ID, teamLong, teamCourt, mode, URL);
                else if (strcmp(temp, ID) == 0)
                    fprintf(repo2, "%s %s %s %s %s %s\n", ID, teamLong, teamCourt, mode, URL, siteTeam);
                else
                    killswitche = 1;
            }
            fclose(telechargement);
            removeR("config.dat");

            if(!killswitche)
            {
                /*Téléchargement de la BDD disponible*/
                crashTemp(temp, 350);
                if(strcmp(mode, "DB") == 0)
                {
                    sprintf(temp, "http://dl.dropbox.com/u/%s/mangas", URL);
                }
                else if(strcmp(mode, "O") == 0)
                {
                    sprintf(temp, "http://%s/mangas", URL);
                }

                if(i)
                {
                    telechargeur(temp, "config.dat", 0);

                    /*Test du fichier*/
                    telechargement = fopenR("config.dat", "r");
                    for(i = 0; i < 30000 && fgetc(telechargement) != EOF; i++)
                    {
                        fseek(telechargement, -1, SEEK_CUR);
                        if(fgetc(telechargement) == '#')
                            i = 50000;
                    }
                }
                else
                    i = 50001; //si repo corrompue, on n'actualise pas mangas (dépôt down)

                if(i < 50000)
                {
                    /*Si fichier corrompu, on récupère l'ancienne version*/
                    /*On met le nom de la team*/
                    fprintf(mangaDispo2, "%s %s\n", teamLong, teamCourt);
                    while(strcmp(teamCourt, buffer1) != 0) //On se positionne pour récupérer les anciennes données
                    {
                        crashTemp(buffer1, 100);
                        fscanf(mangaDispo, "%s\n", buffer1);
                    }
                    while (fgetc(mangaDispo) != '#') //On copie jusqu'au # final
                    {
                        fseek(mangaDispo, -1, SEEK_CUR);
                        crashTemp(buffer1, 100);
                        crashTemp(buffer2, 100);
                        fscanf(mangaDispo, "%s %s %d %d %d %d\n", buffer1, buffer2, &l, &j, &buffer, &glad0s);
                        fprintf(mangaDispo2, "%s %s %d %d %d %d\n", buffer1, buffer2, l, j, buffer, glad0s);
                    }
                }
                else
                {
                    /*Si le fichier est complet*/
                    rewind(telechargement); //On repart du début
                    crashTemp(teamLong, 100); //On crash tout
                    crashTemp(teamCourt, 5);
                    fscanf(telechargement, "%s %s\n", teamLong, teamCourt); //Copie du nom de la team
                    fprintf(mangaDispo2, "%s %s\n", teamLong, teamCourt);
                    while (fgetc(telechargement) != '#') //Copie des mangas
                    {
                        fseek(telechargement, -1, SEEK_CUR);
                        crashTemp(buffer1, 100);
                        crashTemp(buffer2, 100);
                        fscanf(telechargement, "%s %s %d %d %d %d\n", buffer1, buffer2, &l, &j, &buffer, &k);
                        fprintf(mangaDispo2, "%s %s %d %d %d %d\n", buffer1, buffer2, l, j, buffer, k);
                    }
                }
                fprintf(mangaDispo2, "#\n");
                fclose(telechargement);
                removeR("config.dat"); //Suppression du fichier mangas DL
            }
        }
        /*Si killswitch activé, les données ne seront pas réécrites*/
        else
            killswitchEnabled(teamCourt); //On sort l'arme atomique
    }
    fprintf(repo2, "#");
    fclose(repo2);
    fclose(DB);
    fclose(mangaDispo);
    fclose(mangaDispo2);
    removeR("mangas");
    removeR("repo");
    renameR("mangas.temp", "mangas");
    renameR("repo.tmp", "repo");
    return -1;
}

int ecritureDansImport(char mangaDispoLong[LONGUEUR_NOM_MANGA_MAX], char mangaDispoCourt[LONGUEUR_COURT], int chapitreChoisis, char teamsCourt[LONGUEUR_COURT])
{
    FILE* fichier = NULL;
    FILE* test = NULL;
    char temp[TAILLE_BUFFER];
    int dernierChap = 0, i = 0, j = 0, nombreChapitre = 0;

    /*On ouvre le fichier d'import*/
    fichier = fopenR("update/import.dat", "a+");

    if(chapitreChoisis != 0)
    {
		nombreChapitre = 1;
        /*On test l'existance du fichier (zipé ou dézipé)*/
        crashTemp(temp, TAILLE_BUFFER);
        sprintf(temp, "manga/%s/Chapitre_%d/config.dat", mangaDispoLong, chapitreChoisis);
        test = fopenR(temp, "r");
        crashTemp(temp, TAILLE_BUFFER);
        sprintf(temp, "manga/%s/Chapitre_%d.zip", mangaDispoLong, chapitreChoisis);
        if(test == NULL)
        {
            test = fopenR(temp, "r");
            if(test == NULL)
            {
                fprintf(fichier, "\n%s %s %d", teamsCourt, mangaDispoCourt, chapitreChoisis);
            }
            else
            {
                fclose(test);
            }
        }
        else
            fclose(test);
    }
    else
    {
        test = fopenR("mangas", "r");
        while(strcmp(temp, mangaDispoCourt))
        {
            crashTemp(temp, TAILLE_BUFFER);
            fscanf(test, "%s", temp);
        }
        fscanf(test, "%d %d", &j, &dernierChap);
        fclose(test);
        for(i = j; i <= dernierChap; i++)
        {
            crashTemp(temp, TAILLE_BUFFER);
            sprintf(temp, "manga/%s/Chapitre_%d/config.dat", mangaDispoLong, i);
            test = fopenR(temp, "r");
            crashTemp(temp, TAILLE_BUFFER);
            sprintf(temp, "manga/%s/Chapitre_%d.zip", mangaDispoLong, i);
            if(test == NULL)
            {
                test = fopenR(temp, "r");
                if(test == NULL)
                {
                    fprintf(fichier, "\n%s %s %d", teamsCourt, mangaDispoCourt, i);
					nombreChapitre++;
                }
                else
                {
                    fclose(test);
                }
            }
            else
                fclose(test);
        }
    }
    fclose(fichier);
	return nombreChapitre;
}

