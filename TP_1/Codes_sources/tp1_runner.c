#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#define INPUT_COUNT 12
#define ALGORITHM_COUNT 4
#define DATASET_COUNT 5
#define MAX_DATASET_VALUES 1200
#define MIN_SECONDS 0.05
#define DATASET_MIN_SECONDS 0.001
#define A1_DATASET_LIMIT 100000000LL
#define A2_DATASET_LIMIT 1000000000LL

static volatile int benchmark_sink;
static volatile long long benchmark_input;

static const long long inputs[INPUT_COUNT] = {
    1000003, 2000003, 4000037, 8000009,
    16000057, 32000011, 64000031, 128000003,
    256000001, 512000009, 1024000009, 2048000011
};

static const char *algorithm_names[ALGORITHM_COUNT] = {
    "A1", "A2", "A3", "A4"
};

static const char *dataset_names[DATASET_COUNT] = {
    "Random100", "Random1000", "Test-1", "Test-2", "Test-3"
};

static bool is_prime_a1(long long n)
{
    if (n < 2) return false;

    for (long long divisor = 2; divisor < n; divisor++) {
        if (n % divisor == 0) return false;
    }
    return true;
}

static bool is_prime_a2(long long n)
{
    if (n < 2) return false;

    for (long long divisor = 2; divisor <= n / 2; divisor++) {
        if (n % divisor == 0) return false;
    }
    return true;
}

static bool is_prime_a3(long long n)
{
    if (n < 2) return false;

    for (long long divisor = 2; divisor <= n / divisor; divisor++) {
        if (n % divisor == 0) return false;
    }
    return true;
}

static bool is_prime_a4(long long n)
{
    if (n < 2) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;

    for (long long divisor = 3; divisor <= n / divisor; divisor += 2) {
        if (n % divisor == 0) return false;
    }
    return true;
}

/* Choisit l'algorithme demandé sans utiliser de pointeur de fonction. */
static bool run_algorithm(int algorithm, long long n)
{
    switch (algorithm) {
        case 0: return is_prime_a1(n);
        case 1: return is_prime_a2(n);
        case 2: return is_prime_a3(n);
        case 3: return is_prime_a4(n);
        default: return false;
    }
}

static double time_batch(int algorithm, long long n, int repetitions)
{
    benchmark_input = n;
    clock_t start = clock();
    if (start == (clock_t)-1) return -1.0;

    for (int i = 0; i < repetitions; i++) {
        benchmark_sink = run_algorithm(algorithm, benchmark_input);
    }

    clock_t end = clock();
    if (end == (clock_t)-1 || end < start) return -1.0;

    return (double)(end - start) / CLOCKS_PER_SEC;
}

static bool measure_algorithm(int algorithm, long long n, double minimum_seconds,
                              double *average_seconds, int *repetitions_used)
{
    int repetitions = 1;
    double elapsed = time_batch(algorithm, n, repetitions);

    if (elapsed < 0.0) return false;

    while (elapsed < minimum_seconds) {
        if (repetitions > 500000000) return false;
        repetitions *= 2;
        elapsed = time_batch(algorithm, n, repetitions);
        if (elapsed < 0.0) return false;
    }

    *average_seconds = elapsed / repetitions;
    *repetitions_used = repetitions;
    return *average_seconds > 0.0;
}

static bool check_edge_cases(void)
{
    const long long values[] = {-7, 0, 1, 2, 3, 4, 9, 25, 49, 97};
    const bool expected[] = {
        false, false, false, true, true, false, false, false, false, true
    };

    for (int i = 0; i < 10; i++) {
        for (int algorithm = 0; algorithm < ALGORITHM_COUNT; algorithm++) {
            if (run_algorithm(algorithm, values[i]) != expected[i]) {
                printf("Erreur sur le cas N=%lld avec %s\n",
                       values[i], algorithm_names[algorithm]);
                return false;
            }
        }
    }
    return true;
}

static bool save_results(double times[INPUT_COUNT][ALGORITHM_COUNT],
                         bool prime[INPUT_COUNT])
{
    FILE *all_results = fopen("results/timings.csv", "w");
    FILE *algorithm_files[ALGORITHM_COUNT];
    FILE *theory = fopen("results/theoretical_work.csv", "w");
    FILE *tables = fopen("report/timing_tables.md", "w");

    if (all_results == NULL || theory == NULL || tables == NULL) {
        printf("Impossible de créer un fichier de résultats.\n");
        return false;
    }

    for (int algorithm = 0; algorithm < ALGORITHM_COUNT; algorithm++) {
        char filename[40];
        snprintf(filename, sizeof(filename), "results/%s.csv",
                 algorithm_names[algorithm]);
        algorithm_files[algorithm] = fopen(filename, "w");
        if (algorithm_files[algorithm] == NULL) {
            printf("Impossible de créer %s.\n", filename);
            return false;
        }
    }

    fprintf(all_results, "N,A1_seconds,A2_seconds,A3_seconds,A4_seconds,is_prime\n");
    fprintf(theory, "N,A1_operations,A2_operations,A3_operations,A4_operations\n");
    fprintf(tables, "# Tables des résultats du TP1\n\n");
    fprintf(tables, "Temps en secondes, moyennés sur un lot d'au moins 50 ms.\n\n");
    fprintf(tables, "| N | A1 (s) | A2 (s) | A3 (s) | A4 (s) | Premier |\n");
    fprintf(tables, "|---:|---:|---:|---:|---:|:---:|\n");

    for (int algorithm = 0; algorithm < ALGORITHM_COUNT; algorithm++) {
        fprintf(algorithm_files[algorithm], "N,seconds,is_prime\n");
    }

    for (int i = 0; i < INPUT_COUNT; i++) {
        long long n = inputs[i];

        fprintf(all_results, "%lld,%.12g,%.12g,%.12g,%.12g,%s\n",
                n, times[i][0], times[i][1], times[i][2], times[i][3],
                prime[i] ? "true" : "false");

        for (int algorithm = 0; algorithm < ALGORITHM_COUNT; algorithm++) {
            fprintf(algorithm_files[algorithm], "%lld,%.12g,%s\n",
                    n, times[i][algorithm], prime[i] ? "true" : "false");
        }

        double root = sqrt((double)n);
        fprintf(theory, "%lld,%.0f,%.1f,%.6f,%.6f\n",
                n, (double)n - 2.0, (double)n / 2.0 - 1.0,
                root, root / 2.0);
        fprintf(tables, "| %lld | %.9g | %.9g | %.9g | %.9g | %s |\n",
                n, times[i][0], times[i][1], times[i][2], times[i][3],
                prime[i] ? "Oui" : "Non");
    }

    fprintf(tables, "\n## Proxys de travail théorique\n\n");
    fprintf(tables, "Ces valeurs sont des nombres d'opérations approximatifs, pas des secondes.\n\n");
    fprintf(tables, "| N | A1 : N−2 | A2 : N/2−1 | A3 : √N | A4 : √N/2 |\n");
    fprintf(tables, "|---:|---:|---:|---:|---:|\n");

    for (int i = 0; i < INPUT_COUNT; i++) {
        double root = sqrt((double)inputs[i]);
        fprintf(tables, "| %lld | %.0f | %.1f | %.3f | %.3f |\n",
                inputs[i], (double)inputs[i] - 2.0,
                (double)inputs[i] / 2.0 - 1.0, root, root / 2.0);
    }

    fclose(all_results);
    fclose(theory);
    fclose(tables);
    for (int algorithm = 0; algorithm < ALGORITHM_COUNT; algorithm++) {
        fclose(algorithm_files[algorithm]);
    }
    return true;
}

static bool can_measure_dataset_value(int algorithm, long long n)
{
    if (algorithm == 0 && n > A1_DATASET_LIMIT) return false;
    if (algorithm == 1 && n > A2_DATASET_LIMIT) return false;
    return true;
}

static bool process_datasets(void)
{
    FILE *summary = fopen("results/dataset_summary.csv", "w");
    FILE *tables = fopen("report/dataset_tables.md", "w");

    if (summary == NULL || tables == NULL) {
        printf("Impossible de créer les résultats des jeux de données.\n");
        return false;
    }

    fprintf(summary, "dataset,value_count");
    fprintf(tables, "# Résultats des cinq jeux de données\n\n");
    fprintf(tables, "Les temps sont des moyennes par entrée, mesurées avec des lots d'au moins 1 ms. A1 est limité à N <= %lld et A2 à N <= %lld pour éviter des durées excessives.\n\n",
            A1_DATASET_LIMIT, A2_DATASET_LIMIT);
    fprintf(tables, "| Jeu de données | Entrées | A1 (s; testées) | A2 (s; testées) | A3 (s; testées) | A4 (s; testées) |\n");
    fprintf(tables, "|---|---:|---:|---:|---:|---:|\n");

    for (int algorithm = 0; algorithm < ALGORITHM_COUNT; algorithm++) {
        fprintf(summary, ",%s_avg_seconds,%s_tested", algorithm_names[algorithm], algorithm_names[algorithm]);
    }
    fprintf(summary, "\n");

    for (int d = 0; d < DATASET_COUNT; d++) {
        char input_name[80];
        char output_name[100];
        snprintf(input_name, sizeof(input_name), "data/%s.txt", dataset_names[d]);
        snprintf(output_name, sizeof(output_name), "results/dataset_%s.csv", dataset_names[d]);

        FILE *input_file = fopen(input_name, "r");
        FILE *output_file = fopen(output_name, "w");
        if (input_file == NULL || output_file == NULL) {
            printf("Impossible d'ouvrir %s ou de créer %s.\n", input_name, output_name);
            if (input_file != NULL) fclose(input_file);
            if (output_file != NULL) fclose(output_file);
            fclose(summary);
            fclose(tables);
            return false;
        }

        fprintf(output_file, "N,A1_seconds,A2_seconds,A3_seconds,A4_seconds,is_prime\n");

        long long values[MAX_DATASET_VALUES];
        int value_count = 0;
        long long n;
        while (fscanf(input_file, "%lld", &n) == 1) {
            if (value_count == MAX_DATASET_VALUES) {
                printf("Le jeu %s contient trop de valeurs.\n", dataset_names[d]);
                fclose(input_file);
                fclose(output_file);
                fclose(summary);
                fclose(tables);
                return false;
            }
            values[value_count++] = n;
        }
        fclose(input_file);

        if (value_count == 0) {
            printf("Le jeu %s est vide ou illisible.\n", dataset_names[d]);
            fclose(output_file);
            fclose(summary);
            fclose(tables);
            return false;
        }

        int tested[ALGORITHM_COUNT] = {0, 0, 0, 0};
        double totals[ALGORITHM_COUNT] = {0, 0, 0, 0};

        for (int i = 0; i < value_count; i++) {
            bool answer = run_algorithm(3, values[i]);
            char time_text[ALGORITHM_COUNT][40];

            for (int algorithm = 0; algorithm < ALGORITHM_COUNT; algorithm++) {
                if (!can_measure_dataset_value(algorithm, values[i])) {
                    snprintf(time_text[algorithm], sizeof(time_text[algorithm]), "SKIP");
                    continue;
                }

                if (run_algorithm(algorithm, values[i]) != answer) {
                    printf("Désaccord entre algorithmes dans %s, N=%lld.\n",
                           dataset_names[d], values[i]);
                    fclose(output_file);
                    fclose(summary);
                    fclose(tables);
                    return false;
                }

                double seconds;
                int repetitions;
                if (!measure_algorithm(algorithm, values[i], DATASET_MIN_SECONDS,
                                       &seconds, &repetitions)) {
                    printf("Échec de mesure dans %s, N=%lld.\n",
                           dataset_names[d], values[i]);
                    fclose(output_file);
                    fclose(summary);
                    fclose(tables);
                    return false;
                }
                (void)repetitions;
                totals[algorithm] += seconds;
                tested[algorithm]++;
                snprintf(time_text[algorithm], sizeof(time_text[algorithm]), "%.12g", seconds);
            }

            fprintf(output_file, "%lld", values[i]);
            for (int algorithm = 0; algorithm < ALGORITHM_COUNT; algorithm++) {
                fprintf(output_file, ",%s", time_text[algorithm]);
            }
            fprintf(output_file, ",%s\n", answer ? "true" : "false");
        }

        fclose(output_file);
        fprintf(summary, "%s,%d", dataset_names[d], value_count);
        fprintf(tables, "| %s | %d", dataset_names[d], value_count);
        for (int algorithm = 0; algorithm < ALGORITHM_COUNT; algorithm++) {
            if (tested[algorithm] == 0) {
                fprintf(summary, ",SKIP,0");
                fprintf(tables, " | SKIP (0/%d)", value_count);
            } else {
                double average = totals[algorithm] / tested[algorithm];
                fprintf(summary, ",%.12g,%d", average, tested[algorithm]);
                fprintf(tables, " | %.6g (%d/%d)", average,
                        tested[algorithm], value_count);
            }
        }
        fprintf(summary, "\n");
        fprintf(tables, " |\n");
        printf("Jeu %s : %d valeurs traitées.\n", dataset_names[d], value_count);
    }

    fclose(summary);
    fclose(tables);
    return true;
}

int main(void)
{
    double times[INPUT_COUNT][ALGORITHM_COUNT];
    bool prime[INPUT_COUNT];

    if (!check_edge_cases()) return 1;

    for (int i = 0; i < INPUT_COUNT; i++) {
        prime[i] = is_prime_a4(inputs[i]);
        if (!prime[i]) {
            printf("N=%lld n'est pas premier.\n", inputs[i]);
            return 1;
        }

        printf("N=%lld est premier;", inputs[i]);
        for (int algorithm = 0; algorithm < ALGORITHM_COUNT; algorithm++) {
            int repetitions;
            if (!measure_algorithm(algorithm, inputs[i], MIN_SECONDS,
                                   &times[i][algorithm], &repetitions)) {
                printf("\nErreur de mesure pour %s.\n",
                       algorithm_names[algorithm]);
                return 1;
            }
            printf(" %s=%.9g s (%d répétitions);",
                   algorithm_names[algorithm], times[i][algorithm], repetitions);
        }
        printf("\n");
    }

    if (!save_results(times, prime)) return 1;
    if (!process_datasets()) return 1;
    printf("CSV et tableaux générés.\n");
    return 0;
}
