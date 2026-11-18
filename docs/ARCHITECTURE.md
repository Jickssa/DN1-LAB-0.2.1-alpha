# Architecture / Architecture

## Français

DN1-LAB sépare six couches : (1) le noyau DSP indépendant du firmware, (2) les manifestes de machines, (3) les adaptateurs vers une version donnée du SDK, (4) un runtime hôte de test, (5) un moteur de validation et (6) le constructeur firmware. Le Workbench ne fait qu'orchestrer ces couches ; il ne doit pas réimplémenter silencieusement le format firmware.

Un manifeste doit déclarer identifiant, version, matériel/OS cibles, API SDK, paramètres, ressources, dépendances, licence, provenance et empreintes. Les IDs et contraintes de nombre de machines sont vérifiés sur le build combiné, pas machine par machine seulement.

Flux cible : `manifest -> validation -> résolution des dépendances -> build SDK -> packaging -> build d'intégration -> image firmware -> vérification`. Toute étape échouée bloque la suite. Les logs, versions et hashes sont conservés pour reproduire le résultat.

Le runtime hôte ne simule que les services explicitement modélisés ; il n'est pas un émulateur matériel. La compatibilité exige une matrice matériel / OS / SDK et un niveau de preuve identifiable.

## English

DN1-LAB separates six layers: (1) firmware-independent DSP cores, (2) machine manifests, (3) adapters for a specific SDK version, (4) a host test runtime, (5) a validation engine and (6) the firmware builder. The Workbench orchestrates these layers; it must not silently reimplement firmware formats.

A manifest must declare ID, version, target hardware/OS, SDK API, parameters, resource requirements, dependencies, license, provenance and digests. IDs and machine-count constraints are checked against the combined build, not only per machine.

Target flow: `manifest -> validation -> dependency resolution -> SDK build -> packaging -> integration build -> firmware image -> verification`. Any failed step blocks subsequent work. Logs, versions and hashes are retained for reproducibility.

The host runtime models only documented services; it is not a hardware emulator. Compatibility requires an explicit hardware / OS / SDK matrix and an identifiable evidence level.


## Organisation des implémentations / Implementation workstreams

- `sdk-alpha/`: three shared SDK-alpha ports, host-tested (L2), but not yet built or linked for the target.
- `prototypes/`: six host-tested DSP prototypes (L2), deliberately outside the firmware builder.
- `host/`: local subset of the API used only by prototype host harnesses; never substitute it for upstream target headers.

Les prototypes disposent de tests hôte complets, mais restent séparés de la chaîne firmware tant que le vrai SDK, l'ABI, le budget cycles/bloc et le linkage combiné n'ont pas été vérifiés.
