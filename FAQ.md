# FAQ / Foire aux questions

## Français

### Peut-on installer les machines sur un Digitone maintenant ?
Non. Les trois machines SDK alpha sont testées sur hôte, mais leur build ColdFire, le linkage combiné, l'émulation et l'essai matériel ne sont pas validés. Les six nouveaux moteurs sont encore des prototypes hôte. Aucun `.elemod` ni firmware modifié n'est publié.

### Que signifie « testé » dans le README ?
Le dépôt distingue les niveaux L0 à L5 : concept, prototype hôte, tests hôte, build cible, émulateur, matériel. Voir le [README](README.md) et les rapports de validation. Un test hôte vert ne démontre pas le budget CPU du Digitone.

### Pourquoi les IDs 103 à 108 sont-ils provisoires ?
Un ID isolé peut sembler libre, mais les collisions et limites doivent être contrôlées dans le build combiné avec les compagnons et toutes les machines. Ce contrôle n'a pas encore été exécuté.

### Où sont les préécoutes audio ?
Le générateur de démonstration est dans `tests/run_prototype_demos.sh`. Il crée localement six WAV mono 48 kHz. Ce sont des rendus hôte secs, pas des enregistrements du Digitone ni des presets installables.

### ANIMA fait-il interagir plusieurs voix du Digitone ?
Non. L'implémentation actuelle contient quatre agents virtuels dans une seule voix. Le bus inter-voix reste bloqué jusqu'à ce que le SDK garantisse un état global synchronisé et son cycle de vie.

### Comment proposer un nouveau moteur ?
Ouvre une Discussion pour le concept ou une Issue de proposition. Décris le résultat sonore, l'architecture, les contrôles, les contraintes CPU/mémoire, les références, la licence et les critères de validation.

### DN1-LAB est-il affilié à Elektron ?
Non. Il s'agit d'un travail indépendant et non officiel.

## English

### Can I install these machines on a Digitone now?
No. The three shared SDK-alpha engines have host tests, but their ColdFire build, combined linkage, emulator validation and hardware testing remain unverified. The six new engines are host prototypes. No `.elemod` or modified firmware is published.

### What does “tested” mean in the README?
The repository distinguishes L0–L5: concept, host prototype, host tests, target build, emulator and hardware. See the README and validation reports. A passing host test does not prove the Digitone CPU budget.

### Why are IDs 103–108 provisional?
An ID may appear unused in one artifact, but collisions and machine-count limits must be checked in the combined build with all companions and machines. That check has not yet run.

### Where are the audio previews?
The demo generator is `tests/run_prototype_demos.sh`. It creates six local 48 kHz mono WAVs. These are dry host renders, not Digitone recordings or installable presets.

### Does ANIMA make Digitone voices interact?
No. The current implementation contains four virtual agents inside one voice. Cross-voice behavior is blocked until the SDK guarantees synchronized shared state and its lifecycle.

### How do I propose a new engine?
Open a Discussion for an idea or an Issue for a structured proposal. Describe the sonic goal, architecture, controls, CPU/memory constraints, references, license and validation criteria.

### Is DN1-LAB affiliated with Elektron?
No. It is independent, unofficial research.
