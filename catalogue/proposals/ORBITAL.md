# ORBITAL — Gesture-Conditioned Harmonic Field

**État / Status:** prototype hôte L1 réalisé et testé ; ColdFire, linker combiné et matériel non validés. ID 105 is provisional. **Prior art:** unknown; no claim of uniqueness.

## Intention sonore / Sonic intent
**FR.** Une trajectoire bidimensionnelle pilote un champ de rapports harmoniques et d'amplitudes. Le timbre dépend du chemin parcouru, pas seulement de la position actuelle : deux gestes arrivant au même point peuvent produire deux spectres par hystérésis contrôlée.

**EN.** A two-dimensional path steers a field of harmonic ratios and amplitudes. Timbre depends on the path, not only the current point: two gestures ending at the same point can yield different spectra through controlled hysteresis.

## Architecture proposée / Proposed architecture
- Two bounded phase coordinates from envelopes, LFO/CC or internal gesture dynamics.
- Small fixed harmonic bank; each partial maps ratio and gain from a low-dimensional coefficient field.
- Path memory stores previous vector and filtered direction.
- Fixed-point/table interpolation; no arbitrary wavetable scan.

## Contrôles candidats / Candidate controls
X, Y, PATH, TURN, FIELD, HARM, SPREAD, LOCK, FRIC, SLEW, FOLD, SUB, NOIS, VIB, MIX.

## Risques et expérience / Risks and experiment
Harmonic mapping can alias under rapid gestures. Bound partial count and maximum instantaneous frequency. Compare straight, circular, figure-eight and self-intersecting paths; test path reversal and same-endpoint/different-path behavior. Hysteresis must be audible and musically useful.
