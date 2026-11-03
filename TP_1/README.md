# TP1 — Guide d’utilisation

Ce TP compare quatre algorithmes de primalité sur les 12 nombres du sujet et sur cinq jeux de données. Exécutez les commandes depuis le dossier `TP_1`.

## 1. Préparer les données

Avant de lancer le programme, créez le dossier `data/` à la racine de `TP_1` et placez-y les cinq fichiers suivants :

```text
TP_1/
└── data/
    ├── Random100.txt
    ├── Random1000.txt
    ├── Test-1.txt
    ├── Test-2.txt
    └── Test-3.txt
```

Chaque fichier doit contenir un entier par ligne. Gardez exactement ces noms : le runner les ouvre directement depuis `data/`. Si vos fichiers portent d’autres noms, modifiez la liste `dataset_names` dans `Codes_sources/tp1_runner.c` pour qu’elle corresponde aux noms de fichiers, sans l’extension `.txt`.

Les 12 nombres fixes du sujet sont déjà dans le programme : les fichiers de données ne les remplacent pas. Le runner teste les deux groupes.

## 2. Prérequis

- **Linux :** GCC, Python 3 et pip.
- **Windows :** GCC (par exemple MinGW-w64) ajouté au `PATH`, Python 3 et pip.
- Matplotlib, installé avec `requirements.txt`.

## 3. Compiler, tester et générer les graphiques

### Linux

```sh
gcc -std=c11 -O2 -Wall -Wextra -Wpedantic Codes_sources/tp1_runner.c -lm -o tp1_runner
./tp1_runner
python3 -m pip install -r requirements.txt
python3 scripts/generate_graphs.py
```

### Windows (PowerShell)

```powershell
gcc -std=c11 -O2 -Wall -Wextra -Wpedantic Codes_sources/tp1_runner.c -lm -o tp1_runner.exe
.\tp1_runner.exe
py -m pip install -r requirements.txt
py scripts\generate_graphs.py
```

Si `py` n’est pas disponible, remplacez-le par `python`.

Le programme mesure les 12 nombres fixes avec les quatre algorithmes. Il teste aussi les nombres de chacun des cinq fichiers et écrit un CSV détaillé par jeu de données. Pour éviter des temps d’attente excessifs, A1 n’est pas chronométré au-delà de `N = 100000000` et A2 au-delà de `N = 1000000000`; ces cases sont notées `SKIP`. A3 et A4 sont chronométrés sur toutes les entrées.

## 4. Fichiers produits

- `results/` : CSV des 12 nombres fixes et des cinq jeux de données.
- `report/timing_tables.md` et `report/dataset_tables.md` : tableaux lisibles des résultats.
- `graphs/` : 17 graphiques PNG.
- `report/main.tex` : rapport complet en français. Pour produire le PDF, compilez-le depuis `report/` avec `pdflatex main.tex`.

Les fichiers `Codes_sources/A1.c` à `A4.c` sont conservés tels quels. Le programme de mesure est `Codes_sources/tp1_runner.c`.
