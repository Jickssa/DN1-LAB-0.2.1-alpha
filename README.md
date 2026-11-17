# DN1-LAB — Experimental synthesis machines for Digitone MKI

[Français](#français) · [English](#english) · [Workbench](https://jickssa.github.io/DN1-LAB-0.2.1-alpha/workbench/)

> Independent community research. DN1-LAB is not affiliated with Elektron. Alpha sources are not certified as installable.

## Français

DN1-LAB est un laboratoire indépendant de moteurs de synthèse et d'outils de recherche pour Elektron Digitone MKI. Cette intégration GitHub contient les trois sources de machines du port shared SDK alpha 0.2.2 et leurs manifestes, un catalogue communautaire, la documentation, un Workbench statique et des outils de contrôle de métadonnées.

**Cible documentaire :** Digitone MKI / Digitone Keys OS 1.44. **Niveau de preuve revendiqué ici : L2 (tests hôte effectués dans l'environnement de préparation).**

### Accès rapide

- [Workbench](https://jickssa.github.io/DN1-LAB-0.2.1-alpha/workbench/) — sélection et plan JSON local.
- [Présentation technique](docs/TECHNICAL-OVERVIEW.md)
- [Guide des instruments (FR)](docs/INSTRUMENTS.md) · [Instrument guide (EN)](docs/INSTRUMENTS.en.md)
- [Validation et limites](docs/VALIDATION.md)
- [Portage SDK alpha](docs/ALPHA-PORT.md)
- [Recherche et antériorité](docs/RESEARCH.md)
- [Contribuer](CONTRIBUTING.md)

### Moteurs inclus

| Machine | ID | Modèle | Statut |
|---|---:|---|---|
| KARST — Modal Matter | 100 | Trois résonateurs modaux inharmoniques | Source SDK alpha, tests hôte L2 |
| MORPHANT — Formant Organism | 101 | Six partiels additifs, morphing formantique, trois textures | Source SDK alpha, tests hôte L2 |
| TESSERA — Phase Lattice | 102 | Deux accumulateurs de phase couplés, feedback et fold | Source SDK alpha, tests hôte L2 |

Le port SDK alpha est dans sdk-alpha/. La branche historique modules/ et le harnais tests/ de l'archive source 0.2.2-alpha ne sont pas encore présents dans cette intégration GitHub ; les résultats locaux de la suite complète sont décrits dans le rapport de validation. La CI GitHub ne prétend donc pas recompiler ni rejouer cette suite absente.

### Contrôles réellement disponibles dans ce dépôt

    python3 tools/validate_catalogue.py
    python3 tools/validate_site.py
    python3 -m compileall -q tools

Ces contrôles valident la cohérence des manifestes catalogue/SDK/IDs C et les liens locaux des pages statiques. Ils ne compilent pas les moteurs contre le SDK cible.

### Limites essentielles

Aucun module ColdFire .elemod ni firmware modifié .syx n'est fourni. Le vrai build upstream, le linker combiné, le runner hôte officiel, l'émulateur et le Digitone physique n'ont pas été validés. Les tests hôte ne prouvent ni budget temps réel ni compatibilité matérielle. Le Workbench vérifie localement l'empreinte d'un fichier sélectionné et exporte un plan JSON ; il ne parse pas complètement ELE3, ne compile pas, ne patche pas et ne flashe.

### Communauté

Contributions en français et en anglais : [Discussions](https://github.com/Jickssa/DN1-LAB-0.2.1-alpha/discussions), [Issues](https://github.com/Jickssa/DN1-LAB-0.2.1-alpha/issues), [Pull Requests](https://github.com/Jickssa/DN1-LAB-0.2.1-alpha/pulls). Discussions et GitHub Pages doivent être activés dans les paramètres du dépôt par un administrateur.

---

## English

DN1-LAB is an independent research lab for experimental synthesis engines and tooling for the Elektron Digitone MKI. This GitHub integration contains the three shared machine SDK-alpha 0.2.2 source engines and manifests, a community catalogue, documentation, a static Workbench and metadata validation tools.

**Documented target:** Digitone MKI / Digitone Keys OS 1.44. **Evidence claimed here: L2 (host tests performed in the preparation environment).**

### Quick links

- [Workbench](https://jickssa.github.io/DN1-LAB-0.2.1-alpha/workbench/) — module selection and local JSON plan.
- [Technical overview](docs/TECHNICAL-OVERVIEW.md)
- [Instrument guide (EN)](docs/INSTRUMENTS.en.md) · [Guide des instruments (FR)](docs/INSTRUMENTS.md)
- [Validation and limitations](docs/VALIDATION.md)
- [SDK-alpha port](docs/ALPHA-PORT.md)
- [Research and prior art](docs/RESEARCH.md)
- [Contributing](CONTRIBUTING.md)

### Included engines

| Machine | ID | Model | Status |
|---|---:|---|---|
| KARST — Modal Matter | 100 | Three inharmonic modal resonators | SDK-alpha source, L2 host tests |
| MORPHANT — Formant Organism | 101 | Six additive partials, formant morphing, three textures | SDK-alpha source, L2 host tests |
| TESSERA — Phase Lattice | 102 | Two coupled phase accumulators, feedback and fold | SDK-alpha source, L2 host tests |

The SDK-alpha port is in sdk-alpha/. The legacy modules/ branch and tests/ harness from the 0.2.2-alpha source archive are not yet present in this GitHub integration; full local test results are described in the validation report. GitHub CI therefore does not claim to compile against the target SDK or rerun the absent DSP suite.

### Checks actually available in this repository

    python3 tools/validate_catalogue.py
    python3 tools/validate_site.py
    python3 -m compileall -q tools

These checks validate catalogue/SDK/C-ID consistency and local static-site links. They do not compile the engines against the target SDK.

### Critical limitations

No ColdFire .elemod or modified firmware .syx is supplied. The actual upstream build, combined linker, official host runner, emulator and physical Digitone have not been validated. Host tests do not prove a real-time budget or hardware compatibility. The Workbench hashes a user-selected file locally and exports a JSON plan; it does not fully parse ELE3, compile, patch or flash firmware.

### Community

Contributions in French and English are welcome: [Discussions](https://github.com/Jickssa/DN1-LAB-0.2.1-alpha/discussions), [Issues](https://github.com/Jickssa/DN1-LAB-0.2.1-alpha/issues), [Pull Requests](https://github.com/Jickssa/DN1-LAB-0.2.1-alpha/pulls). An administrator must enable Discussions and configure GitHub Pages in repository settings.
