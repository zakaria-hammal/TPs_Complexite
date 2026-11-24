#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NP_FIXE 50
#define DUREE_MIN 0.2   

static int dims_depuis_nom(const char *path, int *n, int *m)
{
    const char *base = path, *p;
    for (p = path; *p; p++){
        if (*p == '/' || *p == '\\') base = p + 1;
    }
    for (p = base; *p; p++){
        if (*p >= '0' && *p <= '9' && (p == base || p[-1] < '0' || p[-1] > '9'))
            if (sscanf(p, "%dx%d", n, m) == 2) return 1;
    }
            return 0;
}

static int *lire_matrice(const char *path, int n, int m)
{
    FILE *f = fopen(path, "r");
    if (!f) return NULL;
    size_t total = (size_t)n * m;
    int *M = malloc(total * sizeof(int));
    if (M){
        for (size_t k = 0; k < total; k++){
            if (fscanf(f, "%d", &M[k]) != 1) { free(M); M = NULL; break; }}
    }
    fclose(f);
    return M;
}

static int correspond(const int *A, int m, const int *B, int np, int mp,
                      int i, int j)
{
    for (int r = 0; r < np; r++) {
        const int *a = &A[(size_t)(i + r) * m + j];
        const int *b = &B[(size_t)r * mp];
        for (int c = 0; c < mp; c++){
            if (a[c] != b[c]) return 0;
        }
    }
    return 1;
}

int sousMat1(const int *A, int n, int m, const int *B, int np, int mp,
             int *pi, int *pj)
{
    for (int i = 0; i <= n - np; i++){
        for (int j = 0; j <= m - mp; j++){
            if (correspond(A, m, B, np, mp, i, j)) {
                *pi = i; *pj = j;
                return 1;
            }
        }
    }
    return 0;
}

static int borne_inf(const int *row, int m, int x)
{
    int lo = 0, hi = m;
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (row[mid] < x) lo = mid + 1; else hi = mid;
    }
    return lo;
}

int sousMat2(const int *A, int n, int m, const int *B, int np, int mp,
             int *pi, int *pj)
{
    int v = B[0];                          
    for (int i = 0; i <= n - np; i++) {
        const int *row = &A[(size_t)i * m];
        for (int j = borne_inf(row, m, v); j <= m - mp && row[j] == v; j++){
            if (correspond(A, m, B, np, mp, i, j)) {
                *pi = i; *pj = j;
                return 1;
            }
        }
    }
    return 0;
}

static void mesurer(int algo, const int *A, int n, int m, const int *B, int np, int mp)
{
    int (*f)(const int *, int, int, const int *, int, int, int *, int *) =
        (algo == 1) ? sousMat1 : sousMat2;
    int trouve = 0, pi = -1, pj = -1;
    long reps = 0;
    clock_t t0 = clock();
    double ecoule;
    do {
        trouve = f(A, n, m, B, np, mp, &pi, &pj);
        reps++;
        ecoule = (double)(clock() - t0) / CLOCKS_PER_SEC;
    } while (ecoule < DUREE_MIN);
    printf("sousMat%d,%d,%d,%d,%d,%d,%.9f\n", algo, n, m, np, mp, trouve,
           ecoule / reps);
    fflush(stdout);
}

static int *extraire_B(const int *A, int n, int m, int np, int mp)
{
    int *B = malloc((size_t)np * mp * sizeof(int));
    if (!B) return NULL;
    for (int r = 0; r < np; r++){
        memcpy(&B[(size_t)r * mp], &A[(size_t)(n - np + r) * m + (m - mp)],
               mp * sizeof(int));
    }
    return B;
}

static void pire_cas(int algo, int n, int m, int np, int mp)
{
    int *A = calloc((size_t)n * m, sizeof(int));
    int *B = calloc((size_t)np * mp, sizeof(int));
    if (!A || !B) { fprintf(stderr, "Memoire insuffisante\n"); free(A); free(B); return; }
    B[(size_t)np * mp - 1] = 1;
    mesurer(algo, A, n, m, B, np, mp);
    free(A); free(B);
}

int main(int argc, char *argv[])
{
    int ok = argc >= 4 && (!strcmp(argv[1], "n") || !strcmp(argv[1], "np") ||
                           !strcmp(argv[1], "pire") || !strcmp(argv[1], "pirenp")) &&
             (!strcmp(argv[2], "1") || !strcmp(argv[2], "2"));
    if (!ok) {
        fprintf(stderr, "Usage : %s n|np 1|2 fichier [fichier ...]\n"
                        "        %s pire 1|2 n [n ...]\n"
                        "        %s pirenp 1|2 n\n", argv[0], argv[0], argv[0]);
        return EXIT_FAILURE;
    }
    int variable_n = (strcmp(argv[1], "n") == 0);
    int algo = atoi(argv[2]);

    printf("algo,n,m,np,mp,trouve,temps_s\n");

    if (!strcmp(argv[1], "pire")) {         
        for (int a = 3; a < argc; a++) {
            int n = atoi(argv[a]);
            if (n >= 2) pire_cas(algo, n, n, n / 2, n / 2);
        }
        return EXIT_SUCCESS;
    }
    if (!strcmp(argv[1], "pirenp")) {          
        int n = atoi(argv[3]);
        for (int k = 1; k <= 9; k++)
            if (n * k / 10 >= 1) pire_cas(algo, n, n, n * k / 10, n * k / 10);
        return EXIT_SUCCESS;
    }

    for (int a = 3; a < argc; a++) {
        int n = 0, m = 0;
        int *A = NULL;
        if (dims_depuis_nom(argv[a], &n, &m)) A = lire_matrice(argv[a], n, m);
        if (!A) { fprintf(stderr, "Erreur avec %s\n", argv[a]); continue; }

        if (variable_n) {
            if (NP_FIXE < n && NP_FIXE < m) {
                int *B = extraire_B(A, n, m, NP_FIXE, NP_FIXE);
                mesurer(algo, A, n, m, B, NP_FIXE, NP_FIXE);
                free(B);
            }
        } else {
            for (int k = 1; k <= 9; k++) {
                int np = n * k / 10, mp = m * k / 10;
                if (np < 1 || mp < 1) continue;
                int *B = extraire_B(A, n, m, np, mp);
                mesurer(algo, A, n, m, B, np, mp);
                free(B);
            }
        }
        free(A);
    }
    return EXIT_SUCCESS;
}