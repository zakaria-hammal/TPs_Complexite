# TP1 — Guide d’utilisation

Ce TP compare quatre algorithmes de test de primalité. Les commandes ci-dessous sont à exécuter depuis le dossier `TP_1`.

## Prérequis

- **Linux :** GCC, Python 3 et pip.
- **Windows :** GCC (par exemple MinGW-w64) ajouté au `PATH`, Python 3 et pip.
- Python utilise Matplotlib, installé avec `requirements.txt`.

## Linux

```sh
gcc -std=c11 -O2 -Wall -Wextra -Wpedantic Codes_sources/tp1_runner.c -lm -o tp1_runner
./tp1_runner
python3 -m pip install -r requirements.txt
python3 scripts/generate_graphs.py
```

## Windows (PowerShell)

```powershell
gcc -std=c11 -O2 -Wall -Wextra -Wpedantic Codes_sources/tp1_runner.c -lm -o tp1_runner.exe
.\tp1_runner.exe
py -m pip install -r requirements.txt
py scripts\generate_graphs.py
```

Si la commande `py` n’est pas disponible, utilisez `python` à sa place.

## Résultats

- `results/` : tableaux CSV des mesures et du travail théorique.
- `graphs/` : 11 graphiques PNG.
- `report/timing_tables.md` : tableaux lisibles des résultats.
- `report/reponses.md` : réponses et analyse en français.
- `report/main.tex` : rapport complet en LaTeX. Pour produire le PDF, compilez-le avec LaTeX depuis le dossier `report` (par exemple `pdflatex main.tex`).

Le programme mesure les 12 nombres du sujet. Les tests rapides sont répétés jusqu’à ce que leur durée totale atteigne au moins 50 ms, puis le temps moyen est enregistré. Les fichiers `Codes_sources/A1.c` à `A4.c` sont conservés tels quels ; le programme utilisé pour les mesures est `Codes_sources/tp1_runner.c`.
