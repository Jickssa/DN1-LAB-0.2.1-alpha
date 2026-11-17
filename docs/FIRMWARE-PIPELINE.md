# Firmware pipeline / Pipeline firmware

## Français
DN1-LAB doit d'abord être complémentaire à elekloader, non un clone supposé de son service. Référence externe : https://github.com/elekloader/elekloader

Le Workbench DN1-LAB aura sa propre identité : catalogue de machines, matrice de compatibilité, manifestes, rapports de validation et orchestration d'un constructeur local versionné. L'utilisateur conserve son firmware localement ; aucun envoi vers un backend par défaut.

Étapes requises : importer le firmware de base ; identifier matériel/OS/format ; établir intégrité et hash ; résoudre manifestes et dépendances ; vérifier IDs et collisions du build combiné ; compiler dans un environnement isolé ; tests unitaires/statique/ABI/ressources ; assemblage déterministe ; vérification de la structure et des checksums ; générer logs, manifeste, SBOM éventuelle et SHA-256 ; afficher avertissements, sauvegarde et récupération avant tout flash.

Un hash identifie un fichier mais ne prouve pas son authenticité. Un build qui termine n'est pas une validation matérielle. Les modules tiers sont du code non fiable et requièrent sandbox et audit.

## English
DN1-LAB should initially complement elekloader, not claim to clone its service. External reference: https://github.com/elekloader/elekloader

The DN1-LAB Workbench should have its own identity: machine catalogue, compatibility matrix, manifests, validation reports and orchestration of a version-pinned local builder. Users keep firmware local; no default backend upload.

Required steps: import base firmware; identify hardware/OS/format; record integrity and digest; resolve manifests and dependencies; check IDs and combined-build collisions; compile in isolation; run unit/static/ABI/resource tests; deterministic assembly; verify structure and checksums; emit logs, manifest, optional SBOM and SHA-256; show warnings, backup and recovery instructions before flashing.

A hash identifies a file but does not prove authenticity. A successful build is not hardware validation. Third-party modules are untrusted code and require sandboxing and audit.
