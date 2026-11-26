#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void MaxEtMinA(int *tab, int n, int *max, int *min, long long int *cpt)
{
    if (n <= 0)
    {
        return;
    }

    *max = tab[0];
    *min = tab[0];

    int i = 1;
    while (i < n)
    {
        *cpt = *cpt + 1;
        if (tab[i] > *max)
        {
            *max = tab[i];
        }

        *cpt = *cpt + 1;
        if (tab[i] < *min)
        {
            *min = tab[i];
        }

        i = i + 1;
    }
}

int main(int argc, char *argv[])
{
    int sizes[14] = {100000, 200000, 400000, 600000, 800000, 1000000,
                     1200000, 1400000, 1600000, 1800000, 2000000,
                     4000000, 6000000, 8000000};

    FILE *f_result = fopen("B1_resultat.csv", "w+");
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
        MaxEtMinA(tab, n, &max, &min, &cpt);
        clock_t fin = clock();

        double temps_ms = (double)(fin - debut) * 1000.0 / CLOCKS_PER_SEC;
        fprintf(f_result, "%d,%d,%d,%lld,%.9f\n", n, max, min, cpt, temps_ms);

        free(tab);
    }

    fclose(f_result);
    printf("Resultats ecrits dans B1_resultat.csv avec succes.\n");

    return EXIT_SUCCESS;
}
