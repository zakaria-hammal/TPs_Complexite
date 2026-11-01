#!/usr/bin/env python3
"""Generate TP1 experimental, theoretical, and progressive comparison plots."""

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


def main():
    experimental = load_experimental()
    theoretical = load_theoretical()
    for algorithm in COLORS:
        plot_individual_experimental(algorithm, experimental[algorithm])
        plot_individual_theoretical(algorithm, theoretical[algorithm])
    plot_progressive_comparisons(experimental)


if __name__ == "__main__":
    main()
