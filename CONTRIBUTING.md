# Contributing / Contribuer

DN1-LAB accepts code, DSP research, test cases, documentation, translation and audio-design contributions in French or English.

## Français

### Avant de contribuer
- Lis le [guide de démarrage](docs/START-HERE.md), la [gouvernance](GOVERNANCE.md), le [code de conduite](CODE_OF_CONDUCT.md) et la [carte des licences](LICENSING.md).
- Vérifie les Issues ouvertes et les Pull Requests pour éviter les doublons.
- Distingue clairement les trois machines SDK alpha des six prototypes hôte. Ne place pas un prototype dans le chemin firmware sans les validations requises.

### Issues et Discussions
- Discussions : concepts de machines, objectifs sonores, questions ouvertes et idées générales (si la fonction est activée).
- Issues : bugs reproductibles, tâches, demande de fonctionnalité, documentation manquante ou rapport de validation.
- Une proposition de moteur doit expliquer ce qu'elle permet musicalement et pourquoi les moteurs existants ne couvrent pas déjà ce besoin.

### Exigences pour une modification DSP
Inclure :
1. Objectif sonore et architecture algorithmique.
2. Plage et signification de chaque paramètre, unités et valeurs par défaut.
3. Hypothèses sur fréquence d'échantillonnage, taille de bloc, format numérique et fréquence de contrôle.
4. Bornes des états, feedback, énergie, phase et intermédiaires entiers.
5. Taille d'état par voix, nombre de partiels/nœuds et estimation du pire cas CPU.
6. Gestion du retrigger, des notes hors plage, des valeurs extrêmes et des états silencieux.
7. Antériorités et références pertinentes ; ne pas prétendre à une nouveauté absolue sans recherche.
8. Licence, provenance et dépendances.

### Tests attendus
Pour les prototypes hôte, exécuter lorsque pertinent :

```sh
python3 tests/validate_prototypes.py
CC=gcc sh tests/run_prototype_tests.sh
CC=clang sh tests/run_prototype_tests.sh
sh tests/run_prototype_sanitizers.sh
sh tests/run_prototype_analyzers.sh
sh tests/run_prototype_reproducibility.sh
```

Pour les changements de catalogue/site :

```sh
python3 tools/validate_catalogue.py
python3 tools/validate_site.py
```

Décrire les commandes réellement exécutées et leur résultat. Ne pas transformer une validation non exécutée en case cochée.

### Pull Request
Chaque PR doit inclure la motivation, le périmètre, les fichiers modifiés, les commandes et résultats, les limites, les risques, le niveau de maturité et les captures/sons non confidentiels si utiles. Mets à jour la documentation FR/EN ou ouvre une tâche de traduction.

Ne soumets pas de firmware propriétaire, de clé, de secret, de données personnelles ou de binaire expérimental présenté comme installable.

## English

### Before contributing
- Read the [getting-started guide](docs/START-HERE.md), [governance](GOVERNANCE.md), [code of conduct](CODE_OF_CONDUCT.md) and [licensing map](LICENSING.md).
- Check existing Issues and Pull Requests to avoid duplicate work.
- Keep the three shared SDK-alpha engines distinct from the six host prototypes. Do not move a prototype into the firmware path before it passes the required gates.

### Issues and Discussions
- Discussions: engine concepts, sonic goals, open questions and general ideas (when enabled).
- Issues: reproducible bugs, tasks, feature requests, missing documentation or validation reports.
- A machine proposal must state what it enables musically and why existing engines do not already cover the use case.

### DSP change requirements
Include the sonic goal, algorithm, parameter ranges/units/defaults, sample-rate/block/numeric assumptions, bounds on state/feedback/energy/phase/integer intermediates, per-voice state size, worst-case CPU estimate, retrigger and edge-case behavior, relevant prior art, license, provenance and dependencies.

### Expected tests
For host prototypes, run the applicable commands from the README. For catalogue/site changes, run the catalogue and site validators. State exactly what was run and what passed; never mark an unexecuted check as complete.

### Pull Requests
Every PR should explain motivation, scope, changed files, exact commands and results, limitations, risks, maturity level and non-confidential evidence where useful. Update French and English documentation or open a translation task.

Do not submit proprietary firmware, keys, secrets, personal data or experimental binaries represented as installable.
