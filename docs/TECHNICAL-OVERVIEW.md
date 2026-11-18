# DN1-LAB 0.3.0-alpha — technical overview / Présentation technique

## Français

DN1-LAB est un laboratoire de moteurs DSP et de validation pour Digitone MKI / Digitone Keys. La cible documentaire est OS 1.44. Le dépôt comprend deux ensembles logiciels qui doivent rester distincts.

### A. Trois moteurs shared machine SDK alpha (L2, tests hôte)

Les sources `sdk-alpha/` déclarent des machines avec `struct elek_machine` et `ELEK_REGISTER_MACHINE` :

- **KARST (ID 100)** : trois résonateurs modaux du second ordre, partiels inharmoniques et excitation par frappe/bruit/frottement.
- **MORPHANT (ID 101)** : six partiels additifs, profils spectraux inspirés des voyelles, morphing, vibrato et textures BRETH/NOIS/AIR.
- **TESSERA (ID 102)** : deux accumulateurs de phase couplés, modulation à fréquence audio, feedback borné, skew, sync, ring blend et fold.

Le port vise quinze paramètres logiques maximum, deux pages de contrôle, un état par voix ≤256 octets et un callback de 32 échantillons à 48 kHz selon le SDK partagé consulté. Les valeurs `v->knob[]` sont déjà dans les plages logiques des descripteurs, pas des mots SYN bruts 8.8.

### B. Six prototypes DSP hôte (L2, tests automatisés, non intégrés au SDK alpha)

Les sources `prototypes/` sont des expériences hôte, compilées contre un sous-ensemble local de l'interface pour tester les algorithmes et les invariants ; cet en-tête local n'est pas le véritable SDK upstream.

- **VESPER (103)** : stencil d'excitation 32 cellules, lecture inverse/vitesse variable et deux résonateurs amortis.
- **MYCEL (104)** : quatre nœuds résonants, six arêtes fixes, transferts d'énergie et pertes.
- **ORBITAL (105)** : champ harmonique 2D à mémoire de chemin et hystérésis.
- **LITHOS (106)** : six partiels alimentés par trois réservoirs de matière spectrale.
- **PHOTON (107)** : événements clairsemés injectés dans un réseau de phase à quatre nœuds.
- **ANIMA (108)** : quatre agents virtuels partageant activité et mémoire dans une seule voix ; aucune interaction inter-voix.

Les IDs 103–108 sont des candidats. Les états hôte mesurés font 40 à 60 octets, mais cette mesure ne garantit ni layout ni ABI cible.

### Architecture du dépôt

| Répertoire | Rôle |
|---|---|
| `sdk-alpha/` | Trois portages vers le shared machine SDK alpha |
| `prototypes/` | Six expériences DSP hôte, hors chemin firmware |
| `host/` | Sous-ensemble d'API pour les tests hôte des prototypes |
| `tests/` | Bancs de test, sanitizers, analyse statique, reproductibilité |
| `tools/` | Validateurs catalogue et site |
| `catalogue/` | Manifestes des trois machines et fiches des concepts/propositions |
| `site/` | Page statique bilingue et Workbench de préparation |
| `scripts/` | Orchestration de build cible des trois machines SDK alpha |

### Pipeline cible envisagé

`manifest -> preflight -> dependency resolution -> ColdFire build -> lint/link combined -> patch stock image -> structural/hash verification -> emulator -> hardware`.

Toute étape échouée bloque la progression. Les versions, commits upstream, empreintes, journaux et résultats doivent être conservés.

Le script `scripts/build_digitone_alpha.sh` concerne uniquement les trois sources SDK alpha dans `sdk-alpha/`. Il attend le checkout upstream elekloader compatible, les compagnons OS 1.44, la toolchain ColdFire et l'entrée firmware stock exacte. Le build cible n'a pas encore été exécuté.

### Limites actuelles

- Aucun `.elemod` ColdFire n'est fourni.
- Aucun firmware modifié `.syx` n'est fourni.
- Les IDs 103–108 n'ont pas été validés par le linker combiné.
- Aucun test DigiKit/digiemu ni test matériel n'est revendiqué.
- Le budget cycles/bloc ColdFire et le mixage polyphonique simultané restent inconnus.
- Le Workbench calcule une empreinte locale et exporte un plan JSON ; il ne parse pas intégralement ELE3, ne compile pas, ne patch pas et ne flashe pas.

## English

DN1-LAB is a DSP-engine and validation research lab for Digitone MKI / Digitone Keys. The documented target is OS 1.44. The repository contains two software workstreams which must remain separate.

### A. Three shared machine SDK-alpha engines (L2, host-tested)

The `sdk-alpha/` sources declare machines through `struct elek_machine` and `ELEK_REGISTER_MACHINE`:

- **KARST (ID 100):** three second-order modal resonators, inharmonic partials and strike/noise/friction excitation.
- **MORPHANT (ID 101):** six additive partials, vowel-inspired spectral profiles, morphing, vibrato and BRETH/NOIS/AIR textures.
- **TESSERA (ID 102):** two coupled phase accumulators, audio-rate modulation, bounded feedback, skew, sync, ring blend and fold.

The port targets up to fifteen logical parameters, two custom pages, up to 256 bytes of per-voice state and a 32-sample callback at 48 kHz under the consulted shared SDK contract. `v->knob[]` values are already mapped into descriptor ranges, not raw SYN 8.8 words.

### B. Six host DSP prototypes (L2 host-tested, not integrated into SDK alpha)

The `prototypes/` sources are host experiments built against a local subset of the interface to test algorithms and invariants; this local header is not the upstream SDK.

- **VESPER (103):** 32-cell excitation stencil, reverse/variable-speed readout and two damped resonators.
- **MYCEL (104):** four resonant nodes, six fixed edges, energy transfer and losses.
- **ORBITAL (105):** 2D harmonic field with path memory and hysteresis.
- **LITHOS (106):** six partials fed by three spectral material reservoirs.
- **PHOTON (107):** sparse events injected into a four-node phase network.
- **ANIMA (108):** four virtual agents sharing activity and memory inside one voice; no cross-voice interaction.

IDs 103–108 are candidates. Measured host states are 40–60 bytes, but that does not guarantee target layout or ABI.

### Repository layout

| Directory | Role |
|---|---|
| `sdk-alpha/` | Three ports to the shared machine SDK alpha |
| `prototypes/` | Six host DSP experiments, outside the firmware path |
| `host/` | Local API subset for prototype host tests |
| `tests/` | Test harnesses, sanitizers, static analysis and reproducibility |
| `tools/` | Catalogue and site validators |
| `catalogue/` | Three machine manifests and research proposals |
| `site/` | Bilingual static page and preparation Workbench |
| `scripts/` | Target-build orchestration for the three SDK-alpha machines |

### Intended target pipeline

`manifest -> preflight -> dependency resolution -> ColdFire build -> combined lint/link -> patch stock image -> structural/hash verification -> emulator -> hardware`.

Any failed step blocks further progress. Preserve versions, upstream commits, digests, logs and results.

The `scripts/build_digitone_alpha.sh` script only targets the three SDK-alpha sources in `sdk-alpha/`. It requires a compatible elekloader checkout, OS 1.44 companions, a ColdFire toolchain and the exact stock firmware input. The target build has not yet been executed.

### Current limitations

No ColdFire `.elemod` or modified `.syx` is supplied; candidate IDs 103–108 have not been checked by the combined linker; no DigiKit/digiemu or hardware test is claimed; ColdFire cycles per block and simultaneous polyphonic mixing remain unknown. The Workbench hashes a local file and exports a JSON plan only; it does not fully parse ELE3, compile, patch or flash.
