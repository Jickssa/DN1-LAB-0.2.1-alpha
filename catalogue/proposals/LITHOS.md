# LITHOS — Spectral Erosion Engine

**État / Status:** prototype hôte L2 validé par tests automatisés GCC/Clang ; ColdFire, linker combiné et matériel non validés. ID 106 is provisional. **Prior art:** unknown; no claim of uniqueness.

## Intention sonore / Sonic intent
**FR.** Une matière spectrale s'use, se dépose, se fissure et se recompose. L'énergie des partiels est transférée entre quelques classes de grain plutôt que simplement filtrée. Le geste transforme un son lisse en texture poreuse puis en éclats métalliques.

**EN.** A spectral material wears down, deposits, fractures and reforms. Partial energy moves between grain classes rather than merely being filtered.

## Architecture proposée / Proposed architecture
- Six partial amplitudes plus three low-cost reservoirs (smooth / granular / fractured).
- Bounded transfer matrix moves energy between bins at control rate.
- Excitation injects energy; decay and erosion remove it.
- Additive fixed-point core; no FFT or per-sample allocation.

## Contrôles candidats / Candidate controls
ERODE, DEPOSIT, FRACT, GRAIN, POROUS, SEAM, DUST, MASS, WEAR, REPAIR, TILT, SPREAD, HIT, DECAY, MIX.

## Risques et expérience / Risks and experiment
Energy transfer must be conservative or explicitly damped. Smooth parameter transitions; keep partials below Nyquist. Measure total energy in every state, zero-input convergence, retrigger and high-note aliasing. Differentiation must come from the erosion/deposition state model rather than vocabulary.
