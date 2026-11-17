# DN1-LAB — Experimental sound machines for Elektron Digitone MKI

[Français](#français) · [English](#english)

> Independent community research. DN1-LAB is not affiliated with Elektron.

## Français

DN1-LAB est un laboratoire logiciel indépendant consacré à la recherche de machines de synthèse et d'outils pour Elektron Digitone MKI. Le dépôt organise le code, la documentation, les propositions communautaires et les validations reproductibles.

**État de cette branche :** fondation documentaire et architecture publique. Les sources complètes KARST, MORPHANT et TESSERA, ainsi que le générateur firmware, restent à importer et à tester dans ce dépôt. Cette branche ne contient aucun binaire firmware modifié et ne constitue pas une version installable.

Cible de recherche : DN1-LAB 0.2.x-alpha, Digitone MKI OS 1.44. Toute compatibilité doit être confirmée par une compilation SDK et des essais adaptés ; un test hôte ne prouve pas le fonctionnement matériel.

### Machines de recherche

| ID | Machine | Concept | État ici |
|---:|---|---|---|
| 100 | KARST — Modal Matter | Résonateurs modaux, matériaux et excitations | Fiche conceptuelle ; sources à importer |
| 101 | MORPHANT — Formant Organism | Synthèse additive, morphing de formants, textures | Fiche conceptuelle ; sources à importer |
| 102 | TESSERA — Phase Lattice | Accumulateurs de phase couplés, feedback, fold | Fiche conceptuelle ; sources à importer |

Les IDs sont à vérifier dans chaque build combiné : ils ne sont pas réservés universellement.

### Objectifs

- concevoir des moteurs sonores originaux dans les contraintes du Digitone MKI ;
- établir des contrats DSP et SDK indépendants ;
- fournir rendu hôte, tests déterministes, analyse numérique et rapports de ressources ;
- maintenir un catalogue communautaire bilingue ;
- construire à terme un pipeline local, vérifiable et reproductible de préparation firmware ;
- séparer clairement concepts, code testé sur hôte, builds SDK, émulation et essais matériels.

### Architecture visée

DSP isolé par machine → manifeste versionné → adaptateur SDK → hôte de test → validation → résolution de dépendances → assemblage firmware → vérification d'intégrité et rapport de build.

### Documentation et participation

- [Documentation](docs/README.md)
- [Architecture](docs/ARCHITECTURE.md)
- [Catalogue et concepts de machines](docs/MACHINES.md)
- [Pipeline firmware](docs/FIRMWARE-PIPELINE.md)
- [Workbench](docs/WORKBENCH.md)
- [Politique de validation](docs/VALIDATION.md)
- [Communauté](docs/COMMUNITY.md)
- [Contribuer](CONTRIBUTING.md)
- [Feuille de route](ROADMAP.md)
- [Sécurité](SECURITY.md)

Les contributions FR et EN sont ouvertes. Utiliser les Discussions pour les idées et les Issues pour les tâches ou défauts reproductibles.

## English

DN1-LAB is an independent software research lab for experimental synthesis machines and tooling for the Elektron Digitone MKI. This repository organizes source code, documentation, community proposals and reproducible validation.

**Status of this branch:** public documentation and architecture foundation. The full KARST, MORPHANT and TESSERA source trees, as well as a firmware generator, still need to be imported and tested in this repository. This branch contains no modified firmware binary and is not an installable release.

Research target: DN1-LAB 0.2.x-alpha, Digitone MKI OS 1.44. Compatibility requires a target SDK build and appropriate testing; host tests alone do not prove hardware operation.

### Research machines

| ID | Machine | Concept | Status here |
|---:|---|---|---|
| 100 | KARST — Modal Matter | Modal resonators, materials and excitations | Concept entry; source to be imported |
| 101 | MORPHANT — Formant Organism | Additive synthesis, formant morphing, textures | Concept entry; source to be imported |
| 102 | TESSERA — Phase Lattice | Coupled phase accumulators, feedback, fold | Concept entry; source to be imported |

IDs must be checked in every combined build; they are not universally reserved.

### Goals

- design original sound engines within Digitone MKI constraints;
- establish independent DSP and SDK contracts;
- provide host rendering, deterministic tests, numerical analysis and resource reports;
- maintain a bilingual community catalogue;
- eventually build a local, verifiable and reproducible firmware-preparation pipeline;
- clearly distinguish concepts, host-tested code, SDK builds, emulation and hardware tests.

### Intended architecture

Isolated per-machine DSP → versioned manifest → SDK adapter → test host → validation → dependency resolution → firmware assembly → integrity verification and build report.

### Documentation and participation

See the [documentation](docs/README.md), [architecture](docs/ARCHITECTURE.md), [machine concepts](docs/MACHINES.md), [firmware pipeline](docs/FIRMWARE-PIPELINE.md), [Workbench](docs/WORKBENCH.md), [validation policy](docs/VALIDATION.md), [community guide](docs/COMMUNITY.md), [contribution guide](CONTRIBUTING.md), [roadmap](ROADMAP.md) and [security policy](SECURITY.md).

Contributions in French and English are welcome. Use Discussions for ideas and Issues for tasks or reproducible defects.
