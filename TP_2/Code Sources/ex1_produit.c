#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static int dims_depuis_nom(const char *path, int *n, int *m)
{
    const char *base = path, *p;
    for (p = path; *p; p++)
        if (*p == '/' || *p == '\\') base = p + 1;
    for (p = base; *p; p++)
        if (*p >= '0' && *p <= '9' && (p == base || p[-1] < '0' || p[-1] > '9'))
            if (sscanf(p, "%dx%d", n, m) == 2) return 1;
    return 0;
}

// lit une matrice n x n stockee ligne par ligne dans un tableau 1D 
static int *lire_matrice(const char *path, int n)
{
    FILE *f = fopen(path, "r");
    if (!f) return NULL;
    size_t total = (size_t)n * n;
    int *M = malloc(total * sizeof(int));
    if (M)
        for (size_t k = 0; k < total; k++)
            if (fscanf(f, "%d", &M[k]) != 1) { free(M); M = NULL; break; }
    fclose(f);
    return M;
}

static void produit(const int *A, const int *B, unsigned long long *C,
                    int n, int m, int p)
{
    memset(C, 0, (size_t)n * p * sizeof(*C));
    for (int i = 0; i < n; i++)
        for (int k = 0; k < m; k++) {
            unsigned long long a = (unsigned long long)A[(size_t)i * m + k];
            const int *Bk = &B[(size_t)k * p];
            unsigned long long *Ci = &C[(size_t)i * p];
            for (int j = 0; j < p; j++)
                Ci[j] += a * (unsigned long long)Bk[j];
        }
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage : %s fichier1 [fichier2 ...]\n", argv[0]);
        return EXIT_FAILURE;
    }
    printf("n,temps_s,checksum\n");

    for (int a = 1; a < argc; a++) {
        int n = 0, m = 0;
        if (!dims_depuis_nom(argv[a], &n, &m) || n != m) {
            fprintf(stderr, "Ignore %s (matrice carree NxN attendue)\n", argv[a]);
            continue;
        }
        int *A = lire_matrice(argv[a], n);
        unsigned long long *C = A ? malloc((size_t)n * n * sizeof(*C)) : NULL;
        if (!A || !C) {
            fprintf(stderr, "Erreur avec %s\n", argv[a]);
            free(A); free(C);
            continue;
        }

        clock_t t0 = clock();
        produit(A, A, C, n, n, n);          // B = A
        double t = (double)(clock() - t0) / CLOCKS_PER_SEC;

        // checksum : empeche le compilateur de supprimer le calcul
        unsigned long long s = 0;
        for (size_t k = 0; k < (size_t)n * n; k++) s += C[k];

        printf("%d,%.6f,%llu\n", n, t, s);
        fflush(stdout);
        free(A); free(C);
    }
    return EXIT_SUCCESS;
}