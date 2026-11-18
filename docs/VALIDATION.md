# DN1-LAB 0.3.0-alpha — validation record / Rapport de validation

## Résumé

Cible documentaire : Digitone MKI / Digitone Keys OS 1.44. Le dépôt distingue les trois machines shared SDK-alpha et les six prototypes hôte. **Les validations décrites ici sont des validations hôte et CI du dépôt ; aucun niveau de compatibilité matérielle n'est revendiqué.**

## 1. Machines SDK alpha : KARST, MORPHANT, TESSERA

Le paquet de travail DN1-LAB 0.2.2-alpha a passé les contrôles suivants dans l'environnement de préparation :

| Vérification | Résultat | Portée |
|---|---|---|
| Manifestes alpha / IDs / cibles | PASS | IDs 100/101/102, OS 1.44, 15 paramètres |
| Harnais DSP alpha GCC / Clang | PASS | Notes MIDI, huit états indépendants, retrigger, ordre de rendu |
| Build hôte de type production `-Os` | PASS | Rendus GCC/Clang byte-identical |
| ASan + UBSan sous GCC et Clang | PASS | Aucun défaut signalé par ces suites |
| GCC `-fanalyzer` | PASS | Analyse statique du code couvert |
| Tests API historique 0.1.1 | PASS | Branche `dnm_machine` dans le paquet de travail |
| Validateurs de plages / métadonnées | PASS | Valeurs et plages des trois machines |
| Équivalence du fold TESSERA alpha | PASS | 416 245 valeurs dans le domaine testé |
| Vérificateur firmware stock OS 1.44 | PASS | Entrée locale fournie, non redistribuée |

Le harnais alpha a rendu 2 081 888 échantillons hôte sans clamp dur signalé par son compteur de test. Les huit lanes sont huit états séparés rendus en séquence, pas un vrai mixage polyphonique simultané.

Le harnais complet 0.2.2-alpha et l'ancienne API 0.1.1 restent dans l'archive locale de travail et ne sont pas tous importés fichier par fichier dans ce dépôt. Ces résultats ne doivent pas être présentés comme des tests relancés par GitHub Actions.

## 2. Six prototypes hôte : VESPER, MYCEL, ORBITAL, LITHOS, PHOTON, ANIMA

Les sources sont sous `prototypes/` et sont séparées de `sdk-alpha/`. Le sous-ensemble d'interface dans `host/include/` sert uniquement au banc de test local.

- GCC 14.2 et Clang 17 : tests hôte réussis.
- ASan/UBSan sous GCC et Clang : réussis.
- GCC `-fanalyzer` et Clang `--analyze` : aucune alerte signalée.
- Comparaison des six préécoutes : WAV GCC et Clang identiques octet par octet.
- Balayage des notes MIDI 0–127, paramètres min/défaut/max, patch par défaut, coins combinés, huit états indépendants, ordre de rendu, retrigger et bornes internes : réussi.
- ORBITAL : test d'hystérésis vérifiant deux trajectoires différentes vers une même position finale.
- MYCEL et LITHOS : tests de bornes et d'invariants énergétiques.
- Aucun clamp Q15 instrumenté détecté dans les suites couvertes.

Les huit lanes sont huit structures d'état rendues séquentiellement. Elles ne représentent pas un mixage simultané de huit voix ni une mesure de cycles DSP.

Détails et pics hôte : [PROTOTYPE-VALIDATION.md](PROTOTYPE-VALIDATION.md). Les préécoutes mono 48 kHz sont générées localement par `tests/run_prototype_demos.sh` et ne sont pas suivies dans le dépôt.

## 3. CI GitHub sur la branche communautaire

Dernière exécution vérifiée pour le commit `329aa1429d8854a6f805e1ef495bd7557cf00794` :

[GitHub Actions — run 44151171712](https://github.com/Jickssa/DN1-LAB-0.2.1-alpha/actions/runs/44151171712)

| Job | Résultat |
|---|---|
| Catalogue, manifestes et metadata | PASS |
| Tests DSP prototypes GCC | PASS |
| Tests DSP prototypes Clang | PASS |
| ASan / UBSan GCC et Clang | PASS |
| Analyse statique GCC et Clang | PASS |
| Reproductibilité des six WAV | PASS |

Cette CI valide les tests hôte des six prototypes et les validateurs du dépôt. Elle ne compile pas les trois machines existantes contre le builder upstream ColdFire et ne produit pas de firmware.

## 4. Identité du firmware stock vérifié localement

Le fichier OS 1.44 fourni pour l'analyse a été identifié par les empreintes suivantes :

- SHA-256 SysEx : `d4f200d04484333d82822db7744e6484d0def8f2db8ddf55ee2b780cc13c9659`
- Conteneur ELE3 : 1 320 656 octets ; SysEx device ID `0x0D` ; sept sections `[5, 2, 3, 4, 6, 7, 8]`.
- Section principale : 2 736 304 octets décompressés à `0x40000400`.
- SHA-256 de l'image principale : `fce648a97c6c5d93b961732e8f8db6b02e0820c6d344c7e2131fa05a4b3168e4`.

Ces empreintes identifient l'entrée locale utilisée pour l'analyse. Elles ne confèrent aucun droit de redistribution et le firmware propriétaire n'est pas inclus.

## 5. Étapes non exécutées — blocage matériel

- Build ColdFire des trois machines SDK alpha et des six prototypes.
- Compilation avec les vrais headers SDK upstream pour les prototypes.
- Lint/linkage combiné avec core et compagnon OS 1.44 exacts.
- Validation des IDs 103–108 dans le registre complet et du nombre maximal de machines.
- Production et vérification d'un firmware modifié `.syx`.
- Test DigiKit/digiemu ou d'un émulateur adapté.
- Profilage cycles/bloc, latence et charge de huit voix simultanées sur le ColdFire.
- Essais physiques : note-off/release, p-locks, LFO, MIDI, changement de Sound/machine, persistance et voice stealing.

**Conclusion :** les moteurs disposent de preuves de tests hôte, pas de preuve de compatibilité ou d'installation Digitone. Ne pas flasher les sources alpha.
