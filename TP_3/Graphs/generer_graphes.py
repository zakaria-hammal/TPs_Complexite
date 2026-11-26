import os
import pandas as pd
import matplotlib.pyplot as plt

os.makedirs("Graphs", exist_ok=True)


def graphe_exercice1():
    df = pd.read_csv("Csv files/ex1.csv").sort_values(by="n")
    n = df["n"].values
    temps = df["temps_s"].values

    theorique = (temps[-1] / (n[-1] ** 3)) * (n ** 3)

    plt.figure(figsize=(9, 6))
    plt.plot(n, temps, marker="o", color="blue", label="Mesures experimentales")
    plt.plot(n, theorique, linestyle="--", color="red", label="Theorique O(n^3)")

    plt.title("Exercice 1 : Temps d'execution du produit de deux matrices")
    plt.xlabel("Taille de la matrice (n)")
    plt.ylabel("Temps d'execution (secondes)")
    plt.grid(True)
    plt.legend()
    plt.savefig("Graphiques/ex1_produit_matrices.png", dpi=300)
    plt.close()


def graphe_exercice2_variation_n():
    df1 = pd.read_csv("Csv files/ex2_s1_n.csv").sort_values(by="n")
    df2 = pd.read_csv("Csv files/ex2_s2_n.csv").sort_values(by="n")

    plt.figure(figsize=(9, 6))
    plt.plot(df1["n"], df1["temps_s"], marker="o", color="red", label="sousMat1 (non triee)")
    plt.plot(df2["n"], df2["temps_s"], marker="s", color="green", label="sousMat2 (lignes triees)")

    plt.title("Exercice 2 : Recherche de sous-matrice en faisant varier n (np fixe a 50)")
    plt.xlabel("Taille de la matrice principale (n)")
    plt.ylabel("Temps d'execution (secondes)")
    plt.grid(True)
    plt.legend()
    plt.savefig("Graphiques/ex2_variation_n.png", dpi=300)
    plt.close()


def graphe_exercice2_variation_np():
    df1 = pd.read_csv("Csv files/ex2_s1_np.csv").sort_values(by="np")
    df2 = pd.read_csv("Csv files/ex2_s2_np.csv").sort_values(by="np")

    plt.figure(figsize=(9, 6))
    plt.plot(df1["np"], df1["temps_s"], marker="o", color="red", label="sousMat1 (n=1851)")
    plt.plot(df2["np"], df2["temps_s"], marker="s", color="green", label="sousMat2 (n=1902)")

    plt.title("Exercice 2 : Recherche de sous-matrice en faisant varier np")
    plt.xlabel("Taille de la sous-matrice recherchee (np)")
    plt.ylabel("Temps d'execution (secondes)")
    plt.grid(True)
    plt.legend()
    plt.savefig("Graphiques/ex2_variation_np.png", dpi=300)
    plt.close()


if __name__ == "__main__":
    graphe_exercice1()
    graphe_exercice2_variation_n()
    graphe_exercice2_variation_np()
