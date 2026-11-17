# Workbench / Atelier

## Français
Choix recommandé : construire une interface DN1-LAB propre, qui renvoie vers elekloader pour les fonctions existantes qu'elle ne réimplémente pas. Ne pas copier son identité graphique ni prétendre fournir la même chaîne d'assemblage.

Parcours utilisateur cible : sélectionner Digitone MKI et OS exact ; importer le firmware local ; confirmer la détection ; choisir les modules ; visualiser dépendances, conflits d'ID, limites, ressources et niveau de validation ; générer un plan de build ; exécuter un builder local isolé ; consulter logs/tests/hash ; télécharger l'artefact et lire les consignes de récupération.

V1 catalogue : fiches, filtres de compatibilité, licences, statut de preuve et liens elekloader. Pas de génération.
V2 preflight : import local, inspection en lecture seule, dépendances, collisions et plan exportable.
V3 builder : SDK et outils versionnés, sandbox, intégration déterministe, tests, manifeste et artefact local.

Aucune de ces versions d'interface ou de génération n'est implémentée dans cette branche : ce document est une spécification.

## English
Recommended direction: build a distinct DN1-LAB interface and link to elekloader for existing capabilities that DN1-LAB does not implement. Do not copy its visual identity or imply an equivalent assembly pipeline.

Target workflow: select Digitone MKI and exact OS; import firmware locally; confirm detection; select modules; inspect dependencies, ID conflicts, limits, resources and validation level; generate a build plan; run an isolated local builder; inspect logs/tests/digest; download the artifact and read recovery instructions.

V1 catalogue: entries, compatibility filters, licenses, evidence status and elekloader links. No generation.
V2 preflight: local import, read-only inspection, dependency/conflict checks and exportable plan.
V3 builder: pinned SDK/toolchain, sandbox, deterministic integration, tests, manifest and local artifact.

None of these UI or generation versions is implemented in this branch; this is a specification.
