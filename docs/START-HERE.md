# DN1-LAB — commencer ici / Start here

## Français

### Ce qu'est ce dépôt
DN1-LAB contient trois moteurs portés vers le shared machine SDK alpha (KARST, MORPHANT, TESSERA), six prototypes DSP hôte (VESPER, MYCEL, ORBITAL, LITHOS, PHOTON, ANIMA), un catalogue, une documentation, un Workbench statique et des validateurs. Il ne contient pas de firmware modifié prêt à flasher.

### Parcours conseillé
1. Lire le [README](../README.md) et le [statut du projet](TECHNICAL-OVERVIEW.md).
2. Comprendre les différences entre [machines SDK alpha](INSTRUMENTS.fr.md) et [prototypes hôte](PROTOTYPES.md).
3. Consulter les rapports [validation SDK alpha](VALIDATION.md) et [validation des prototypes](PROTOTYPE-VALIDATION.md).
4. Examiner les sources dans `sdk-alpha/` et `prototypes/`.
5. Exécuter les contrôles hôte documentés avant toute proposition de modification.
6. Lire [CONTRIBUTING](../CONTRIBUTING.md) avant d'ouvrir une Issue ou une Pull Request.

### Environnement hôte
- Python 3 pour les validateurs.
- GCC et/ou Clang avec un compilateur C99 pour les bancs de test.
- POSIX shell pour les scripts de test.
- Git pour récupérer la branche et conserver un état reproductible.

Commandes de base :

```sh
python3 tools/validate_catalogue.py
python3 tools/validate_site.py
python3 tools/validate_docs.py
python3 tests/validate_prototypes.py
CC=gcc sh tests/run_prototype_tests.sh
CC=clang sh tests/run_prototype_tests.sh
sh tests/run_prototype_sanitizers.sh
sh tests/run_prototype_analyzers.sh
sh tests/run_prototype_reproducibility.sh
```

Les préécoutes peuvent être générées localement avec `CC=gcc sh tests/run_prototype_demos.sh`. Elles sont des rendus hôte secs, pas des enregistrements du Digitone.

### Construire pour le Digitone
Le script `scripts/build_digitone_alpha.sh` est prévu pour les trois machines de `sdk-alpha/`. Il exige le checkout upstream elekloader compatible OS 1.44, les compagnons exacts, la toolchain ColdFire et le SysEx stock exact. La chaîne réelle n'a pas encore été validée ; aucun fichier `.elemod` ni firmware modifié n'est livré.

Les six répertoires de `prototypes/` ne sont pas inclus dans ce builder. Leur passage vers le SDK cible est une étape de développement future, après analyse du coût DSP et validation de leurs interfaces.

### Aide et échanges
- Idées et orientations : [GitHub Discussions](https://github.com/Jickssa/DN1-LAB-0.2.1-alpha/discussions), si la fonction est activée.
- Défauts reproductibles et demandes : [Issues](https://github.com/Jickssa/DN1-LAB-0.2.1-alpha/issues).
- Modifications : [Pull Requests](https://github.com/Jickssa/DN1-LAB-0.2.1-alpha/pulls).
- Voir aussi [Support](../SUPPORT.md) et [FAQ](../FAQ.md).

## English

### What this repository contains
DN1-LAB contains three shared SDK-alpha engines (KARST, MORPHANT, TESSERA), six host DSP prototypes (VESPER, MYCEL, ORBITAL, LITHOS, PHOTON, ANIMA), a catalogue, documentation, a static Workbench and validators. It does not provide modified firmware that is ready to flash.

### Recommended path
1. Read the [README](../README.md) and [project status](TECHNICAL-OVERVIEW.md).
2. Understand the difference between [SDK-alpha machines](INSTRUMENTS.en.md) and [host prototypes](PROTOTYPES.md).
3. Review the [SDK-alpha validation](VALIDATION.md) and [prototype validation](PROTOTYPE-VALIDATION.md) reports.
4. Browse source under `sdk-alpha/` and `prototypes/`.
5. Run the documented host checks before proposing changes.
6. Read [CONTRIBUTING](../CONTRIBUTING.md) before opening an Issue or Pull Request.

### Host environment
- Python 3 for validators.
- GCC and/or Clang with a C99 compiler for host test harnesses.
- POSIX shell for test scripts.
- Git for checking out the branch and retaining reproducible state.

Run the commands listed in the README. Preview WAVs are host renders, not recordings from a Digitone.

### Building for Digitone
The `scripts/build_digitone_alpha.sh` script targets the three machines in `sdk-alpha/`. It requires a compatible elekloader OS 1.44 checkout, exact companion modules, a ColdFire toolchain and the exact stock SysEx. The actual target pipeline has not yet been validated; no `.elemod` or modified firmware is delivered.

The six `prototypes/` directories are not included in this builder. Porting them to the target SDK is future work, after DSP-cost analysis and interface validation.

### Help and discussion
Use [GitHub Discussions](https://github.com/Jickssa/DN1-LAB-0.2.1-alpha/discussions) for ideas when enabled, [Issues](https://github.com/Jickssa/DN1-LAB-0.2.1-alpha/issues) for reproducible defects and requests, and [Pull Requests](https://github.com/Jickssa/DN1-LAB-0.2.1-alpha/pulls) for changes. See [Support](../SUPPORT.md) and [FAQ](../FAQ.md).
