# Security policy / Politique de sécurité

## Français
Les images firmware et le code tiers sont des entrées non fiables. Ne flashez pas un artefact au seul motif que le build a réussi. Signalez les vulnérabilités aux mainteneurs par un canal privé GitHub ; ne publiez pas d'exploit opérationnel et n'incluez ni firmware propriétaire ni secrets.

Avant un builder public : sandbox, limites CPU/mémoire/temps, vérification des dépendances, provenance, compilation sans privilèges, gestion sûre des fichiers temporaires et journal d'audit. Sauvegarde et récupération doivent être décrites avant tout essai matériel.

## English
Firmware images and third-party code are untrusted inputs. Do not flash an artifact merely because the build succeeded. Report vulnerabilities privately to maintainers through GitHub; do not publish actionable exploits or include proprietary firmware or secrets.

Before a public builder: sandboxing, CPU/memory/time limits, dependency verification, provenance checks, unprivileged builds, safe temporary files and audit logs. Backup and recovery must be documented before hardware testing.
