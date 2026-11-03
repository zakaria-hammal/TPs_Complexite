#!/usr/bin/env python3
"""Generate plots for the fixed TP1 values and the five input datasets."""

import csv
from pathlib import Path

import matplotlib.pyplot as plt

ROOT = Path(__file__).resolve().parents[1]
RESULTS = ROOT / "results"
GRAPHS = ROOT / "graphs"
INPUTS = [
    1000003, 2000003, 4000037, 8000009, 16000057, 32000011,
    64000031, 128000003, 256000001, 512000009, 1024000009, 2048000011,
]
DATASETS = ["Random100", "Random1000", "Test-1", "Test-2", "Test-3"]
COLORS = {"A1": "#FF6B6B", "A2": "#4ECDC4", "A3": "#45B7D1", "A4": "#96CEB4"}


def read_csv(path):
    with path.open(newline="", encoding="utf-8") as source:
        return list(csv.DictReader(source))


def validate_rows(rows, path):
    values = [int(row["N"]) for row in rows]
    if values != INPUTS:
        raise ValueError(f"{path}: N values or order do not match the 12 TP1 values")


def save_plot(fig, name):
    GRAPHS.mkdir(parents=True, exist_ok=True)
    destination = GRAPHS / name
    fig.savefig(destination, dpi=180, bbox_inches="tight")
    plt.close(fig)
    print(f"[OK] {destination.relative_to(ROOT)}")


def load_experimental():
    data = {}
    for algorithm in COLORS:
        path = RESULTS / f"{algorithm}.csv"
        rows = read_csv(path)
        validate_rows(rows, path)
        times = [float(row["seconds"]) for row in rows]
        if any(value <= 0 for value in times):
            raise ValueError(f"{path}: timings must all be positive")
        if any(row["is_prime"].lower() != "true" for row in rows):
            raise ValueError(f"{path}: expected every supplied N to be prime")
        data[algorithm] = times
    return data


def plot_individual_experimental(algorithm, times):
    fig, ax = plt.subplots(figsize=(9, 5.5))
    ax.plot(INPUTS, times, marker="o", linewidth=2, color=COLORS[algorithm])
    ax.set_xscale("log", base=2)
    ax.set_xlabel("N (échelle logarithmique, base 2)")
    ax.set_ylabel("Temps moyen par exécution (s)")
    ax.set_title(f"{algorithm} — temps expérimental")
    ax.grid(True, which="both", alpha=0.3)
    save_plot(fig, f"experimental_{algorithm}.png")


def load_theoretical():
    path = RESULTS / "theoretical_work.csv"
    rows = read_csv(path)
    validate_rows(rows, path)
    return {
        algorithm: [float(row[f"{algorithm}_operations"]) for row in rows]
        for algorithm in COLORS
    }


def plot_individual_theoretical(algorithm, operations):
    fig, ax = plt.subplots(figsize=(9, 5.5))
    ax.plot(INPUTS, operations, marker="o", linewidth=2, color=COLORS[algorithm])
    ax.set_xscale("log", base=2)
    ax.set_yscale("log")
    ax.set_xlabel("N (échelle logarithmique, base 2)")
    ax.set_ylabel("Proxy du nombre d'opérations (pas des secondes)")
    ax.set_title(f"{algorithm} — croissance théorique")
    ax.grid(True, which="both", alpha=0.3)
    save_plot(fig, f"theoretical_{algorithm}.png")


def plot_progressive_comparisons(data):
    for included in (("A1", "A2"), ("A1", "A2", "A3"), ("A1", "A2", "A3", "A4")):
        fig, ax = plt.subplots(figsize=(10, 6))
        for algorithm in included:
            ax.plot(INPUTS, data[algorithm], marker="o", linewidth=2,
                    color=COLORS[algorithm], label=algorithm)
        ax.set_xscale("log", base=2)
        ax.set_yscale("log")
        ax.set_xlabel("N (échelle logarithmique, base 2)")
        ax.set_ylabel("Temps moyen par exécution (s, échelle logarithmique)")
        ax.set_title("Comparaison expérimentale : " + " vs ".join(included))
        ax.grid(True, which="both", alpha=0.3)
        ax.legend()
        save_plot(fig, "comparison_" + "_".join(included) + ".png")


def load_dataset_results():
    all_results = {}
    for dataset in DATASETS:
        source = ROOT / "data" / f"{dataset}.txt"
        expected_values = [int(value) for value in source.read_text(encoding="utf-8").split()]
        path = RESULTS / f"dataset_{dataset}.csv"
        rows = read_csv(path)
        actual_values = [int(row["N"]) for row in rows]
        if actual_values != expected_values:
            raise ValueError(f"{path}: rows do not match {source.name}")

        for row in rows:
            if row["is_prime"].lower() not in ("true", "false"):
                raise ValueError(f"{path}: invalid primality result")
            for algorithm in COLORS:
                value = row[f"{algorithm}_seconds"]
                if value != "SKIP" and float(value) <= 0:
                    raise ValueError(f"{path}: timings must be positive or SKIP")

        all_results[dataset] = rows
    return all_results


def make_dataset_summary(dataset_results):
    summary = {}
    output = RESULTS / "dataset_summary.csv"
    with output.open("w", newline="", encoding="utf-8") as destination:
        fields = ["dataset", "value_count"]
        for algorithm in COLORS:
            fields.extend([f"{algorithm}_avg_seconds", f"{algorithm}_tested"])
        writer = csv.DictWriter(destination, fieldnames=fields)
        writer.writeheader()

        for dataset, rows in dataset_results.items():
            record = {"dataset": dataset, "value_count": len(rows)}
            summary[dataset] = {}
            for algorithm in COLORS:
                values = [float(row[f"{algorithm}_seconds"]) for row in rows
                          if row[f"{algorithm}_seconds"] != "SKIP"]
                record[f"{algorithm}_avg_seconds"] = (
                    sum(values) / len(values) if values else "SKIP"
                )
                record[f"{algorithm}_tested"] = len(values)
                summary[dataset][algorithm] = record[f"{algorithm}_avg_seconds"]
            writer.writerow(record)
    return summary


def plot_each_dataset(dataset_results):
    for dataset, rows in dataset_results.items():
        fig, ax = plt.subplots(figsize=(9, 5.5))
        for algorithm, color in COLORS.items():
            points = [(int(row["N"]), float(row[f"{algorithm}_seconds"]))
                      for row in rows if row[f"{algorithm}_seconds"] != "SKIP"]
            points.sort()
            if points:
                x_values, y_values = zip(*points)
                ax.plot(x_values, y_values, marker=".", linewidth=1,
                        color=color, label=algorithm)
        ax.set_xscale("log")
        ax.set_yscale("log")
        ax.set_xlabel("Valeur N (échelle logarithmique)")
        ax.set_ylabel("Temps moyen par test (s, échelle logarithmique)")
        ax.set_title(f"Comparaison des algorithmes — {dataset}")
        ax.grid(True, which="both", alpha=0.3)
        ax.legend()
        save_plot(fig, f"datasets_{dataset}.png")


def plot_dataset_summary(summary):
    fig, ax = plt.subplots(figsize=(10, 6))
    positions = list(range(len(DATASETS)))
    width = 0.18

    for index, algorithm in enumerate(COLORS):
        values = [summary[dataset][algorithm] for dataset in DATASETS]
        values = [float("nan") if value == "SKIP" else value for value in values]
        offsets = [position + (index - 1.5) * width for position in positions]
        ax.bar(offsets, values, width=width, color=COLORS[algorithm], label=algorithm)

    ax.set_yscale("log")
    ax.set_xticks(positions)
    ax.set_xticklabels(DATASETS, rotation=20, ha="right")
    ax.set_ylabel("Temps moyen par entrée mesurée (s, échelle logarithmique)")
    ax.set_title("Temps moyens des algorithmes par jeu de données")
    ax.grid(True, axis="y", which="both", alpha=0.3)
    ax.legend()
    save_plot(fig, "datasets_comparison.png")


def main():
    experimental = load_experimental()
    theoretical = load_theoretical()
    for algorithm in COLORS:
        plot_individual_experimental(algorithm, experimental[algorithm])
        plot_individual_theoretical(algorithm, theoretical[algorithm])
    plot_progressive_comparisons(experimental)

    dataset_results = load_dataset_results()
    dataset_summary = make_dataset_summary(dataset_results)
    plot_each_dataset(dataset_results)
    plot_dataset_summary(dataset_summary)


if __name__ == "__main__":
    main()
