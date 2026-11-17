# DN1-LAB 0.2.2-alpha — Instrument guide

The controls below reflect the shared machine SDK alpha source declarations. Values are logical knob ranges, not raw 8.8 SYN words. The two custom pages are exposed in the Digitone MKI track's SYN2 area.

## KARST — Modal Matter

| Page | Encoder | Control | Range | Function |
|---|---:|---|---:|---|
| MATTER | A | MTRL | 0–7 | Wood, metal, glass, membrane, bell, stone, tube, crystal |
| MATTER | B | STIF | 0–127 | Shifts upper modes |
| MATTER | C | SPRK | 0–127 | Relative energy/brightness of upper partials |
| MATTER | D | INH | 0–127 | Inharmonicity of upper mode |
| MATTER | E | BOW | 0–127 | Continuous noisy bow-like excitation |
| MATTER | F | EDGE | 0–127 | Weight toward upper modes |
| MATTER | G | DAMP | 0–127 | Relative damping of upper modes |
| MATTER | H | DECY | 0–127 | Decay time |
| RESON | A | TONE | 0–127 | Spectral balance and damping |
| RESON | B | HIT | 0–127 | Note-on strike intensity |
| RESON | C | NOIS | 0–127 | Continuous noise excitation |
| RESON | D | RASP | 0–127 | Resolution reduction |
| RESON | E | SPRD | 0–127 | Separation of upper modes |
| RESON | F | BODY | 0–127 | Fundamental-mode weighting |
| RESON | G | LVL | 0–127 | Modal level before native AMP |

**Glass Bow:** choose glass or crystal, medium-high SPRK, medium INH, BOW 40–70, high DECY and modest NOIS. Use a slow LFO on BOW or EDGE.

**Stone Drops:** choose stone, set BOW to zero, raise HIT, use medium DECY, high DAMP and low EDGE. Use parameter locks on HIT and MTRL for tuned percussion patterns.

## MORPHANT — Formant Organism

| Page | Encoder | Control | Range | Function |
|---|---:|---|---:|---|
| VOICE | A | VOWL | 0–4 | A/E/I/O/U profile position, interpolated over time |
| VOICE | B | FORM | 0–127 | Spectral ridge position |
| VOICE | C | FINE | −100…+100 cents | Fine fundamental tuning |
| VOICE | D | HARM | 0–127 | Six-partial spectral tilt |
| VOICE | E | BRETH | 0–127 | Slowly filtered breath noise |
| VOICE | F | NOIS | 0–127 | Broadband noise |
| VOICE | G | DRFT | 0–127 | Slow vibrato depth |
| VOICE | H | GLID | 0–127 | Morph speed; higher means slower transition |
| SPECT | A | NASL | 0–127 | Boost of central partials |
| SPECT | B | AIR | 0–127 | High-pass noise texture |
| SPECT | C | SUB | 0–127 | Sub-octave blend |
| SPECT | D | OCT | −12…+12 semitones | Octave transposition |
| SPECT | E | BITE | 0–127 | Spectral emphasis and fold |
| SPECT | F | FM2 | 0–127 | Secondary emphasis on fifth partial |
| SPECT | G | MIX | 0–127 | Harmonic / breath-layer balance |

**Vocal Glass:** set VOWL near I/O, FORM around the middle, HARM medium, NASL 20–45, modest AIR/BRETH, medium-high GLID and high MIX.

**Choir Dust:** automate A→U motion, medium NASL, moderate BRETH/AIR, subtle DRFT, low SUB and low BITE. Use the Digitone's native polyphony and effects.

LVL is intentionally omitted: the native AMP page controls overall level and release.

## TESSERA — Phase Lattice

| Page | Encoder | Control | Range | Function |
|---|---:|---|---:|---|
| PHASE | A | RATIO | 0–7 | Modulator ratio from eight discrete ratios |
| PHASE | B | INDEX | 0–127 | Phase-modulation depth |
| PHASE | C | FOLD | 0–127 | Wavefold amount |
| PHASE | D | SKEW | 0–127 | Asymmetric phase warp |
| PHASE | E | SYNC | 0–127 | Modulator sync on carrier wrap |
| PHASE | F | FBK | 0–127 | Previous-output feedback |
| PHASE | G | SUB | 0–127 | Sub-octave blend |
| PHASE | H | LVL | 0–127 | Generator level |
| SHAPE | A | DETUN | −100…+100 cents | Modulator detuning |
| SHAPE | B | BIAS | 0–127 | DC bias before fold |
| SHAPE | C | RING | 0–127 | Blend toward product component |
| SHAPE | D | BITS | 0–127 | 0 bypasses; 1 is coarse; 127 is about 10-bit |
| SHAPE | E | GLCH | 0–127 | Probability/resolution of quantized glitches |
| SHAPE | F | NOIS | 0–127 | Noise layer |
| SHAPE | G | MIX | 0–127 | Carrier/modulator balance |

**Lattice Bell:** RATIO near 4 or 5, moderate INDEX, low FOLD, high SYNC, low FBK, subtle DETUN and medium RING.

**Broken Orbit:** higher RATIO, medium-high INDEX/SKEW, low-to-medium FBK, slightly offset BIAS and low GLCH.

DUST was removed because its layer duplicated NOIS. BITS=0 disables quantization; GLCH is active from value 1.

## Recommended test order

1. Test one machine on one track with moderate AMP/effects.
2. Check MIDI notes 36, 48, 60, 72 and 84, attack and native AMP release.
3. Exercise every encoder, LFO, parameter locks, sound changes and machine changes.
4. Add voices and tracks gradually; profile TESSERA and MORPHANT carefully.
5. Repeat in an emulator before testing on the instrument.

These are programming notes, not preset .syx or .dnprj files.
