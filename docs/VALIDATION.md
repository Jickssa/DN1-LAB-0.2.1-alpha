# DN1-LAB 0.2.2-alpha — validation record / Rapport de validation

## Résumé / Summary

Cible documentaire : Digitone MKI / Digitone Keys OS 1.44, shared elekloader machine SDK alpha. **Niveau de preuve des moteurs : L2 (tests hôte).** Aucun build ColdFire, fichier .elemod, firmware modifié, test émulateur ni test sur Digitone physique n'est revendiqué.

## Matrice des tests exécutés dans l'environnement de préparation

| Vérification | GCC | Clang | Résultat |
|---|---:|---:|---|
| Manifestes alpha / IDs / cibles OS | PASS | PASS | IDs 100/101/102, OS 1.44, 15 paramètres |
| Harnais DSP alpha | PASS | PASS | Huit états indépendants, 256 blocs, ordre inversé et retrigger |
| Build hôte de production -Os | PASS | PASS | Rendus byte-identical aux références instrumentées |
| AddressSanitizer + UndefinedBehaviorSanitizer | PASS | PASS | Aucun défaut signalé |
| Analyse statique GCC -fanalyzer | PASS | — | Aucun défaut signalé |
| Tests hôte API historique 0.1.1 | PASS | — | Branche dnm_machine |
| Sanitizers historiques | PASS | — | Aucun défaut signalé |
| Validation plages/defaults metadata | PASS | — | Plages OS1.44 et valeurs par défaut |
| Équivalence du pliage TESSERA | PASS | — | 416 045 valeurs du domaine démontré |
| Vérificateur du firmware stock OS1.44 | PASS | — | Entrée locale fournie vérifiée |

Le harnais alpha a rendu 2 081 888 échantillons hôte sans clamp dur signalé par son compteur de test. Les huit lanes ont été testées séparément pour leur état : ce n'est pas un mixage natif de huit voix mesuré, ni une mesure de cycles ColdFire.

## Reproduction depuis un clone propre

Les sources de tests, l'ancien port 0.1.1 et les scripts sont fournis dans `source-bundles/dn1-lab-0.2.2-alpha-host-source.tar.gz`. Le bundle est vérifié par SHA-256 avant extraction :

    ./scripts/unpack_source_bundle.sh
    python3 tests/validate_alpha_manifests.py
    python3 tools/validate_catalogue.py
    python3 tools/validate_site.py
    CC=gcc ./tests/run_alpha_tests.sh
    CC=clang ./tests/run_alpha_tests.sh
    CC=gcc ./tests/run_alpha_production_like.sh
    CC=clang ./tests/run_alpha_production_like.sh
    CC=gcc ./tests/run_alpha_sanitizers.sh
    CC=clang ./tests/run_alpha_sanitizers.sh
    CC=gcc ./tests/run_alpha_analyzer.sh
    CC=gcc ./tests/run_host_tests.sh
    CC=gcc ./tests/run_sanitizers.sh
    python3 tests/validate_metadata.py
    python3 tests/test_tessera_fold.py

GitHub Actions exécute la suite hôte et les tests historiques avec GCC, et la suite alpha avec Clang. Le vérificateur du firmware stock nécessite le fichier OS 1.44 original fourni localement ; ce fichier propriétaire n'est pas inclus.

## Identité du firmware stock vérifié

- SHA-256 SysEx : `d4f200d04484333d82822db7744e6484d0def8f2db8ddf55ee2b780cc13c9659`
- Conteneur ELE3 : 1 320 656 octets ; SysEx device ID `0x0D` ; sept sections `[5, 2, 3, 4, 6, 7, 8]`.
- Section principale : 2 736 304 octets décompressés à `0x40000400`.
- SHA-256 de l'image principale : `fce648a97c6c5d93b961732e8f8db6b02e0820c6d344c7e2131fa05a4b3168e4`.

Ces empreintes identifient le fichier stock utilisé pour l'analyse locale ; elles ne confèrent aucun droit de redistribution.

## Limites de l'environnement

L'environnement de préparation ne contenait pas m68k-linux-gnu-gcc/as/ld, m68k-elf-gcc/as/ld, de backend Clang M68K/ColdFire, qemu-m68k, DigiKit/digiemu ni de checkout elekloader local avec le SDK alpha exact. DNS vers GitHub était indisponible dans le conteneur. Restent non exécutés :

1. Build des .elemod avec le vrai SDK partagé et le compilateur ColdFire.
2. Runner hôte upstream avec les vrais headers.
3. Lint et linkage core + compagnon + trois modules dans un build combiné.
4. Génération et vérification d'un firmware .syx modifié.
5. Démarrage dans un émulateur et mesure du budget CPU huit voix.
6. Test Digitone MKI physique : note-off/release, p-locks, LFO, MIDI, persistance des projets et voice stealing.

Un test hôte réussi ne vaut ni certification d'installation, ni preuve de compatibilité matérielle. Ne pas flasher les sources alpha.
