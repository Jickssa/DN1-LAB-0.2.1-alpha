# Historique DN1 LAB

## 0.3.0-alpha — six prototypes DSP additionnels (hôte seulement)

- Ajout de VESPER : mémoire courte d'excitation en 32 cellules, tête de lecture réversible et deux résonateurs amortis.
- Ajout de MYCEL : quatre nœuds résonants liés par six transferts d'énergie symétriques ; l'énergie est non croissante dans le test sans excitation aléatoire.
- Ajout d'ORBITAL : champ harmonique piloté par trajectoire 2D, avec test montrant une différence de timbre pour deux chemins atteignant le même point final.
- Ajout de LITHOS : six partiels et trois réservoirs spectraux (lisse, granulaire, fracturé), avec bilan de transfert conservatif à l'arrondi près.
- Ajout de PHOTON : événements clairsemés et réseau de phase à quatre nœuds ; FEEDBACK effectue un transfert borné plutôt qu'une injection d'énergie en boucle.
- Ajout d'ANIMA : repli sûr à quatre agents par voix. Le bus inter-voix est explicitement non implémenté tant que le SDK ne garantit pas une mémoire globale synchronisée.
- Les six prototypes comportent quinze paramètres et deux pages, avec 40 à 60 octets d'état par voix mesurés sur l'hôte ; les IDs 103–108 restent provisoires.
- Ajout d'un harnais qui balaye MIDI 0–127 et les min/défaut/max, rend huit états indépendants, vérifie l'ordre d'appel, les redéclenchements, les bornes et l'absence de clamps Q15. Ajout de tests d'invariant énergétique et d'hystérésis ORBITAL.
- GCC 14.2, Clang 17, ASan/UBSan sous les deux compilateurs, GCC `-fanalyzer` et Clang `--analyze` passent. Un overflow int32 détecté dans le vibrato ORBITAL a été corrigé puis retesté par sanitizers.
- Ajout de six préécoutes WAV mono 48 kHz. Le fondu final est généré par le banc d'essai et n'est pas le modèle de l'enveloppe AMP native.

**État :** prototypes hôte réussis uniquement. Aucun `.elemod`, build ColdFire, lint/linker combiné, profil CPU, émulateur ou essai matériel n'a été effectué pour ces six moteurs. Ils ne sont pas installables ; les sources restent hors du chemin de construction firmware.


## 0.2.2-alpha — correction de l'état MORPHANT au retrigger

- Ajout d'un test de régression qui chauffe les filtres de texture BRETH/AIR, redéclenche la même voix et compare le premier bloc au rendu d'un état vocal neuf.
- Le test échouait avant correction : MORPHANT conservait `breath_lp` et `air_lp` entre deux notes jouées sur le même slot vocal, contrairement aux réinitialisations de phase, de morphing et de PRNG.
- Réinitialisation des deux mémoires de filtre au même moment que la phase, le vibrato et le générateur pseudo-aléatoire. Le nouveau test passe maintenant à l'identité échantillon par échantillon.
- La suite instrumentée inclut 1 600 échantillons supplémentaires ; aucun clamp dur n'est relevé sur 2 081 888 échantillons.
- Version des trois manifests et de la sortie de build portée à `0.2.2-alpha`.

**État :** test de fuite d'état reproduit avant correction et passé après correction ; GCC, Clang, ASan/UBSan sous les deux compilateurs, GCC `-fanalyzer`, builds hôte `-Os`, tests historiques et vérification stock OS 1.44 ont été relancés et passent sur cette révision. Les WAV GCC/Clang sont identiques octet par octet. La compilation ColdFire et les validations DigiKit/digiemu/matériel restent bloquantes.

## 0.2.1-alpha — correction DSP, headroom et audit de régression

- Correction du signe du vibrato MORPHANT : l'incrément de phase n'est plus mélangé à un terme signé dans une expression promue en non signé ; les deux demi-cycles sont maintenant symétriques.
- Séparation fonctionnelle des trois textures MORPHANT : BRETH = bruit passe-bas lent, NOIS = bruit large bande, AIR = composante passe-haut.
- Correction de TESSERA : BITS ne possède plus de plateau de silence aux petites valeurs ; GLCH agit dès 1 au lieu de rester inactif sur ses premières positions.
- Correction de KARST : un coefficient modal de cosinus quantifié à zéro ne désactive plus le résonateur ; les partiels dont la hauteur dépasse la limite MIDI SDK sont écartés au lieu d'être rabattus silencieusement sur MIDI 127.
- Optimisation de MORPHANT : le nombre de partiels sous Nyquist est déterminé une fois par bloc avec la déviation maximale du vibrato, sans division variable dans la boucle d'échantillons.
- Mutualisation de la quantification par puissance de deux et du pliage à trois réflexions ; les propriétés sont comparées à des références entières sur l'ensemble des bornes prouvées. Les shifts larges (jusqu'à 40 dans le test) sont aussi définis sans comportement indéfini, et le poids Q16 est borné à 65536.
- Réduction de calcul dans KARST par quantification en décalages entiers, hors division variable par puissance de deux dans la boucle audio.
- Réglage des headrooms : KARST réserve environ 15 % dans le stress hôte huit états ; TESSERA reçoit une atténuation fixe de 245/256 qui élimine les clamps observés au scénario all-controls-max.
- Ajout d'un compteur de clamps exclusivement activé dans les binaires de test. Il est absent du chemin de production. Le harnais final ne détecte aucun clamp sur 2 080 288 échantillons hôte.
- Test d'indépendance de l'état renforcé de deux à huit voix : comparaison bit à bit entre rendu croissant et décroissant sur 256 blocs et redéclenchements.
- Version des trois manifests portée à `0.2.1-alpha` ; ajout d'un validateur statique de cohérence des trois manifests et du prévol du script de build.
- Audit partiel du registre public : l'espace partagé `dnmachine` est confirmé dans le profil Digitone ; DRUM+ déclare `dnmachine:78`, aucun conflit détecté avec les IDs 100–102 dans les artefacts inspectés. Le contrôle du linker combiné reste obligatoire.
- Ajout de `tests/run_alpha_production_like.sh` : build hôte `-Os`, sans compteur de test, puis comparaison de WAV par `cmp` ; GCC et Clang passent.
- Reproductibilité intercompilateurs : les trois WAV de GCC 14.2 et Clang 17 sont identiques octet par octet ; chaque build `-Os` sans compteur retrouve aussi sa référence instrumentée.

**État :** GCC, Clang, ASan/UBSan, GCC `-fanalyzer`, tests historiques et validation stock OS 1.44 exécutés. La compilation ColdFire, le lint upstream, le firmware combiné, digiemu/DigiKit et le test matériel restent non exécutés. Le budget temps réel demeure inconnu.

## 0.2.0-alpha — portage shared machine SDK

- Conservation des trois moteurs : KARST (résonateurs modaux), MORPHANT (synthèse additive formantique), TESSERA (lattice de phase).
- Portage vers `struct elek_machine` et `ELEK_REGISTER_MACHINE`, avec 15 contrôles logiques maximum, deux pages personnalisées et état indépendant par voix (≤256 octets).
- Conversion des valeurs par défaut depuis les plages raw 8.8 du firmware OS 1.44 vers les nouvelles plages logiques des encodeurs.
- Remplacement de l'oscillateur sinus par une table quart d'onde Q15 interpolée et ajout d'une interpolation Q16 à somme bornée.
- Correction des dépassements entiers potentiels dans les mélanges pondérés de TESSERA.
- Tests élargis à toute la plage MIDI 0–127, à huit voix, aux redéclenchements et à l'indépendance des états selon l'ordre de rendu.
- Prévol de build : le script vérifie le port OS 1.44 du core et du compagnon, ainsi que leur minimum de version SDK alpha.
- Documentation mise à jour avec les contraintes temporelles mesurées publiquement sur le SDK/compagnon Digitone.

**État :** tests hôte locaux réussis avec GCC, Clang, ASan/UBSan et GCC `-fanalyzer`. La compilation ColdFire, le lint, le firmware combiné, le test digiemu et le test matériel n'ont pas été exécutés. La marge CPU des trois moteurs reste inconnue.

## 0.1.1 — branche historique

- Première version des trois moteurs sur l'API historique `dnm_machine`.
- Seize contrôles empruntant les paramètres SYN1/SYN2 et la table `dn_machines` du compagnon historique.
- Sources conservées dans `modules/`, documentation d'origine dans `docs/*-0.1.1.md` et préécoutes dans `audio/`.