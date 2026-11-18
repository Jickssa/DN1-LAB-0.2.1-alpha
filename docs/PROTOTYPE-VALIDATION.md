# Validation DN1-LAB 0.3.0-alpha — prototypes (L2, hôte uniquement)

## Résultats mesurés sur hôte

| Moteur | État | Pic du patch par défaut* | Pic sur balayage MIDI/paramètres | Pic sur stress 8 lanes randomisées | Clamps détectés |
|---|---:|---:|---:|---:|---:|
| VESPER | 60 o | 27.9 % FS | 33.6 % FS | 63.7 % FS | 0 |
| MYCEL | 40 o | 14.2 % FS | 26.3 % FS | 43.1 % FS | 0 |
| ORBITAL | 44 o | 19.4 % FS | 41.9 % FS | 74.8 % FS | 0 |
| LITHOS | 44 o | 10.1 % FS | 64.4 % FS | 85.9 % FS | 0 |
| PHOTON | 40 o | 8.1 % FS | 31.7 % FS | 52.6 % FS | 0 |
| ANIMA | 56 o | 8.3 % FS | 26.4 % FS | 72.3 % FS | 0 |

*Pic mesuré sur 128 blocs (4096 échantillons, soit environ 85,3 ms) à note MIDI 60, paramètres par défaut et état fraîchement initialisé. Les pourcentages sont `peak_Q31 / 2147483647`. Ils sont des mesures du moteur isolé en hôte, pas une normalisation perceptive ni une mesure de gain après chaîne native Digitone.

Les pics huit lanes sont des maxima de blocs rendus avec huit structures d'état séparées, contrôles randomisés et appels séquentiels. Ce n'est pas une mesure de somme polyphonique ni une simulation de huit DSP simultanés.

## Tests exécutés

- GCC 14.2 : `-std=c99 -O2 -Wall -Wextra -Werror -pedantic` — réussi.
- Clang 17 : mêmes options — réussi.
- GCC ASan + UBSan (`-O1 -g`) — réussi, `UBSAN_OPTIONS=halt_on_error=1`.
- Clang ASan + UBSan (`-O1 -g`) — réussi, `UBSAN_OPTIONS=halt_on_error=1`.
- GCC `-fanalyzer` sur les six sources — aucune alerte.
- Clang `--analyze` sur les six sources — aucune alerte.
- Aucun clamp Q15 mesuré dans les suites instrumentées.
- `tests/run_prototype_reproducibility.sh` relance les démos sous GCC et Clang et compare les six WAV octet par octet : les six comparaisons passent.

## Défauts détectés et corrigés pendant le prototypage

1. VESPER : une division trop forte quantifiait l'empreinte initiale vers zéro ; le niveau d'écriture a été redimensionné pour conserver une granularité exploitable dans 8 bits.
2. Interpolation VESPER : les poids fractionnaires sont explicitement convertis en entiers signés avant multiplication, évitant une promotion non signée des cellules négatives.
3. ORBITAL : la multiplication du terme de vibrato par la phase Q15 débordait `int32_t` dans certains cas. L'ordre des divisions est réorganisé pour maintenir les intermédiaires sous la borne 32 bits ; les sanitizers passent.
4. PHOTON : le contrôle FEEDBACK est désormais un transfert d'énergie borné du nœud 0 au nœud 3, et non un terme positif ajouté en continu.
5. ANIMA : les différences de phase modulaires passent par un helper défini, puis sont réduites avant multiplication ; les états restent locaux à chaque voix.

## Non exécuté / blocage

- Pas de compilation M68K/ColdFire ; pas de `.elemod`.
- Pas de lint/linker combiné avec les dépendances `core-dn1` et `machines-dn1`.
- Les IDs 103–108 sont provisoires ; le linker intégrateur doit trancher.
- Pas de test d'émulateur, d'estimation cycle/block au DSP cible, ni de test matériel.
- Pas de validation du mix polyphonique, du chemin enveloppe/filtre/effets ni du comportement de vol de voix sur Digitone.

Les résultats positifs de l'hôte ne doivent pas être interprétés comme une certification temps réel ou firmware.

## Niveau de preuve

Les six prototypes sont **L2 — tests hôte automatisés**. Ce niveau signifie que les sources ont passé les suites hôte publiées et CI ; il ne garantit pas le layout ABI du SDK réel, la compilation ColdFire, le coût temps réel, l'émulation ou le matériel. Les IDs 103–108 restent candidats.
