#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int rechElets_TabTries(int *tab, int n, int x, long long int *cpt)
{
    int i = 0;

    while (i < n)
    {
        *cpt = *cpt + 1;
        if (tab[i] == x)
        {
            return 1;
        }
        if (tab[i] > x)
        {
            return 0;
        }
        i = i + 1;
    }

    return 0;
}

int main(int argc, char *argv[])
{
    int sizes[10] = {100000, 200000, 400000, 600000, 800000,
                     1000000, 1200000, 1400000, 1600000, 1800000};

    FILE *f_result = fopen("A2_resultat.csv", "w+");
    if (f_result == NULL)
    {
        perror("Erreur ouverture fichier resultat\n");
        return EXIT_FAILURE;
    }

    fprintf(f_result, "n,cas,x,comparaisons,temps_ms\n");

    for (int s = 0; s < 10; s++)
    {
        int n = sizes[s];
        int *tab = malloc(n * sizeof(int));
        if (tab == NULL)
        {
            perror("Erreur allocation memoire\n");
            return EXIT_FAILURE;
        }

        FILE *f_data = fopen("../Data/tab_trie.txt", "r");
        if (f_data == NULL)
        {
            f_data = fopen("Data/tab_trie.txt", "r");
        }
        if (f_data == NULL)
        {
            f_data = fopen("tab_trie.txt", "r");
        }
        if (f_data == NULL)
        {
            perror("Erreur ouverture du fichier tab_trie.txt\n");
            free(tab);
            return EXIT_FAILURE;
        }

        for (int i = 0; i < n; i++)
        {
            if (fscanf(f_data, "%d", &tab[i]) != 1)
            {
                break;
            }
        }
        fclose(f_data);

        int x_meilleur = tab[0];
        long long int cpt_meilleur = 0;

        clock_t debut = clock();
        rechElets_TabTries(tab, n, x_meilleur, &cpt_meilleur);
        clock_t fin = clock();

        double temps_meilleur = (double)(fin - debut) * 1000.0 / CLOCKS_PER_SEC;
        fprintf(f_result, "%d,Meilleur cas,%d,%lld,%.9f\n", n, x_meilleur, cpt_meilleur, temps_meilleur);

        int x_pire = tab[n - 1] + 1;
        long long int cpt_pire = 0;

        debut = clock();
        rechElets_TabTries(tab, n, x_pire, &cpt_pire);
        fin = clock();

        double temps_pire = (double)(fin - debut) * 1000.0 / CLOCKS_PER_SEC;
        fprintf(f_result, "%d,Pire cas,%d,%lld,%.9f\n", n, x_pire, cpt_pire, temps_pire);

        free(tab);
    }

    fclose(f_result);
    printf("Resultats ecrits dans A2_resultat.csv avec succes.\n");

    return EXIT_SUCCESS;
}
