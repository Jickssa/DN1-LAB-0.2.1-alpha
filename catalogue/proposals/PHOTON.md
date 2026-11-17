# PHOTON — Event-Driven Phase Camera

**État / Status:** concept L0, not implemented. **Prior art:** unknown; no claim of uniqueness.

## Intention sonore / Sonic intent
**FR.** Les événements de note, vélocité et modulation déclenchent des impulsions dans un réseau de phase. Elles se réfléchissent, se recroisent et forment des interférences contrôlables : métal liquide, verre instable, battements microscopiques.

**EN.** Note, velocity and modulation events inject impulses into a phase network. They reflect, cross and form controllable interference patterns.

## Architecture proposée / Proposed architecture
- Three or four phase accumulators with bounded offsets.
- Sparse event injection; no unbounded event queue.
- Fixed reflection/coupling matrix with analytically bounded feedback coefficients.
- Optional low-rate phase diffusion for controlled divergence.

## Contrôles candidats / Candidate controls
PULSE, REFLECT, COUPLE, DIFFUSE, DENS, DECAY, RATIO, SHIFT, SYNC, FEEDBACK, HARD, SOFT, VELOC, JITTER, MIX.

## Risques et expérience / Risks and experiment
Phase discontinuities may alias. Feedback stability and DC behavior require proof. Bound event rate and avoid heap-backed queues. Test single/repeated/asynchronous events; measure spectral spread, peak, DC offset, determinism and worst-case operations per block.
