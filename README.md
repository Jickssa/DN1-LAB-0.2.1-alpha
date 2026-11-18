# DN1-LAB — Experimental synthesis research for Elektron Digitone MKI

[Français](#français) · [English](#english) · [Workbench](https://jickssa.github.io/DN1-LAB-0.2.1-alpha/workbench/) · [Issues](https://github.com/Jickssa/DN1-LAB-0.2.1-alpha/issues) · [Pull requests](https://github.com/Jickssa/DN1-LAB-0.2.1-alpha/pulls)

> **Independent community research.** DN1-LAB is not affiliated with, endorsed by, or supported by Elektron. Product and company names remain the property of their respective owners. This repository does not currently provide firmware that is certified or ready to install.

---

## Français

DN1-LAB est un laboratoire indépendant consacré à la recherche en synthèse sonore embarquée pour **Elektron Digitone MKI / Digitone Keys**. Le projet explore des moteurs DSP natifs, leurs interfaces de paramètres, les contraintes d'exécution temps réel, les outils de validation et une chaîne de préparation du firmware qui reste locale à l'utilisateur.

La cible documentaire actuelle est **Digitone MKI / OS 1.44**. La présence d'une source C, le succès d'un test hôte ou le succès d'une CI ne prouvent pas qu'un moteur fonctionne sur le processeur du Digitone.

### État du projet

| Ensemble | Contenu | Maturité |
|---|---|---|
| Machines SDK alpha | KARST, MORPHANT, TESSERA — IDs 100–102 | **L2 · tests hôte** |
| Prototypes DSP | VESPER, MYCEL, ORBITAL, LITHOS, PHOTON, ANIMA — IDs candidats 103–108 | **L2 · prototypes hôte testés** |
| Workbench | Sélection, empreinte SHA-256 locale, export d'un plan JSON | Outil de préparation uniquement |
| Construction firmware | Script de prévol et d'orchestration OS 1.44 | Présent, mais build cible non exécuté |
| Compatibilité matérielle | ColdFire, linkage combiné, émulateur, Digitone physique | **Non validée** |

**Niveau de preuve utilisé dans ce dépôt**

- **L0 — Concept :** proposition de conception sans moteur exécutable.
- **L1 — Prototype hôte :** source exécutable dans le harnais local, avant validation automatisée complète.
- **L2 — Tests hôte :** suite automatisée et reproductible, GCC/Clang, sanitizers et contrôles de bornes ; cela ne prouve pas le budget CPU ColdFire.
- **L3 — Build cible :** compilation avec la toolchain et les headers SDK exacts, puis lint/linkage combiné réussi.
- **L4 — Émulateur :** comportement testé dans un émulateur adapté à la cible.
- **L5 — Matériel :** essais documentés sur le modèle et la version d'OS indiqués.

Un moteur ne passe pas automatiquement d'un niveau au suivant. Les preuves, versions, résultats et limites doivent être publiés.

### Moteurs SDK alpha existants

| Moteur | ID | Principe |
|---|---:|---|
| **KARST — Modal Matter** | 100 | Résonateurs modaux inharmoniques, excitation et matières résonantes |
| **MORPHANT — Formant Organism** | 101 | Six partiels additifs, profils formantiques, textures souffle/bruit/air |
| **TESSERA — Phase Lattice** | 102 | Accumulateurs de phase couplés, modulation, feedback et wavefolding |

Ces trois sources sont dans `sdk-alpha/`. Elles ciblent le shared machine SDK alpha, avec quinze paramètres logiques au maximum, deux pages de contrôle et un état par voix de 256 octets au plus selon le contrat SDK documenté.

### Six prototypes DSP additionnels

| Prototype | ID candidat | Principe |
|---|---:|---|
| **VESPER — Resonant Time Loom** | 103 | Empreinte d'excitation courte, tête de lecture réversible, résonateurs |
| **MYCEL — Distributed Coupling Synth** | 104 | Quatre nœuds résonants et graphe de transferts d'énergie |
| **ORBITAL — Gesture-Conditioned Harmonic Field** | 105 | Champ harmonique 2D avec mémoire de trajectoire |
| **LITHOS — Spectral Erosion Engine** | 106 | Érosion, dépôt et fracture entre réservoirs spectraux |
| **PHOTON — Event-Driven Phase Camera** | 107 | Impulsions clairsemées et réseau de réflexion/couplage de phase |
| **ANIMA — Local Ecology Fallback** | 108 | Quatre agents virtuels coopérant à l'intérieur d'une seule voix |

Les six prototypes résident dans `prototypes/`, séparés de `sdk-alpha/` : ils ne sont pas compilés par le script de firmware. Chaque prototype expose quinze paramètres, deux pages et un état hôte mesuré de 40 à 60 octets. Les IDs 103–108 sont provisoires : seul le linker combiné peut confirmer leur validité dans l'ensemble final.

**ANIMA n'implémente pas le bus inter-voix initialement envisagé.** Le repli livré conserve les quatre agents dans l'état d'une seule voix. Aucun état global statique n'est utilisé sans garantie SDK de synchronisation et de cycle de vie.

### Commencer

- [Guide de démarrage](docs/START-HERE.md) — environnement, structure et premières commandes.
- [Présentation technique](docs/TECHNICAL-OVERVIEW.md) — architecture et séparation des couches.
- [Guide des instruments — français](docs/INSTRUMENTS.fr.md) · [Instrument guide — English](docs/INSTRUMENTS.en.md).
- [Description des six prototypes](docs/PROTOTYPES.md).
- [Rapport de validation des prototypes](docs/PROTOTYPE-VALIDATION.md).
- [Validation des trois moteurs SDK alpha](docs/VALIDATION.md).
- [Portage SDK alpha](docs/ALPHA-PORT.md) · [Pipeline firmware](docs/FIRMWARE-PIPELINE.md).
- [Catalogue des machines et propositions](catalogue/README.md) · [Recherche et antériorité](docs/RESEARCH.md).
- [Contribuer](CONTRIBUTING.md) · [Support](SUPPORT.md) · [FAQ](FAQ.md) · [Gouvernance](GOVERNANCE.md).
- [Métadonnées GitHub à configurer](docs/GITHUB-METADATA.md) · [Notes de version 0.3.0-alpha](RELEASE-NOTES-0.3.0-alpha.md).

### Tests hôte

Depuis la racine du dépôt, avec Python 3 et GCC ou Clang disponibles :

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

Pour générer les six préécoutes locales (les WAV produits ne sont pas suivis dans ce dépôt) :

```sh
CC=gcc sh tests/run_prototype_demos.sh
```

La CI GitHub du commit `329aa1429d8854a6f805e1ef495bd7557cf00794` a terminé avec succès : **six jobs sur six** — validateurs et manifestes, tests GCC, tests Clang, ASan/UBSan, analyse statique, reproductibilité des WAV. [Consulter l'exécution](https://github.com/Jickssa/DN1-LAB-0.2.1-alpha/actions/runs/44151171712).

### Construction firmware : limites impératives

Le script `scripts/build_digitone_alpha.sh` attend un checkout upstream elekloader compatible OS 1.44, les compagnons requis, la toolchain ColdFire et le fichier firmware stock exact. Son prévol contrôle l'empreinte du firmware et des dépendances avant de tenter la chaîne de build.

**Cette chaîne cible n'a pas encore été exécutée ici.** Aucun module ColdFire `.elemod`, aucun firmware modifié `.syx`, aucun installateur et aucune preuve d'essai sur Digitone physique ne sont publiés. Les tests hôte ne mesurent pas le temps CPU réel, le mélange polyphonique simultané, le vol de voix, l'enveloppe native, les effets ou l'anti-aliasing sur cible.

Ne flashe pas un artefact expérimental sur la seule base d'une CI verte. La sauvegarde, la récupération et les tests sur une unité récupérable sont des prérequis à tout essai matériel.

### Outils et principes

- **Local-first :** le Workbench hash le fichier sélectionné localement et exporte un plan JSON ; il ne téléverse, ne compile, ne patche et ne flashe rien.
- **Séparation des preuves :** concept, prototype hôte, tests hôte, build cible, émulateur et matériel sont distingués.
- **Reproductibilité :** versions, hashes, commandes, journaux et limites sont consignés.
- **Antériorité :** aucune revendication d'invention ou d'unicité n'est faite pour les concepts ; les techniques apparentées sont documentées dans la recherche.
- **Sécurité :** ne publie pas de firmware propriétaire, de secret, de dump de mémoire ou de données personnelles.

### Communauté

Les contributions en français et en anglais sont bienvenues. Utilise les [Discussions](https://github.com/Jickssa/DN1-LAB-0.2.1-alpha/discussions) pour les idées, les [Issues](https://github.com/Jickssa/DN1-LAB-0.2.1-alpha/issues) pour les problèmes ou demandes structurées et les [Pull Requests](https://github.com/Jickssa/DN1-LAB-0.2.1-alpha/pulls) pour les modifications vérifiables. Les Discussions et GitHub Pages doivent être activées dans les paramètres du dépôt si elles ne sont pas déjà disponibles.

---

## English

DN1-LAB is an independent research lab exploring embedded sound synthesis for the **Elektron Digitone MKI / Digitone Keys**. It studies native DSP engines, parameter interfaces, real-time constraints, validation tooling and a firmware preparation workflow that keeps user firmware local.

The current documented target is **Digitone MKI / OS 1.44**. The presence of C source code, a passing host test or a green CI run does not prove that an engine runs on the Digitone processor.

### Project status

| Workstream | Contents | Maturity |
|---|---|---|
| Shared SDK-alpha machines | KARST, MORPHANT, TESSERA — IDs 100–102 | **L2 · host-tested** |
| DSP prototypes | VESPER, MYCEL, ORBITAL, LITHOS, PHOTON, ANIMA — candidate IDs 103–108 | **L1 · host prototypes** |
| Workbench | Selection, local SHA-256 digest, JSON build-plan export | Preparation tool only |
| Firmware build | OS 1.44 preflight and orchestration script | Present; target build not executed |
| Hardware compatibility | ColdFire, combined linkage, emulator, physical Digitone | **Not validated** |

**Evidence levels used by this repository**

- **L0 — Concept:** design proposal without executable engine.
- **L1 — Host prototype:** source runs in the local host harness before full automated validation.
- **L2 — Host-tested:** automated, reproducible host tests; this does not prove the ColdFire CPU budget.
- **L3 — Target build:** built with the exact target toolchain and SDK headers; combined lint/link succeeds.
- **L4 — Emulator:** behavior tested in an emulator suitable for the target.
- **L5 — Hardware:** documented tests on the stated device model and OS version.

An engine does not automatically advance from one level to the next. Evidence, versions, results and limitations must be published.

### Existing shared SDK-alpha machines

| Engine | ID | Synthesis model |
|---|---:|---|
| **KARST — Modal Matter** | 100 | Inharmonic modal resonators, excitation and resonant materials |
| **MORPHANT — Formant Organism** | 101 | Six-partial additive synthesis, formant profiles, breath/noise/air textures |
| **TESSERA — Phase Lattice** | 102 | Coupled phase accumulators, modulation, feedback and wavefolding |

These three sources live in `sdk-alpha/`. They target the shared machine SDK alpha, with up to fifteen logical parameters, two custom control pages and up to 256 bytes of per-voice state under the documented SDK contract.

### Six additional DSP prototypes

| Prototype | Candidate ID | Design |
|---|---:|---|
| **VESPER — Resonant Time Loom** | 103 | Short excitation stencil, reversible read head and resonators |
| **MYCEL — Distributed Coupling Synth** | 104 | Four resonant nodes and an energy-transfer graph |
| **ORBITAL — Gesture-Conditioned Harmonic Field** | 105 | 2D harmonic field with path-dependent memory |
| **LITHOS — Spectral Erosion Engine** | 106 | Erosion, deposition and fracture across spectral reservoirs |
| **PHOTON — Event-Driven Phase Camera** | 107 | Sparse impulses and a phase reflection/coupling network |
| **ANIMA — Local Ecology Fallback** | 108 | Four virtual agents interacting inside one voice |

The six prototypes live in `prototypes/`, separate from `sdk-alpha/`; the firmware build script does not include them. Each prototype exposes fifteen parameters, two pages and a measured 40–60-byte host state. IDs 103–108 are provisional: only the combined linker can confirm their validity in the final machine set.

**ANIMA does not implement the originally proposed cross-voice bus.** The delivered fallback keeps all four agents inside one voice's state. No static global state is used without a documented SDK synchronization and lifetime contract.

### Start here

- [Getting started](docs/START-HERE.md) — environment, repository map and first commands.
- [Technical overview](docs/TECHNICAL-OVERVIEW.md) — architecture and layer boundaries.
- [Instrument guide — English](docs/INSTRUMENTS.en.md) · [Guide des instruments — français](docs/INSTRUMENTS.fr.md).
- [Six prototype descriptions](docs/PROTOTYPES.md).
- [Prototype validation report](docs/PROTOTYPE-VALIDATION.md).
- [Shared SDK-alpha validation](docs/VALIDATION.md).
- [SDK-alpha port](docs/ALPHA-PORT.md) · [Firmware pipeline](docs/FIRMWARE-PIPELINE.md).
- [Machine catalogue and proposals](catalogue/README.md) · [Research and prior art](docs/RESEARCH.md).
- [Contributing](CONTRIBUTING.md) · [Support](SUPPORT.md) · [FAQ](FAQ.md) · [Governance](GOVERNANCE.md).
- [GitHub repository metadata](docs/GITHUB-METADATA.md) · [0.3.0-alpha release notes](RELEASE-NOTES-0.3.0-alpha.md).

### Host tests

From the repository root, with Python 3 and GCC or Clang available:

```sh
python3 tools/validate_catalogue.py
python3 tools/validate_site.py
python3 tests/validate_prototypes.py

CC=gcc sh tests/run_prototype_tests.sh
CC=clang sh tests/run_prototype_tests.sh
sh tests/run_prototype_sanitizers.sh
sh tests/run_prototype_analyzers.sh
sh tests/run_prototype_reproducibility.sh
```

To generate the six local preview WAV files (generated WAVs are not tracked in this repository):

```sh
CC=gcc sh tests/run_prototype_demos.sh
```

GitHub Actions completed successfully for commit `329aa1429d8854a6f805e1ef495bd7557cf00794`: **six of six jobs passed** — metadata and manifests, GCC tests, Clang tests, ASan/UBSan, static analysis and WAV reproducibility. [View the run](https://github.com/Jickssa/DN1-LAB-0.2.1-alpha/actions/runs/44151171712).

### Firmware build: critical limitations

The `scripts/build_digitone_alpha.sh` script expects a compatible elekloader OS 1.44 checkout, required companion modules, a ColdFire toolchain and the exact stock firmware image. Its preflight checks the firmware digest and dependency versions before attempting the build pipeline.

**The target build pipeline has not yet been executed here.** No ColdFire `.elemod`, modified `.syx`, installer or physical Digitone test evidence is published. Host tests do not measure actual target CPU time, simultaneous polyphonic mixing, voice stealing, native envelopes, effects or target anti-aliasing.

Do not flash an experimental artifact merely because CI is green. Backup, recovery and testing on a recoverable unit are prerequisites for hardware experiments.

### Tools and principles

- **Local-first:** the Workbench hashes a selected file locally and exports a JSON plan; it does not upload, compile, patch or flash.
- **Evidence separation:** concepts, host prototypes, host tests, target builds, emulator tests and hardware tests are distinct.
- **Reproducibility:** versions, digests, commands, logs and limitations are recorded.
- **Prior art:** no invention or uniqueness claim is made for the concepts; related techniques are documented in the research notes.
- **Security:** do not publish proprietary firmware, secrets, memory dumps or personal data.

### Community

Contributions in French and English are welcome. Use [Discussions](https://github.com/Jickssa/DN1-LAB-0.2.1-alpha/discussions) for ideas, [Issues](https://github.com/Jickssa/DN1-LAB-0.2.1-alpha/issues) for structured problems and requests, and [Pull Requests](https://github.com/Jickssa/DN1-LAB-0.2.1-alpha/pulls) for reviewable changes. Discussions and GitHub Pages may need to be enabled in repository settings.
