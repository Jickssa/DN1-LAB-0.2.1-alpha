# VESPER — Resonant Time Loom

**État / Status:** prototype hôte L1 réalisé et testé ; ColdFire, linker combiné et matériel non validés. ID 103 is provisional. **Prior art:** unknown; no claim of uniqueness.

## Intention sonore / Sonic intent
**FR.** Transformer chaque excitation en empreinte temporelle courte qui traverse un champ de résonances. La note ne relit pas un sample : elle re-parcourt les résidus d'excitation tandis que les modes changent de vitesse, de direction et de décroissance. Sons visés : cordes préparées impossibles, métal granulaire, frottements mémorisant leur attaque.

**EN.** Turn each excitation into a short temporal fingerprint moving through a resonant field. The note does not replay a sample: it revisits excitation residues while modes change speed, direction and decay.

## Architecture proposée / Proposed architecture
- Compact per-voice excitation stencil, e.g. 32 signed 8-bit cells plus cursor/phase metadata; no long audio buffer.
- Two or three all-pole resonators fed by different taps/interpolated stencil positions.
- Bounded folded read-head trajectory with reverse and freeze states.
- Slow state variable changes stencil decay and modal ratios per block, not per sample.

## Contrôles candidats / Candidate controls
TRACE, GRAIN, DRIFT, REV, FREEZ, MODES, DECAY, FRAY, BOW, STRIKE, TILT, WIDTH, FEED, DAMP, MIX.

## Risques et expérience / Risks and experiment
State must fit the documented 256-byte per-voice area. Multiple read heads may be expensive at eight voices; abrupt read-rate changes may alias. Start with 32-byte stencil and two resonators. Render impulse/noise/bow inputs, test freeze/reverse/accelerate paths, measure state size, cycles per block, peak level and foldback.

Acceptance requires a useful sound-design space distinct from KARST without long buffers. Prior-art research is mandatory before claiming novelty.
