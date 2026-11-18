# Security policy / Politique de sécurité

## Français

DN1-LAB manipule des sources C, des manifestes, des scripts de construction et, dans le futur, des images firmware fournies localement. Ces entrées doivent être considérées comme non fiables.

### Signaler une vulnérabilité
Ne publie pas de détails exploitables, d'exploit, de firmware propriétaire ou de dumps sensibles dans une Issue publique. Utilise le mécanisme de signalement privé de GitHub s'il est activé pour ce dépôt. Si ce mécanisme n'est pas disponible, contacte le mainteneur par un canal privé accessible depuis son profil GitHub, sans joindre de secret ni de firmware propriétaire. Les signalements publics généraux ne doivent contenir que l'impact et les étapes non sensibles.

### Principes de sécurité
- Ne jamais exécuter des modules tiers avec privilèges élevés.
- Vérifier versions, provenance, empreintes et dépendances avant une construction.
- Le futur builder doit être isolé et limité en CPU, mémoire, durée et accès fichiers.
- Traiter les fichiers SysEx et archives comme des entrées malformées possibles.
- Utiliser des chemins temporaires sûrs, nettoyer les fichiers temporaires et produire des journaux auditables.
- Un hash identifie un fichier, mais ne prouve pas à lui seul son authenticité.
- Un build réussi ne garantit pas un firmware sûr ni un fonctionnement sur matériel.
- Exiger sauvegarde, procédure de récupération et validation sur une unité récupérable avant tout essai.

Aucune fonction de flash matériel n'est fournie par le Workbench actuel.

## English

DN1-LAB handles C sources, manifests, build scripts and, in future, firmware images supplied locally. Treat these inputs as untrusted.

### Reporting a vulnerability
Do not publish actionable exploit details, exploits, proprietary firmware or sensitive dumps in a public Issue. Use GitHub's private vulnerability reporting feature if it is enabled for this repository. If it is unavailable, contact the maintainer privately through a channel available from their GitHub profile; do not attach secrets or proprietary firmware. Public general reports should contain only non-sensitive impact and reproduction details.

### Security principles
- Never run third-party modules with elevated privileges.
- Verify versions, provenance, digests and dependencies before building.
- Any future builder must be isolated and resource-limited.
- Treat SysEx files and archives as potentially malformed input.
- Use safe temporary paths, clean temporary files and retain auditable logs.
- A digest identifies a file but does not by itself prove authenticity.
- A successful build does not guarantee safe firmware or hardware behavior.
- Require backup, recovery procedures and testing on a recoverable unit before hardware experiments.

The current Workbench does not provide any hardware flashing function.
