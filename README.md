# DN1-LAB — Experimental synthesis machines for Digitone MKI

[Français](#français) · [English](#english) · [Workbench](https://jickssa.github.io/DN1-LAB-0.2.1-alpha/workbench/)

> Independent community research. DN1-LAB is not affiliated with Elektron. Alpha sources are not certified as installable.

## Français

DN1-LAB est un laboratoire indépendant de moteurs de synthèse et d'outils de recherche pour Elektron Digitone MKI. Cette intégration contient les trois moteurs du port shared SDK alpha 0.2.2, leurs manifestes, une base historique, des tests hôte reproductibles, un catalogue communautaire, un Workbench statique et des outils de validation.

**Cible documentaire :** Digitone MKI / Digitone Keys OS 1.44. **Niveau de preuve : L2 — tests hôte.** Cela ne signifie pas que les modules sont compilés pour le processeur du Digitone ou prêts à être installés.

### Accès rapide

- [Workbench](https://jickssa.github.io/DN1-LAB-0.2.1-alpha/workbench/) — sélection, hash local et export d'un plan JSON.
- [Présentation technique](docs/TECHNICAL-OVERVIEW.md)
- [Guide des instruments FR](docs/INSTRUMENTS.md) · [Instrument guide EN](docs/INSTRUMENTS.en.md)
- [Validation et limites](docs/VALIDATION.md)
- [Portage SDK alpha](docs/ALPHA-PORT.md)
- [Recherche et antériorité](docs/RESEARCH.md)
- [Contribuer](CONTRIBUTING.md)

### Moteurs inclus

| Machine | ID | Modèle |
|---|---:|---|
| KARST — Modal Matter | 100 | Trois résonateurs modaux inharmoniques |
| MORPHANT — Formant Organism | 101 | Six partiels additifs, morphing formantique, trois textures |
| TESSERA — Phase Lattice | 102 | Deux accumulateurs de phase couplés, feedback et fold |

Le port SDK alpha est consultable dans `sdk-alpha/`. La branche historique API 0.1.1, les headers de l'hôte de test, les scripts de build et les suites de tests hôte sont conservés dans l'archive source versionnée `source-bundles/dn1-lab-0.2.2-alpha-host-source.tar.gz`. Elle est vérifiée par SHA-256 et extraite localement avant exécution.

### Installer les sources de test et lancer les vérifications

Prérequis : Python 3, GCC ou Clang, GNU tar et shell POSIX. Depuis la racine du dépôt :

```sh
./scripts/unpack_source_bundle.sh
python3 tools/validate_catalogue.py
python3 tools/validate_site.py
python3 tests/validate_alpha_manifests.py
CC=gcc ./tests/run_alpha_tests.sh
CC=gcc ./tests/run_alpha_production_like.sh
CC=gcc ./tests/run_alpha_sanitizers.sh
CC=gcc ./tests/run_alpha_analyzer.sh
CC=gcc ./tests/run_host_tests.sh
CC=gcc ./tests/run_sanitizers.sh
python3 tests/validate_metadata.py
python3 tests/test_tessera_fold.py
```

Clang peut être substitué à GCC pour les tests alpha. La GitHub Actions CI exécute ces contrôles sur GCC et Clang à partir de l'archive vérifiée. Le vérificateur du firmware stock nécessite le fichier OS 1.44 original fourni localement ; ce fichier propriétaire n'est pas inclus.

### Limites essentielles

Aucun module ColdFire `.elemod` ni firmware modifié `.syx` n'est fourni. Le vrai build upstream, le linker combiné, le runner hôte officiel, l'émulateur et le Digitone physique n'ont pas été validés. Les tests hôte ne prouvent ni budget temps réel ni compatibilité matérielle. Le Workbench vérifie localement l'empreinte d'un fichier sélectionné et exporte un plan JSON ; il ne parse pas complètement ELE3, ne compile pas, ne patche pas et ne flashe.

### Communauté

Contributions en français et en anglais : [Discussions](https://github.com/Jickssa/DN1-LAB-0.2.1-alpha/discussions), [Issues](https://github.com/Jickssa/DN1-LAB-0.2.1-alpha/issues), [Pull Requests](https://github.com/Jickssa/DN1-LAB-0.2.1-alpha/pulls). Un administrateur doit activer Discussions et configurer GitHub Pages dans les paramètres du dépôt.

---

## English

DN1-LAB is an independent research lab for experimental synthesis engines and tooling for the Elektron Digitone MKI. This integration contains the three shared machine SDK-alpha 0.2.2 engines, their manifests, the historical API branch, reproducible host tests, a community catalogue, a static Workbench and validation tools.

**Documented target:** Digitone MKI / Digitone Keys OS 1.44. **Evidence level: L2 — host-tested.** This does not mean the modules have been compiled for the Digitone processor or are ready to install.

### Quick links

- [Workbench](https://jickssa.github.io/DN1-LAB-0.2.1-alpha/workbench/) — selection, local hashing and JSON-plan export.
- [Technical overview](docs/TECHNICAL-OVERVIEW.md)
- [Instrument guide EN](docs/INSTRUMENTS.en.md) · [Guide FR](docs/INSTRUMENTS.md)
- [Validation and limitations](docs/VALIDATION.md)
- [SDK-alpha port](docs/ALPHA-PORT.md)
- [Research and prior art](docs/RESEARCH.md)
- [Contributing](CONTRIBUTING.md)

### Included engines

| Machine | ID | Model |
|---|---:|---|
| KARST — Modal Matter | 100 | Three inharmonic modal resonators |
| MORPHANT — Formant Organism | 101 | Six additive partials, formant morphing, three textures |
| TESSERA — Phase Lattice | 102 | Two coupled phase accumulators, feedback and wavefolding |

The SDK-alpha port is directly browsable in `sdk-alpha/`. The historical 0.1.1 API branch, test-host headers, build scripts and host test suites are retained in the versioned source archive `source-bundles/dn1-lab-0.2.2-alpha-host-source.tar.gz`. Its SHA-256 is checked before local extraction.

### Extract test sources and run checks

Requirements: Python 3, GCC or Clang, GNU tar and a POSIX shell. From the repository root:

```sh
./scripts/unpack_source_bundle.sh
python3 tools/validate_catalogue.py
python3 tools/validate_site.py
python3 tests/validate_alpha_manifests.py
CC=gcc ./tests/run_alpha_tests.sh
CC=gcc ./tests/run_alpha_production_like.sh
CC=gcc ./tests/run_alpha_sanitizers.sh
CC=gcc ./tests/run_alpha_analyzer.sh
CC=gcc ./tests/run_host_tests.sh
CC=gcc ./tests/run_sanitizers.sh
python3 tests/validate_metadata.py
python3 tests/test_tessera_fold.py
```

Use Clang instead of GCC for the alpha tests as well. GitHub Actions runs the verified archive checks on both GCC and Clang. The stock-firmware verifier requires the original OS 1.44 file to be supplied locally; proprietary firmware is not included.

### Critical limitations

No ColdFire `.elemod` or modified firmware `.syx` is supplied. The actual upstream build, combined linker, official host runner, emulator and physical Digitone have not been validated. Host tests do not prove real-time budget or hardware compatibility. The Workbench hashes a user-selected file locally and exports a JSON plan; it does not fully parse ELE3, compile, patch or flash firmware.

### Community

French and English contributions are welcome: [Discussions](https://github.com/Jickssa/DN1-LAB-0.2.1-alpha/discussions), [Issues](https://github.com/Jickssa/DN1-LAB-0.2.1-alpha/issues), [Pull Requests](https://github.com/Jickssa/DN1-LAB-0.2.1-alpha/pulls). An administrator must enable Discussions and configure GitHub Pages in repository settings.
