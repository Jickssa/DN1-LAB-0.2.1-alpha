# Historique DN1 LAB

## 0.2.2-alpha — correction de l'état MORPHANT au retrigger
- Test de régression : chauffe les filtres BRETH/AIR, redéclenche la même voix et compare le premier bloc au rendu d'un état vocal neuf.
- Le test échouait avant correction : MORPHANT conservait breath_lp et air_lp entre deux notes du même slot vocal.
- Réinitialisation des deux mémoires de filtre avec la phase, le vibrato et le PRNG.
- La suite inclut 1 600 échantillons supplémentaires ; aucun clamp dur n'est relevé sur 2 081 888 échantillons.
- Version des trois manifests portée à 0.2.2-alpha.

État : GCC, Clang, ASan/UBSan sous les deux compilateurs, GCC -fanalyzer, builds hôte -Os, tests historiques et vérification stock OS 1.44 relancés et passés. Les WAV GCC/Clang sont identiques octet par octet. Compilation ColdFire et validations émulateur/matériel restent bloquantes.

## 0.2.1-alpha — correction DSP, headroom et régression
- Correction du signe du vibrato MORPHANT et séparation des textures BRETH, NOIS et AIR.
- TESSERA : BITS n'a plus de plateau de silence ; GLCH agit dès sa première valeur.
- KARST : un coefficient modal de cosinus nul ne désactive plus le résonateur ; les notes hors plage SDK sont écartées.
- MORPHANT : nombre de partiels autorisés déterminé une fois par bloc.
- Quantification par puissance de deux et pliage à trois réflexions avec tests d'équivalence entiers.
- Headroom KARST/TESSERA ; compteur de clamps réservé aux tests.
- Indépendance d'état renforcée à huit voix et 256 blocs avec retrigger.
- Build hôte -Os et comparaison byte-identical GCC/Clang.

État : tests hôte réussis ; ColdFire, lint upstream, firmware combiné, émulation et matériel non exécutés.

## 0.2.0-alpha — portage shared machine SDK
- Portage vers struct elek_machine et ELEK_REGISTER_MACHINE, 15 paramètres, deux pages et état par voix ≤256 octets.
- Conversion des valeurs initiales depuis les plages raw 8.8 du firmware OS 1.44.
- Table sinusoïdale Q15 interpolée, interpolation Q16 bornée et corrections de dépassements entiers.
- Tests étendus aux notes MIDI, huit voix, retrigger et indépendance d'état.

État : tests hôte locaux réussis ; compilation ColdFire, lint, firmware combiné, émulation et matériel non exécutés.

## 0.1.1 — branche historique
Première version des trois moteurs sur l'API dnm_machine, seize contrôles SYN1/SYN2 et table dn_machines. Sources conservées dans modules/.
