# MYCEL — Distributed Coupling Synth

**État / Status:** prototype hôte L2 validé par tests automatisés GCC/Clang ; ColdFire, linker combiné et matériel non validés. ID 104 is provisional. **Prior art:** unknown; no claim of uniqueness.

## Intention sonore / Sonic intent
**FR.** Organisme de résonateurs dont la topologie est aussi musicale que les fréquences. Les nœuds sont des masses modales simplifiées ; les arêtes transportent une fraction bornée d'énergie ou de phase. Un contrôle mute le graphe d'une chaîne en anneau puis en étoile.

**EN.** A resonator organism whose topology is as musical as its frequencies. Nodes are simplified modal masses; edges transfer a bounded fraction of energy or phase. One control morphs a chain into a ring and then a star.

## Architecture proposée / Proposed architecture
- Four fixed resonator nodes, two integrators per node.
- Six possible undirected edges with a sparse fixed update schedule.
- Edge coefficients derive from a continuous topology coordinate; no runtime allocation or dynamic graph container.
- Conservative coupling bound and leak term prevent energy growth.

## Contrôles candidats / Candidate controls
TOPO, LINK, MASS, LOSS, CHAOS, EXCIT, BIAS, CROSS, TENSION, FRIC, DENS, SPREAD, DECAY, BRIGHT, MIX.

## Risques et expérience / Risks and experiment
The four-node version may fit the 256-byte state budget, but worst-case operations must be measured. Stability must be proved for every exposed coefficient combination. Test topology extremes, randomized parameter paths, energy decay, zero-input stability, retrigger and voice independence. Compare chain/ring/star morphs in blind listening.
