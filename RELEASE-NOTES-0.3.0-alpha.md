# DN1-LAB 0.3.0-alpha — six host DSP prototypes

## Français

DN1-LAB 0.3.0-alpha ajoute six prototypes DSP hôte au laboratoire de synthèse indépendant pour Elektron Digitone MKI / Digitone Keys.

### Nouveaux prototypes
- VESPER — mémoire courte d'excitation, lecture réversible et deux résonateurs amortis.
- MYCEL — quatre nœuds résonants et six liaisons de transfert énergétique.
- ORBITAL — champ harmonique 2D avec mémoire de trajectoire et hystérésis.
- LITHOS — érosion, dépôt et fracture entre réservoirs spectraux.
- PHOTON — impulsions clairsemées et réseau de réflexion de phase.
- ANIMA — quatre agents virtuels dans une seule voix ; aucun bus inter-voix.

Chaque prototype dispose de quinze contrôles, deux pages et de 40 à 60 octets d'état hôte. Les IDs 103–108 sont provisoires et non confirmés par le linker combiné.

### Validation
- GCC 14.2 et Clang 17 : tests hôte réussis.
- ASan/UBSan sous GCC et Clang : succès.
- GCC -fanalyzer et Clang --analyze : succès.
- Six WAV de démonstration reproductibles octet par octet entre compilateurs.
- GitHub Actions : six jobs sur six réussis pour le commit 329aa1429d8854a6f805e1ef495bd7557cf00794.

### Limites
Cette version ne contient aucun `.elemod` ColdFire ni aucun firmware modifié `.syx`. Les prototypes ne sont pas intégrés dans le builder firmware. Aucune compilation ColdFire, liaison combinée, émulation ou validation matérielle n'est revendiquée. Les tests hôte ne certifient pas le temps réel sur Digitone.

Ne flashe pas ces sources. Voir [les limites et rapports de validation](docs/PROTOTYPE-VALIDATION.md).

## English

DN1-LAB 0.3.0-alpha adds six host DSP prototypes to the independent synthesis research lab for Elektron Digitone MKI / Digitone Keys.

### New prototypes
- VESPER — short excitation memory, reversible readout and two damped resonators.
- MYCEL — four resonant nodes and six energy-transfer links.
- ORBITAL — 2D harmonic field with path memory and hysteresis.
- LITHOS — erosion, deposition and fracture across spectral reservoirs.
- PHOTON — sparse impulses and a phase-reflection network.
- ANIMA — four virtual agents inside one voice; no cross-voice bus.

Each prototype exposes fifteen controls, two pages and 40–60 bytes of host state. IDs 103–108 are provisional and have not been confirmed by the combined linker.

### Validation
- GCC 14.2 and Clang 17 host tests pass.
- ASan/UBSan under GCC and Clang pass.
- GCC -fanalyzer and Clang --analyze pass.
- Six preview WAVs are byte-identical across compilers.
- GitHub Actions: six of six jobs passed for commit 329aa1429d8854a6f805e1ef495bd7557cf00794.

### Limitations
This release includes no ColdFire `.elemod` and no modified `.syx` firmware. The prototypes are not included in the firmware builder. No ColdFire build, combined linkage, emulator or hardware validation is claimed. Host tests do not certify real-time performance on Digitone.

Do not flash these sources. See the [prototype validation report](docs/PROTOTYPE-VALIDATION.md).
