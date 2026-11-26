#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N_MAX 1800000

int main(void)
{
    FILE *f = fopen("Data/tab_trie.txt", "w");
    if (f == NULL)
    {
        f = fopen("tab_trie.txt", "w");
    }
    if (f == NULL)
    {
        perror("Erreur creation du fichier tab_trie.txt\n");
        return EXIT_FAILURE;
    }

    srand(time(NULL));

    int val = (rand() % 50) + 1;
    fprintf(f, "%d\n", val);

    for (int i = 1; i < N_MAX; i++)
    {
        int increment = (rand() % 10) + 1;
        val = val + increment;
        fprintf(f, "%d\n", val);
    }

    fclose(f);
    printf("Fichier tab_trie.txt genere avec succes (%d elements).\n", N_MAX);

    return EXIT_SUCCESS;
}
