# ANIMA — Cross-Voice Ecology Bus

**État / Status:** fallback local à quatre agents réalisé et testé au niveau hôte L2 ; la proposition inter-voix reste concept L0, bloquée sur une API SDK synchronisée vérifiée. ID 108 is provisional. **Prior art:** unknown; no claim of uniqueness.

## Intention sonore / Sonic intent
**FR.** Les voix d'une même machine forment un écosystème : chacune publie une mesure lente de densité, tension, hauteur ou activité ; les autres reçoivent une influence limitée. L'objectif est un comportement collectif sans qu'une voix possède l'état d'une autre.

**EN.** Voices of one machine form an ecosystem: each publishes a slow measure of density, tension, pitch or activity; others receive bounded influence. The goal is collective behavior without one voice owning another's state.

## Architecture proposée / Proposed architecture
- Fixed-size shared control block only if the SDK exposes safe machine-global lifetime and synchronization.
- Per-voice publish/consume values updated at block boundaries.
- Deterministic, voice-order-independent reduction.
- Fallback: four virtual agents inside one voice; no cross-voice access.

## Contrôles candidats / Candidate controls
POPUL, COHES, MUTUAL, PRESS, DENS, TENSION, BIAS, DIFF, MEMORY, DECAY, SYNC, LOCAL, GLOBAL, SEED, MIX.

## Risques et expérience / Risks and experiment
The documented voice callback does not prove thread-safe shared state. Never implement this with guessed static globals. If the SDK lacks shared-state support, keep the fallback voice-local. Test reversed callback order, voice stealing, simultaneous starts, voice-count changes and races.
