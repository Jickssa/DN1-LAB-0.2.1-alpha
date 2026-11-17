# DN1-LAB — Machines de synthèse expérimentales pour Digitone MKI

[Français](#français) · [English](#english) · [Catalogue / Workbench](https://jickssa.github.io/DN1-LAB-0.2.1-alpha/)

> Laboratoire indépendant. DN1-LAB n'est ni affilié à Elektron, ni un produit officiel. Les modules alpha ne sont pas certifiés installables.

## Français

DN1-LAB développe des moteurs de synthèse natifs et des outils de recherche pour Elektron Digitone MKI. La version source intégrée est **0.2.2-alpha**, ciblant exclusivement le Digitone MKI / Digitone Keys OS 1.44 avec le shared machine SDK alpha d'elekloader.

### Démarrage rapide

- [Workbench — catalogue et planificateur local](https://jickssa.github.io/DN1-LAB-0.2.1-alpha/workbench/)
- [Vue technique détaillée](docs/TECHNICAL-OVERVIEW.md)
- [Guide des instruments](docs/INSTRUMENTS.md)
- [Rapport de validation](docs/VALIDATION.md)
- [Architecture et portage SDK](docs/ALPHA-PORT.md)
- [État de la recherche](docs/RESEARCH.md)
- [Contribuer](CONTRIBUTING.md)

### Les trois moteurs inclus

| Machine | ID | Synthèse | Contrôles SDK | État |
|---|---:|---|---:|---|
| KARST — Modal Matter | 100 | Résonateurs modaux inharmoniques et matières virtuelles | 15 | Tests hôte réussis ; build ColdFire / matériel non vérifié |
| MORPHANT — Formant Organism | 101 | Six partiels, profils formantiques et trois textures distinctes | 15 | Tests hôte réussis ; build ColdFire / matériel non vérifié |
| TESSERA — Phase Lattice | 102 | Deux accumulateurs de phase couplés, feedback et repliement | 15 | Tests hôte réussis ; build ColdFire / matériel non vérifié |

La branche historique `modules/` reste disponible pour l'API `dnm_machine` 0.1.1. La branche `sdk-alpha/` contient le port séparé vers `struct elek_machine` et `ELEK_REGISTER_MACHINE`.

### Vérification locale

Prérequis : Python 3, GCC ou Clang, environnement POSIX. Depuis la racine :

```sh
sha256sum -c SHA256SUMS.txt
python3 tests/validate_alpha_manifests.py
python3 tools/validate_catalogue.py
python3 tools/validate_site.py
./tests/run_alpha_tests.sh
./tests/run_alpha_production_like.sh
./tests/run_alpha_sanitizers.sh
./tests/run_alpha_analyzer.sh
```

Les tests historiques sont `./tests/run_host_tests.sh`, `./tests/run_sanitizers.sh`, `python3 tests/validate_metadata.py` et `python3 tests/test_tessera_fold.py`.

### Limites essentielles

Cette version est un paquet de sources et de tests hôte. Aucun `.elemod` ColdFire ni firmware modifié `.syx` n'est fourni. Le build officiel via le vrai checkout elekloader, le linker, le runner hôte upstream, l'émulateur et le Digitone réel n'ont pas été validés ici. Les tests hôte ne démontrent ni budget temps réel, ni compatibilité matérielle. Lire intégralement [VALIDATION.md](docs/VALIDATION.md) avant toute tentative de portage.

Le Workbench vérifie localement le hash d'un fichier sélectionné dans le navigateur et exporte un plan JSON ; il **ne modifie pas** le firmware et n'envoie aucun fichier à un serveur.

### Communauté

Les contributions FR et EN sont ouvertes : [Discussions](https://github.com/Jickssa/DN1-LAB-0.2.1-alpha/discussions), [Issues](https://github.com/Jickssa/DN1-LAB-0.2.1-alpha/issues), [Pull Requests](https://github.com/Jickssa/DN1-LAB-0.2.1-alpha/pulls). La publication du site requiert la source GitHub Pages réglée sur « GitHub Actions » dans Settings → Pages ; l'activation des Discussions requiert également le réglage correspondant dans les paramètres du dépôt.

## English

DN1-LAB develops native synthesis engines and research tools for the Elektron Digitone MKI. The integrated source release is **0.2.2-alpha**, targeting Digitone MKI / Digitone Keys OS 1.44 only, with elekloader's shared machine SDK alpha.

### Quick start

- [Workbench — catalogue and local planner](https://jickssa.github.io/DN1-LAB-0.2.1-alpha/workbench/)
- [Technical overview](docs/TECHNICAL-OVERVIEW.md)
- [Instrument guide](docs/INSTRUMENTS.md)
- [Validation report](docs/VALIDATION.md)
- [Architecture and SDK port](docs/ALPHA-PORT.md)
- [Research state](docs/RESEARCH.md)
- [Contributing](CONTRIBUTING.md)

### Included engines

| Machine | ID | Synthesis | SDK controls | Status |
|---|---:|---|---:|---|
| KARST — Modal Matter | 100 | Inharmonic modal resonators and virtual materials | 15 | Host tests pass; ColdFire build / hardware unverified |
| MORPHANT — Formant Organism | 101 | Six partials, formant profiles and three distinct textures | 15 | Host tests pass; ColdFire build / hardware unverified |
| TESSERA — Phase Lattice | 102 | Two coupled phase accumulators, feedback and wavefolding | 15 | Host tests pass; ColdFire build / hardware unverified |

The legacy `modules/` tree remains available for the 0.1.1 `dnm_machine` API. `sdk-alpha/` contains a separate port using `struct elek_machine` and `ELEK_REGISTER_MACHINE`.

### Local validation

Requirements: Python 3, GCC or Clang, POSIX shell environment. Run from the repository root:

```sh
sha256sum -c SHA256SUMS.txt
python3 tests/validate_alpha_manifests.py
python3 tools/validate_catalogue.py
python3 tools/validate_site.py
./tests/run_alpha_tests.sh
./tests/run_alpha_production_like.sh
./tests/run_alpha_sanitizers.sh
./tests/run_alpha_analyzer.sh
```

Legacy tests are `./tests/run_host_tests.sh`, `./tests/run_sanitizers.sh`, `python3 tests/validate_metadata.py` and `python3 tests/test_tessera_fold.py`.

### Critical limitations

This is a source and host-test alpha package. No ColdFire `.elemod` or modified firmware `.syx` is included. The real elekloader checkout/build, linker, upstream host runner, emulator and physical Digitone have not been validated here. Host tests do not establish real-time budget or hardware compatibility. Read [VALIDATION.md](docs/VALIDATION.md) before any porting attempt.

The Workbench locally hashes a user-selected file in the browser and exports a JSON plan; it **does not modify** firmware or send files to a server.

### Community

French and English contributions are welcome: [Discussions](https://github.com/Jickssa/DN1-LAB-0.2.1-alpha/discussions), [Issues](https://github.com/Jickssa/DN1-LAB-0.2.1-alpha/issues), [Pull Requests](https://github.com/Jickssa/DN1-LAB-0.2.1-alpha/pulls). GitHub Pages must be configured to use “GitHub Actions” under Settings → Pages before the site URL becomes live; Discussions must also be enabled in repository settings.
