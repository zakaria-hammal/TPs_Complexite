import sys
import pandas as pd
import matplotlib.pyplot as plt

folder = sys.argv[1] if len(sys.argv) > 1 else "."


def load(name):
    return pd.read_csv(f"{folder}/{name}_resultat.csv")


def fmt_x(ax):
    ax.xaxis.set_major_formatter(
        plt.FuncFormatter(lambda v, _: f"{v / 1e6:g}M" if v >= 1e6 else f"{v / 1e3:g}k"))
    ax.set_xlabel("n")
    ax.grid(alpha=0.3)


def split(df):
    return df[df.cas == "Meilleur cas"], df[df.cas == "Pire cas"]


# A2 vs A3 (temps, meilleur et pire cas)
b2, w2 = split(load("A2"))
b3, w3 = split(load("A3"))
fig, ax = plt.subplots(figsize=(6.5, 4.2))
ax.plot(b2.n, b2.temps_ms, "o-", color="tab:blue", label="A2 meilleur cas")
ax.plot(w2.n, w2.temps_ms, "s--", color="tab:blue", label="A2 pire cas")
ax.plot(b3.n, b3.temps_ms, "o-", color="tab:orange", label="A3 meilleur cas")
ax.plot(w3.n, w3.temps_ms, "s--", color="tab:orange", label="A3 pire cas")
ax.set_ylabel("temps (ms)")
ax.set_title("A2 vs A3 - temps")
ax.legend()
fmt_x(ax)
fig.tight_layout()
fig.savefig("A2_vs_A3.png")
plt.close()

# B1 vs B2 (temps)
b1 = load("B1")
b2 = load("B2")
fig, ax = plt.subplots(figsize=(6.5, 4.2))
ax.plot(b1.n, b1.temps_ms, "o-", label="B1")
ax.plot(b2.n, b2.temps_ms, "s--", label="B2")
ax.set_ylabel("temps (ms)")
ax.set_title("B1 vs B2 - temps")
ax.legend()
fmt_x(ax)
fig.tight_layout()
fig.savefig("B1_vs_B2.png")
plt.close()
