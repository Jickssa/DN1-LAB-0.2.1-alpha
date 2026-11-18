# DN1-LAB 0.3.0-alpha — guide des instruments

Ce guide décrit les trois moteurs portés vers le shared machine SDK alpha. Les valeurs indiquées sont les plages logiques exposées par les manifestes C, et non les mots bruts SYN 8.8 du firmware. Les pages personnalisées sont présentées dans la zone SYN2 de la piste Digitone.

**Important :** ces sources ont été testées sur hôte. Elles n'ont pas encore été validées par compilation ColdFire, linkage combiné, émulateur ou Digitone physique. Ce guide n'est pas une procédure d'installation.

## KARST — Modal Matter (ID 100)

| Page | Encodeur | Contrôle | Plage | Fonction |
|---|---:|---|---:|---|
| MATTER | A | MTRL | 0–7 | Bois, métal, verre, membrane, cloche, pierre, tube, cristal |
| MATTER | B | STIF | 0–127 | Déplacement des modes supérieurs |
| MATTER | C | SPRK | 0–127 | Énergie relative / brillance des partiels supérieurs |
| MATTER | D | INH | 0–127 | Inharmonicité du mode supérieur |
| MATTER | E | BOW | 0–127 | Excitation continue bruitée, type archet |
| MATTER | F | EDGE | 0–127 | Pondération des modes supérieurs |
| MATTER | G | DAMP | 0–127 | Amortissement relatif des modes supérieurs |
| MATTER | H | DECY | 0–127 | Temps de décroissance |
| RESON | A | TONE | 0–127 | Balance spectrale et amortissement |
| RESON | B | HIT | 0–127 | Intensité de la frappe au déclenchement |
| RESON | C | NOIS | 0–127 | Excitation continue par bruit |
| RESON | D | RASP | 0–127 | Réduction de résolution |
| RESON | E | SPRD | 0–127 | Séparation des modes supérieurs |
| RESON | F | BODY | 0–127 | Pondération du mode fondamental |
| RESON | G | LVL | 0–127 | Niveau modal avant l'enveloppe AMP native |

**Glass Bow :** choisir verre ou cristal, SPRK moyen à élevé, INH moyen, BOW 40–70, DECY élevé et NOIS modéré. Utiliser un LFO lent sur BOW ou EDGE.

**Stone Drops :** choisir pierre, laisser BOW à zéro, augmenter HIT, utiliser DECY moyen, DAMP élevé et EDGE faible. Les parameter locks sur HIT et MTRL conviennent aux séquences de percussion accordée.

## MORPHANT — Formant Organism (ID 101)

| Page | Encodeur | Contrôle | Plage | Fonction |
|---|---:|---|---:|---|
| VOICE | A | VOWL | 0–4 | Position des profils A/E/I/O/U, interpolée |
| VOICE | B | FORM | 0–127 | Position des crêtes spectrales |
| VOICE | C | FINE | −100…+100 cents | Accord fin du fondamental |
| VOICE | D | HARM | 0–127 | Inclinaison spectrale des six partiels |
| VOICE | E | BRETH | 0–127 | Bruit de souffle filtré lentement |
| VOICE | F | NOIS | 0–127 | Bruit large bande |
| VOICE | G | DRFT | 0–127 | Profondeur du vibrato lent |
| VOICE | H | GLID | 0–127 | Vitesse du morphing ; plus haut = transition plus lente |
| SPECT | A | NASL | 0–127 | Renforcement des partiels centraux |
| SPECT | B | AIR | 0–127 | Texture de bruit passe-haut |
| SPECT | C | SUB | 0–127 | Mélange de la sous-octave |
| SPECT | D | OCT | −12…+12 demi-tons | Transposition d'octave |
| SPECT | E | BITE | 0–127 | Accent spectral et fold |
| SPECT | F | FM2 | 0–127 | Accent secondaire du cinquième partiel |
| SPECT | G | MIX | 0–127 | Balance couche harmonique / souffle |

**Vocal Glass :** VOWL proche de I/O, FORM vers le milieu, HARM moyen, NASL 20–45, AIR/BRETH modérés, GLID moyen à élevé et MIX élevé.

**Choir Dust :** automatiser le mouvement A→U, NASL moyen, BRETH/AIR modérés, DRFT subtil, SUB faible et BITE faible. Utiliser la polyphonie et les effets natifs du Digitone.

LVL est volontairement omis : la page AMP native contrôle le niveau global et le release.

## TESSERA — Phase Lattice (ID 102)

| Page | Encodeur | Contrôle | Plage | Fonction |
|---|---:|---|---:|---|
| PHASE | A | RATIO | 0–7 | Ratio du modulateur parmi huit valeurs discrètes |
| PHASE | B | INDEX | 0–127 | Profondeur de modulation de phase |
| PHASE | C | FOLD | 0–127 | Quantité de wavefold |
| PHASE | D | SKEW | 0–127 | Déformation asymétrique de phase |
| PHASE | E | SYNC | 0–127 | Synchronisation du modulateur au retour de phase porteuse |
| PHASE | F | FBK | 0–127 | Feedback de la sortie précédente |
| PHASE | G | SUB | 0–127 | Mélange de la sous-octave |
| PHASE | H | LVL | 0–127 | Niveau du générateur |
| SHAPE | A | DETUN | −100…+100 cents | Désaccord du modulateur |
| SHAPE | B | BIAS | 0–127 | Biais continu avant fold |
| SHAPE | C | RING | 0–127 | Mélange vers la composante produit |
| SHAPE | D | BITS | 0–127 | 0 désactive ; 1 est grossier ; 127 approche 10 bits |
| SHAPE | E | GLCH | 0–127 | Probabilité/résolution des glitches quantifiés |
| SHAPE | F | NOIS | 0–127 | Couche de bruit |
| SHAPE | G | MIX | 0–127 | Balance porteuse / modulateur |

**Lattice Bell :** RATIO proche de 4 ou 5, INDEX modéré, FOLD faible, SYNC élevé, FBK faible, DETUN subtil et RING moyen.

**Broken Orbit :** RATIO plus élevé, INDEX/SKEW moyens à élevés, FBK faible à moyen, BIAS légèrement décalé et GLCH faible.

DUST a été retiré car sa couche doublonnait NOIS. BITS=0 désactive la quantification ; GLCH agit dès la valeur 1.

## Prototypes hôte : guide distinct

Les six moteurs VESPER, MYCEL, ORBITAL, LITHOS, PHOTON et ANIMA possèdent leurs propres contrôles décrits dans `prototypes/*/prototype.json` et dans [PROTOTYPES.md](PROTOTYPES.md). Ils n'appartiennent pas au catalogue des machines SDK alpha sélectionnables et ne sont pas installables.

## Ordre de test recommandé après validation cible

1. Tester une seule machine et une seule piste avec niveau AMP et effets modérés.
2. Vérifier les notes MIDI 36, 48, 60, 72 et 84, l'attaque et le release AMP natif.
3. Tester chaque encodeur, les LFO, parameter locks, changements de Sound et changements de machine.
4. Ajouter progressivement voix et pistes ; profiler particulièrement TESSERA et MORPHANT.
5. Répéter dans un émulateur avant tout essai matériel.

Ces notes ne sont ni des fichiers preset `.syx`, ni des projets `.dnprj`.
