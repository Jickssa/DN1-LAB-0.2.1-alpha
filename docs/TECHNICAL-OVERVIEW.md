# DN1-LAB technical overview / Présentation technique

## Français

Version source : 0.2.2-alpha. Cible documentaire : Digitone MKI / Digitone Keys OS 1.44. Niveau de preuve : L2 — tests hôte, pas de certification matérielle.

DN1-LAB rassemble trois moteurs DSP natifs. Ils remplacent le générateur FM de la piste concernée, puis réutilisent le trajet natif du Digitone : enveloppe AMP, filtre, mixeur, chorus, delay, reverb et sortie. Ce sont des synthèses algorithmiques, pas des lecteurs de samples.

### KARST — Modal Matter (ID 100)
Trois résonateurs modaux du second ordre, rapports partiels inharmoniques et huit familles de matière : bois, métal, verre, membrane, cloche, pierre, tube et cristal. L'excitation combine frappe initiale, bruit et frottement ; chaque mode a son amortissement et sa pondération.

### MORPHANT — Formant Organism (ID 101)
Synthèse additive à six partiels et cinq profils spectraux inspirés des voyelles A/E/I/O/U. Morphing continu, vibrato, sous-octave et trois textures distinctes : BRETH (bruit lent filtré), NOIS (large bande), AIR (hautes fréquences). Les profils sont des références acoustiques, non une simulation anatomique. Les mémoires de filtres sont réinitialisées au retrigger.

### TESSERA — Phase Lattice (ID 102)
Deux accumulateurs de phase couplés ; modulation de phase à fréquence audio, feedback borné, skew, synchronisation, mélange en anneau, repliement asymétrique, sous-octave et textures numériques. La stabilité sur cible et le budget temps réel doivent encore être mesurés.

### Branches de code
- modules/ : API historique dnm_machine, 0.1.1, 16 contrôles basés sur SYN1/SYN2.
- sdk-alpha/ : port séparé elek_machine, ELEK_REGISTER_MACHINE, 15 contrôles logiques et deux pages.
- host/ : adaptateur de test local, pas l'API upstream cible.
- tests/ et tools/ : tests de régression, analyse numérique, validation manifestes et vérification du firmware stock.
- catalogue/ : métadonnées machine et propositions de recherche.
- site/ : site statique FR/EN et Workbench de préparation.

Le shared machine SDK alpha visé limite chaque machine à 15 knobs, deux pages de huit positions, un état par voix de 256 octets et un callback de 32 échantillons Q31 à 48 kHz. Les contrats doivent être confirmés contre la version exacte du SDK et du compagnon utilisée lors du build.

### Build cible et limites
Le script scripts/build_digitone_alpha.sh attend un checkout elekloader contenant le SDK alpha et les ports Digitone OS 1.44 du core/compagnon, une toolchain ColdFire/M68K et le fichier stock OS 1.44. Ces prérequis n'étaient pas disponibles dans l'environnement ayant produit ce dépôt ; le build cible n'a donc pas été exécuté.

Aucun .elemod ColdFire, firmware modifié .syx, bootloader modifié, émulateur ni installateur n'est livré. Le Workbench calcule localement une empreinte et exporte un plan JSON ; il ne modifie pas le firmware et n'envoie aucun fichier à un serveur. Ne pas flasher les sources alpha sans compilation cible, intégration, émulation, sauvegarde et vérification matérielle.

## English

Source version: 0.2.2-alpha. Documented target: Digitone MKI / Digitone Keys OS 1.44. Evidence level: L2 — host-tested, not hardware-certified.

DN1-LAB contains three native DSP engines. They replace the FM generator for the selected track voice and then use the Digitone's native AMP envelope, filter, mixer, chorus, delay, reverb and output chain. These are algorithmic synthesis engines, not sample players.

### KARST — Modal Matter (ID 100)
Three second-order modal resonators, inharmonic partial ratios and eight material families: wood, metal, glass, membrane, bell, stone, tube and crystal. Excitation combines an onset strike, noise and friction; each mode has independent damping and weighting.

### MORPHANT — Formant Organism (ID 101)
Six-partial additive synthesis with five vowel-inspired spectral profiles (A/E/I/O/U). Continuous morphing, vibrato, sub-octave and three distinct textures: BRETH (slow filtered noise), NOIS (broadband), AIR (high frequencies). These are acoustic references, not an anatomical vocal model. Filter memories reset on retrigger.

### TESSERA — Phase Lattice (ID 102)
Two coupled phase accumulators; audio-rate phase modulation, bounded feedback, skew, synchronization, ring blend, asymmetric wavefolding, sub-octave and digital textures. Target stability and real-time budget still require measurement.

### Source branches
- modules/: historical dnm_machine API, 0.1.1, 16 SYN1/SYN2 controls.
- sdk-alpha/: separate elek_machine port, ELEK_REGISTER_MACHINE, 15 logical controls and two pages.
- host/: local test adapter, not a replacement for the target upstream API.
- tests/ and tools/: regression, numerical, manifest and stock-firmware checks.
- catalogue/: structured machine metadata and research proposals.
- site/: French/English static site and preparation Workbench.

The target shared machine SDK alpha documents up to 15 knobs, two pages of eight positions, 256 bytes of per-voice state and a 32-sample Q31 callback at 48 kHz. Check these contracts against the exact SDK and companion versions used for a target build.

The build script requires an elekloader checkout with the shared SDK alpha and Digitone OS 1.44 core/companion ports, a ColdFire/M68K toolchain and the stock OS 1.44 image. Those prerequisites were unavailable here, so no target build was executed.

No ColdFire .elemod, modified .syx, modified bootloader, emulator or installer is provided. The Workbench hashes a local file and exports a JSON plan; it does not modify firmware or upload files. Do not flash alpha sources without target compilation, integration, emulation, backup and hardware validation.
