#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N_MAX 8000000

int main(void)
{
    FILE *f = fopen("Data/tab_non_trie.txt", "w");
    if (f == NULL)
    {
        f = fopen("tab_non_trie.txt", "w");
    }
    if (f == NULL)
    {
        perror("Erreur creation du fichier tab_non_trie.txt\n");
        return EXIT_FAILURE;
    }

    srand(time(NULL));

    for (int i = 0; i < N_MAX; i++)
    {
        int val = (((rand() & 0x7FFF) << 15) | (rand() & 0x7FFF));
        fprintf(f, "%d\n", val);
    }

    fclose(f);
    printf("Fichier tab_non_trie.txt genere avec succes (%d elements).\n", N_MAX);

    return EXIT_SUCCESS;
}
