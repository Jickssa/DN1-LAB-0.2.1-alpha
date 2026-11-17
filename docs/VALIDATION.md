# Validation policy / Politique de validation

Chaque résultat doit préciser ce qui a réellement été exécuté.

| Niveau | Preuve | Formulation permise |
|---|---|---|
| L0 | idée/spécification | proposé, concept |
| L1 | revue statique | inspecté statiquement |
| L2 | tests hôte | testé sur hôte avec environnement indiqué |
| L3 | compilation SDK cible | compilé avec SDK/version indiqués |
| L4 | émulateur | testé dans émulateur indiqué |
| L5 | matériel | testé sur Digitone MKI + OS exact |

Un test hôte ne prouve ni compatibilité ColdFire, ni budget temps réel, ni fonctionnement sur le synthétiseur.

## État du dépôt à sa création
- Cible documentaire : Digitone MKI OS 1.44.
- SDK : shared machine SDK elekloader alpha, version/commit à verrouiller.
- KARST, MORPHANT, TESSERA : fiches de recherche ; sources non importées dans ce dépôt.
- Build firmware : non implémenté.
- Essai émulateur/matériel : aucun dans ce dépôt.

Pour chaque futur rapport, consigner le hash et la source du firmware, commit SDK, compilateur/options, logs, résultats de tests, sample rate/bloc, coût par voix et nombre de voix, cas note-on/off/retrigger/voice stealing, changements de paramètres, limites et essais manquants.

## English
Every result must say exactly what was executed.

| Level | Evidence | Permitted wording |
|---|---|---|
| L0 | idea/specification | proposed, concept |
| L1 | static review | statically inspected |
| L2 | host tests | host-tested with stated environment |
| L3 | target SDK build | built with stated SDK/version |
| L4 | emulator | tested in stated emulator |
| L5 | hardware | tested on Digitone MKI + exact OS |

Host tests do not prove ColdFire compatibility, real-time budget or operation on the synthesizer.

## Repository status at initialization
- Documentation target: Digitone MKI OS 1.44.
- SDK: elekloader shared machine SDK alpha, version/commit to be pinned.
- KARST, MORPHANT, TESSERA: research entries; source code not imported here.
- Firmware build: not implemented.
- Emulator/hardware test: none in this repository.

Future reports must record firmware origin/digest, SDK commit, compiler/flags, logs, tests, sample rate/block, per-voice cost and voice count, note-on/off/retrigger/voice stealing, parameter updates, limitations and missing tests.
