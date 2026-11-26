#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void MaxEtMinB(int *tab, int n, int *max, int *min, long long int *cpt)
{
    if (n <= 0)
    {
        return;
    }

    if (n == 1)
    {
        *max = tab[0];
        *min = tab[0];
        return;
    }

    int i = 0;
    while (i + 1 < n)
    {
        *cpt = *cpt + 1;
        if (tab[i] > tab[i + 1])
        {
            int tmp = tab[i];
            tab[i] = tab[i + 1];
            tab[i + 1] = tmp;
        }
        i = i + 2;
    }

    *min = tab[0];
    int k = 2;
    while (k + 1 < n)
    {
        *cpt = *cpt + 1;
        if (tab[k] < *min)
        {
            *min = tab[k];
        }
        k = k + 2;
    }

    *max = tab[1];
    k = 3;
    while (k < n)
    {
        *cpt = *cpt + 1;
        if (tab[k] > *max)
        {
            *max = tab[k];
        }
        k = k + 2;
    }

    if (n % 2 != 0)
    {
        *cpt = *cpt + 1;
        if (tab[n - 1] < *min)
        {
            *min = tab[n - 1];
        }

        *cpt = *cpt + 1;
        if (tab[n - 1] > *max)
        {
            *max = tab[n - 1];
        }
    }
}

int main(int argc, char *argv[])
{
    int sizes[14] = {100000, 200000, 400000, 600000, 800000, 1000000,
                     1200000, 1400000, 1600000, 1800000, 2000000,
                     4000000, 6000000, 8000000};

    FILE *f_result = fopen("B2_resultat.csv", "w+");
    if (f_result == NULL)
    {
        perror("Erreur ouverture fichier resultat\n");
        return EXIT_FAILURE;
    }

    fprintf(f_result, "n,max,min,comparaisons,temps_ms\n");

    for (int s = 0; s < 14; s++)
    {
        int n = sizes[s];
        int *tab = malloc(n * sizeof(int));
        if (tab == NULL)
        {
            perror("Erreur allocation memoire\n");
            return EXIT_FAILURE;
        }

        FILE *f_data = fopen("../Data/tab_non_trie.txt", "r");
        if (f_data == NULL)
        {
            f_data = fopen("Data/tab_non_trie.txt", "r");
        }
        if (f_data == NULL)
        {
            f_data = fopen("tab_non_trie.txt", "r");
        }
        if (f_data == NULL)
        {
            perror("Erreur ouverture du fichier tab_non_trie.txt\n");
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

        int max, min;
        long long int cpt = 0;

        clock_t debut = clock();
        MaxEtMinB(tab, n, &max, &min, &cpt);
        clock_t fin = clock();

        double temps_ms = (double)(fin - debut) * 1000.0 / CLOCKS_PER_SEC;
        fprintf(f_result, "%d,%d,%d,%lld,%.9f\n", n, max, min, cpt, temps_ms);

        free(tab);
    }

    fclose(f_result);
    printf("Resultats ecrits dans B2_resultat.csv avec succes.\n");

    return EXIT_SUCCESS;
}
