# DN1-LAB — Experimental synthesis machines for Digitone MKI

[Français](#français) · [English](#english) · [Workbench](https://jickssa.github.io/DN1-LAB-0.2.1-alpha/workbench/)

> Independent community research. DN1-LAB is not affiliated with Elektron. Alpha sources are not certified as installable.

## Français

DN1-LAB est un laboratoire indépendant de moteurs de synthèse et d'outils de recherche pour Elektron Digitone MKI. Cette branche publique contient les trois moteurs du port shared machine SDK alpha 0.2.2, leurs manifestes, un catalogue communautaire, une documentation bilingue, un Workbench statique et des validateurs de métadonnées.

**Cible documentaire :** Digitone MKI / Digitone Keys OS 1.44. **Niveau de preuve des moteurs : L2 — tests hôte vérifiés dans l'environnement de préparation.** Cela ne signifie pas que les modules sont compilés pour le processeur du Digitone ou prêts à installer.

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

Les sources SDK alpha sont directement consultables dans `sdk-alpha/`. Le paquet complet DN1-LAB 0.2.2-alpha est conservé comme artefact local de travail, mais son archive et ses anciens modules API 0.1.1 / harnais de test ne sont pas encore publiés dans ce dépôt. Les résultats des tests exécutés sont consignés dans [VALIDATION.md](docs/VALIDATION.md). Le [script de build OS 1.44](scripts/build_digitone_alpha.sh) est fourni, mais son exécution avec la toolchain ColdFire et les compagnons upstream reste à valider.

### Contrôles disponibles dans le dépôt

```sh
python3 tools/validate_catalogue.py
python3 tools/validate_site.py
python3 -m compileall -q tools
```

Ces contrôles valident la cohérence des manifestes catalogue/SDK/IDs C et les liens locaux du site. Ils ne compilent pas les moteurs contre le SDK cible. La CI GitHub actuelle exécute ces contrôles sur chaque PR.

### Limites essentielles

Aucun module ColdFire `.elemod` ni firmware modifié `.syx` n'est fourni. Le vrai build upstream, le linker combiné, l'émulateur et le Digitone physique n'ont pas été validés. Les tests hôte ne prouvent ni budget temps réel ni compatibilité matérielle. Le Workbench vérifie localement l'empreinte d'un fichier sélectionné et exporte un plan JSON ; il ne parse pas complètement ELE3, ne compile pas, ne patche pas et ne flashe.

### Communauté

Contributions en français et en anglais : [Discussions](https://github.com/Jickssa/DN1-LAB-0.2.1-alpha/discussions), [Issues](https://github.com/Jickssa/DN1-LAB-0.2.1-alpha/issues), [Pull Requests](https://github.com/Jickssa/DN1-LAB-0.2.1-alpha/pulls). Un administrateur doit activer Discussions et configurer GitHub Pages dans les paramètres du dépôt.

---

## English

DN1-LAB is an independent research lab for experimental synthesis engines and tooling for the Elektron Digitone MKI. This public branch contains the three shared machine SDK-alpha 0.2.2 engines, their manifests, a community catalogue, bilingual documentation, a static Workbench and metadata validators.

**Documented target:** Digitone MKI / Digitone Keys OS 1.44. **Engine evidence level: L2 — host tests verified in the preparation environment.** This does not mean the modules have been compiled for the Digitone processor or are ready to install.

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

The SDK-alpha sources are directly browsable in `sdk-alpha/`. The complete DN1-LAB 0.2.2-alpha source and test package is available separately as a working archive; the legacy 0.1.1 API modules and test harness are not yet imported file-by-file into this repository. Executed test results are documented in [VALIDATION.md](docs/VALIDATION.md).

### Checks available in this repository

```sh
python3 tools/validate_catalogue.py
python3 tools/validate_site.py
python3 -m compileall -q tools
```

These checks validate catalogue/SDK/C-ID consistency and local static-site links. They do not compile the engines against the target SDK. Current GitHub CI runs these checks on pull requests.

### Critical limitations

No ColdFire `.elemod` or modified firmware `.syx` is supplied. The actual upstream build, combined linker, emulator and physical Digitone have not been validated. Host tests do not prove a real-time budget or hardware compatibility. The Workbench hashes a user-selected file locally and exports a JSON plan; it does not fully parse ELE3, compile, patch or flash firmware.

### Community

French and English contributions are welcome: [Discussions](https://github.com/Jickssa/DN1-LAB-0.2.1-alpha/discussions), [Issues](https://github.com/Jickssa/DN1-LAB-0.2.1-alpha/issues), [Pull Requests](https://github.com/Jickssa/DN1-LAB-0.2.1-alpha/pulls). An administrator must enable Discussions and configure GitHub Pages in repository settings.
