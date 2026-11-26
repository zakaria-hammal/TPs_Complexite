#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct fichier_tp fichier_tp;
struct fichier_tp
{
    FILE *f;
    char filename[100];
    int nb_nombres;
};

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
    fichier_tp files[14] = {
        {
            fopen("../Data/tab_non_trie_100000.txt", "r+"),
            "tab_non_trie_100000.txt",
            100000
        },
        {
            fopen("../Data/tab_non_trie_200000.txt", "r+"),
            "tab_non_trie_200000.txt",
            200000
        },
        {
            fopen("../Data/tab_non_trie_400000.txt", "r+"),
            "tab_non_trie_400000.txt",
            400000
        },
        {
            fopen("../Data/tab_non_trie_600000.txt", "r+"),
            "tab_non_trie_600000.txt",
            600000
        },
        {
            fopen("../Data/tab_non_trie_800000.txt", "r+"),
            "tab_non_trie_800000.txt",
            800000
        },
        {
            fopen("../Data/tab_non_trie_1000000.txt", "r+"),
            "tab_non_trie_1000000.txt",
            1000000
        },
        {
            fopen("../Data/tab_non_trie_1200000.txt", "r+"),
            "tab_non_trie_1200000.txt",
            1200000
        },
        {
            fopen("../Data/tab_non_trie_1400000.txt", "r+"),
            "tab_non_trie_1400000.txt",
            1400000
        },
        {
            fopen("../Data/tab_non_trie_1600000.txt", "r+"),
            "tab_non_trie_1600000.txt",
            1600000
        },
        {
            fopen("../Data/tab_non_trie_1800000.txt", "r+"),
            "tab_non_trie_1800000.txt",
            1800000
        },
        {
            fopen("../Data/tab_non_trie_2000000.txt", "r+"),
            "tab_non_trie_2000000.txt",
            2000000
        },
        {
            fopen("../Data/tab_non_trie_4000000.txt", "r+"),
            "tab_non_trie_4000000.txt",
            4000000
        },
        {
            fopen("../Data/tab_non_trie_6000000.txt", "r+"),
            "tab_non_trie_6000000.txt",
            6000000
        },
        {
            fopen("../Data/tab_non_trie_8000000.txt", "r+"),
            "tab_non_trie_8000000.txt",
            8000000
        },
    };

    FILE *f_result = fopen("B1_resultat.csv", "w+");    
    if (f_result == NULL)
    {
        perror("Erreur ouverture fichier resultat\n");
        return EXIT_FAILURE;
    }

    fprintf(f_result, "n,cas,x,comparaisons,temps_ms\n");

    for (int s = 0; s < 10; s++)
    {
        int n = files[s].nb_nombres;
        int *tab = malloc(n * sizeof(int));
        if (tab == NULL)
        {
            perror("Erreur allocation memoire\n");
            return EXIT_FAILURE;
        }

        FILE *f_data = files[s].f;
        
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
