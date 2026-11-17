# DN1-LAB — feuille de route

## Phase A — trois moteurs SDK alpha
- [x] KARST, MORPHANT et TESSERA portés vers le SDK partagé alpha.
- [x] Tests de paramètres, pitch, retrigger, état indépendant et host headroom.
- [ ] Import complet du harnais existant dans le dépôt communautaire et CI reproductible.
- [ ] Build ColdFire, lint/linker combiné, vérification image OS1.44.
- [ ] Émulateur, profil cycle/block à huit voix, test Digitone physique.

## Phase B — six prototypes hôte
- [x] VESPER : memory stencil + résonateurs.
- [x] MYCEL : réseau de résonateurs couplés à énergie bornée.
- [x] ORBITAL : champ harmonique avec hystérésis de chemin.
- [x] LITHOS : redistribution spectrale entre trois réservoirs.
- [x] PHOTON : impulsions clairsemées dans un réseau de phase.
- [x] ANIMA : écologie locale à quatre agents ; repli voix-locale seulement.
- [x] Tests GCC/Clang, sanitizers, analyse statique, bornes d'état et tests spécifiques.
- [x] Préécoutes hôte mono 48 kHz.
- [ ] Comparaison d'écoute à l'aveugle et réglage des paramètres avec un musicien.
- [ ] Mesures cycle/block sur le ColdFire et contrôle anti-aliasing cible.
- [ ] Audit d'antériorité approfondi par concept et étude du registre d'IDs.
- [ ] Portage individualisé vers `sdk-alpha/` seulement après validation de l'API et budget CPU.

## Phase C — intégration firmware additionnelle
- [ ] Vérifier les identifiants 103–108 dans le linker combiné et le budget total de machines.
- [ ] Construire chaque module en `.elemod` ColdFire reproductible.
- [ ] Lint et lien avec les versions upstream explicitement épinglées.
- [ ] Vérifier l'image patchée et son empreinte à partir de l'OS stock exact.
- [ ] Test émulateur puis test matériel sur une unité récupérable.

## Conditions de passage

Un test hôte vert ne suffit pas pour déclarer un module compatible Digitone. Chaque module doit franchir la preuve mémoire, les tests de stabilité/aliasing, le budget temps réel au pire cas, la vérification SDK/linker, l'émulateur et le matériel. Les identifiants sont candidats et l'état global inter-voix reste interdit tant que l'API ne définit pas son cycle de vie et sa synchronisation.