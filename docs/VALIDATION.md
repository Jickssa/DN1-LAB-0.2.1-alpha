# DN1-LAB 0.2.2-alpha — validation record / Rapport de validation

## Summary / Résumé

Cible source : Digitone MKI / Digitone Keys OS 1.44, shared elekloader machine SDK alpha. **Niveau de preuve : L2 (tests hôte).** Aucun build ColdFire, fichier .elemod, firmware modifié, test émulateur ni test sur Digitone physique n'est revendiqué.

## Matrice des tests exécutés localement

| Vérification | GCC | Clang | Résultat |
|---|---:|---:|---|
| Manifestes alpha et enregistrement machine | PASS | PASS | IDs 100/101/102, OS1.44, 15 contrôles |
| Harnais DSP alpha | PASS | PASS | 8 états indépendants, 256 blocs, ordre inversé et retrigger |
| Build hôte de production -Os | PASS | PASS | WAV identiques aux références instrumentées |
| AddressSanitizer + UndefinedBehaviorSanitizer | PASS | PASS | Aucun défaut signalé |
| Analyse statique | PASS | — | GCC -fanalyzer |
| Tests hôte API historique 0.1.1 | PASS | — | Branche dnm_machine |
| Sanitizers historiques | PASS | — | Aucun défaut signalé |
| Validation metadata/plages | PASS | — | Plages OS1.44 et valeurs par défaut |
| Équivalence du pliage TESSERA | PASS | — | 416 045 valeurs du domaine démontré |
| Vérificateur stock OS1.44 | PASS | — | Entrée stock fournie vérifiée |

Le harnais alpha a rendu 2 081 888 échantillons hôte dans ses scénarios automatisés avec zéro clamp dur observé par le compteur de test. Les huit lanes sont testées séparément pour leur état ; ce n'est ni un mixage natif huit voix mesuré ni une mesure de temps d'exécution ColdFire.

## Reproduction

    sha256sum -c SHA256SUMS.txt
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
    python3 tools/verify_stock_os.py /chemin/local/Digitone_and_Digitone_Keys_OS1.44.syx

Le chemin firmware reste local et doit être fourni par l'utilisateur. Le fichier firmware n'est pas inclus.

## Identité du firmware stock vérifié
- SHA-256 SysEx : d4f200d04484333d82822db7744e6484d0def8f2db8ddf55ee2b780cc13c9659
- Conteneur ELE3 : 1 320 656 octets ; SysEx device ID 0x0D ; sept sections [5, 2, 3, 4, 6, 7, 8].
- Section principale : 2 736 304 octets décompressés à 0x40000400.
- SHA-256 de l'image principale : fce648a97c6c5d93b961732e8f8db6b02e0820c6d344c7e2131fa05a4b3168e4.

Ces empreintes identifient le fichier stock utilisé pour l'analyse locale ; elles ne confèrent aucun droit de redistribution.

## Limites de l'environnement

L'environnement de travail ne contenait pas m68k-linux-gnu-gcc/as/ld, m68k-elf-gcc/as/ld, de backend Clang M68K/ColdFire, qemu-m68k, DigiKit/digiemu ni un checkout elekloader local avec le SDK alpha exact. DNS vers GitHub indisponible dans le conteneur local. Restent non exécutés :

1. Build des .elemod avec le vrai SDK partagé et le compilateur ColdFire.
2. Runner upstream SDK alpha avec les vrais headers.
3. Lint et linkage core + compagnon + trois modules dans un build combiné.
4. Génération et vérification d'un firmware .syx modifié.
5. Démarrage dans un émulateur et mesure du budget CPU huit voix.
6. Test Digitone MKI physique : note-off/release, p-locks, LFO, MIDI, persistance des projets et voice stealing.

Le script de build doit échouer proprement si les prérequis sont absents. Ne pas contourner ses contrôles ni flasher un artefact sur la seule base des tests hôte.
