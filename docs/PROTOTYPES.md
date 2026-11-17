# DN1-LAB 0.3.0-alpha — six DSP prototypes additionnels

Cette couche ajoute six expériences DSP hôte indépendantes des trois machines SDK alpha déjà portées. Les sources sont volontairement dans `prototypes/`, pas dans `sdk-alpha/` : elles ne sont donc pas prises par le script de construction du firmware. Les identifiants 103–108 sont **provisoires** et n'ont pas été soumis au linker combiné ni au registre complet.

## Vue d'ensemble

| Moteur | État hôte | Principe | Contrôle distinctif | Préécoute |
|---|---:|---|---|---|
| VESPER | 60/256 o | Mémoire d'excitation de 32 cellules + deux résonateurs | TRACE / GRAIN / REV / FREEZ | [WAV](../audio-prototypes/VESPER-host-preview.wav) |
| MYCEL | 40/256 o | Quatre nœuds résonants, six liens symétriques | TOPO / LINK / MASS | [WAV](../audio-prototypes/MYCEL-host-preview.wav) |
| ORBITAL | 44/256 o | Champ harmonique piloté par un chemin 2D avec hystérésis | X / Y / PATH / TURN | [WAV](../audio-prototypes/ORBITAL-host-preview.wav) |
| LITHOS | 44/256 o | Six partiels et trois réservoirs énergétiques de matière | ERODE / DEPOS / FRACT | [WAV](../audio-prototypes/LITHOS-host-preview.wav) |
| PHOTON | 40/256 o | Impulsions clairsemées dans un réseau de phase à quatre nœuds | PULSE / REFLECT / COUPLE | [WAV](../audio-prototypes/PHOTON-host-preview.wav) |
| ANIMA | 56/256 o | Écologie de quatre agents **à l'intérieur d'une voix** | POPUL / COHES / MUTUAL | [WAV](../audio-prototypes/ANIMA-host-preview.wav) |

Les nombres d'état sont obtenus par `sizeof(struct ..._state)` dans les six sources hôte et restent largement sous le plafond SDK théorique de 256 octets. Cela ne prouve pas le coût CPU ni la compatibilité de l'adaptateur sur ColdFire.

## Modèles DSP

### VESPER — Resonant Time Loom
Un tampon circulaire de 32 échantillons signés de faible résolution conserve une empreinte de l'excitation. Une tête de lecture à vitesse variable, inversible et traversée par un léger mouvement aléatoire relit cette empreinte. Deux résonateurs du second ordre en extraient deux composantes modales. `FREEZ` arrête l'écriture, `REV` inverse le sens de lecture, `DRIFT` perturbe la vitesse et `FRAY` quantifie la matière relue. Le tampon ne contient pas un sample de note : il conserve les résidus de l'excitation courante.

### MYCEL — Distributed Coupling Synth
Quatre oscillateurs résonants portent chacun un niveau d'énergie borné. Six arêtes fixes transfèrent symétriquement une fraction de la différence d'énergie entre deux nœuds. `TOPO` fait évoluer les poids entre une chaîne, un anneau et un graphe plus dense ; `LINK` borne le coefficient de couplage. L'amortissement est strictement positif dans les cas de test sans excitation/bruit, ce qui rend l'énergie totale non croissante à l'arrondi près. Ce n'est pas un solveur mécanique complet.

### ORBITAL — Gesture-Conditioned Harmonic Field
Deux coordonnées internes filtrées évoluent autour d'un point X/Y, avec une trajectoire orbitale dont la vitesse et le sens sont pilotés par PATH/TURN. Les amplitudes d'une banque fixe de six partiels sont recalculées à partir de la position et du vecteur de chemin mémorisé. `LOCK` rapproche l'état de la cible, `SLEW` règle l'inertie et `VIB` module les incréments de phase. Le test de chemin vérifie qu'un même point final atteint depuis deux trajectoires différentes ne produit pas le même bloc audio.

### LITHOS — Spectral Erosion Engine
Six partiels sont répartis en trois groupes qui puisent dans trois réservoirs d'énergie : matière lisse, granulaire et fracturée. Les transferts ERODE, DEPOSIT, FRACT, POROUS et REPAIR sont calculés à partir de l'ancien état, puis appliqués simultanément ; les transferts internes s'annulent dans la somme énergétique à l'arrondi près. Les pertes DECAY/WEAR sont explicites, la frappe HIT injecte de l'énergie et DUST peut en réinjecter une petite quantité dans la classe fracturée. Aucun FFT n'est employé.

### PHOTON — Event-Driven Phase Camera
Un compteur d'événement borné génère des impulsions clairsemées à une cadence contrôlée par DENS. Elles alimentent un nœud de phase et circulent ensuite dans un graphe à quatre nœuds. COUPLE effectue un transfert symétrique d'énergie ; FEEDBACK transfère une partie de l'énergie du premier au dernier nœud, sans création nette d'énergie ; DECAY et SOFT dissipent le système. Aucune file d'événements ni allocation dynamique n'est requise.

### ANIMA — Local Ecology Fallback
Quatre agents virtuels disposent chacun d'une activité et d'une mémoire lente. Ils échangent une fraction bornée de leur activité avec leurs deux voisins ; la cohésion agit également sur leurs phases. Le contrôle GLOBAL ne désigne pas un bus inter-voix : dans ce repli, il règle la contribution collective des quatre agents à la sortie de cette voix seulement. La proposition inter-voix reste bloquée jusqu'à ce qu'une API SDK officielle garantisse la durée de vie, la synchronisation, l'ordre des callbacks et les règles de vol de voix. Aucun état global statique n'est utilisé.

## Contrôles et interface

Chaque source expose quinze paramètres, deux pages de huit encodeurs maximum, noms de machine et noms courts conformes au sous-ensemble hôte. Les labels de contrôle sont limités à cinq caractères. Les mappages complets et valeurs par défaut sont dans le `prototype.json` du moteur correspondant.

## Exécuter les tests et générer les préécoutes

```sh
CC=gcc ./tests/run_prototype_tests.sh
CC=clang ./tests/run_prototype_tests.sh
./tests/run_prototype_demos.sh
```

Les WAV sont mono, 48 kHz, 16 bits ; quatre notes MIDI 36/48/60/72 sont rendues avec les paramètres par défaut. Le fondu final est appliqué par le générateur de démo hôte, pas par les moteurs ; il ne simule pas exactement l'enveloppe AMP native. Les préécoutes sont des rendus sans le filtre, chorus, delay, reverb ni mixage natif du Digitone.

## État de validation et limites

Le harnais couvre pour chaque moteur : balayage MIDI 0–127 et min/défaut/max de chaque paramètre, rendu de patch par défaut, coins all-min/all-max sur 512 blocs, huit états de voix indépendants pendant 384 blocs randomisés, comparaison bit à bit de l'ordre de rendu 0→7 et 7→0 sur 128 blocs, redéclenchement déterministe, bornes d'état internes et compteur de clamps instrumenté. ORBITAL reçoit un test spécifique d'hystérésis de trajectoire.

GCC 14.2 et Clang 17 réussissent les tests hôte ; ASan/UBSan et les analyseurs GCC `-fanalyzer` / Clang `--analyze` passent après correction d'un dépassement entier détecté dans le vibrato ORBITAL. Aucun clamp Q15 n'est détecté dans cette suite. Les huit lanes sont huit états indépendants rendus en séquence, **pas** un mixage de huit voix simultanées ni une mesure du DSP matériel.

Aucun des six moteurs n'a encore été compilé avec le vrai header/sdk-builder upstream sur ColdFire, lié avec `core-dn1` et `machines-dn1`, passé dans DigiKit/digiemu, profilé en cycles ou essayé sur un Digitone. Les IDs ne sont pas validés par le linker combiné. Les six prototypes ne sont donc ni des `.elemod` installables ni une image `.syx` prête à flasher.