# DN1-LAB — Research notes and prior-art review / Recherche et antériorité

## Scope and claim policy / Périmètre et prudence des revendications

The six names VESPER, MYCEL, ORBITAL, LITHOS, PHOTON and ANIMA describe proposed design combinations. They are not claims of first invention, scientific novelty or absence of prior art. Sound synthesis research is broad, and related methods already exist in modal synthesis, coupled physical networks, gesture-controlled instruments, event/impulse models, time-varying spectra and distributed/coupled oscillators.

Les six noms désignent des combinaisons de conception proposées. Ils ne signifient pas que les techniques sous-jacentes n'ont jamais existé. Avant tout dépôt public affirmant une nouveauté, il faut définir précisément la différence technique, rechercher brevets/articles/instruments/plugins, puis comparer par mesures et écoutes contrôlées.

## Verified related work / Travaux apparentés identifiés

1. Poirot, Bilbao & Kronland-Martinet (2024), “A simplified and controllable model of mode coupling for addressing nonlinear phenomena in sound synthesis processes”. Couplage de filtres résonants, transfert d'énergie entre modes et synthèse temps réel. This directly overlaps MYCEL's broad idea of coupled resonator nodes and energy exchange. [EURASIP open-access article](https://link.springer.com/article/10.1186/s13636-024-00358-2).

2. Ducceschi, Bilbao & Webb (DAFx 2023), “Real-time modal synthesis of nonlinearly interconnected networks”. Nonlinearly interconnected modal systems and real-time physical-model synthesis. This is substantial prior art for modal networks; a narrower MYCEL contribution would need to demonstrate a tiny bounded graph, topology morphing under the Digitone state/CPU budget and useful musical controls. [DAFx 2023 paper](https://www.dafx.de/paper-archive/2023/DAFx23_paper_17.pdf).

3. Bilbao, Ducceschi & Webb (DAFx 2019), “Large-scale real-time Modular Physical Modeling Sound Synthesis”. Modular components and nonlinear interconnection networks already have a published precedent. [DAFx 2019 paper](https://www.mdphys.org/PDF/dafxNET2_2019.pdf).

4. Howard & Rimell (2004), “Real-Time Gesture-Controlled Physical Modelling Music Synthesis with Tactile Feedback”. Gesture-controlled physical modelling is established prior art. [Journal article PDF](https://asp-eurasipjournals.springeropen.com/counter/pdf/10.1155/S1110865704311182.pdf).

5. Couturier (NIME 2002), “A Scanned Synthesis virtual instrument”. A gesture-driven scanned-synthesis instrument was presented long before this project. ORBITAL cannot claim novelty merely for mapping a gesture path to changing spectral or timbral parameters. [NIME 2002 proceedings](https://www.nime.org/proceedings/2002/nime2002_107.pdf).

6. Linke, Bader & Mores (2023), “Model-based sonification based on the impulse pattern formulation”. Describes coupled subsystems, impulse excitation, reflection, attenuation and feedback. This overlaps PHOTON's broad event/impulse network language. [Springer open-access article](https://link.springer.com/article/10.1007/s12193-023-00423-8).

7. Christopher et al. (2014), “Interactive Sound Synthesis Mediated through Computer Networks”. Impulse feedback across distributed performers is used to evolve sonic material. [eContact article](https://www.econtact.ca/16_2/christopher-etal_synthesis.html).

8. Puckette (ICMC 2001), “Synthesizing Sounds with Specified, Time-Varying Spectra”. Time-varying partials and spectral envelopes are established techniques. LITHOS must differentiate itself through a defined erosion/deposition state model, not simply spectral motion. [Publication page](https://msp.ucsd.edu/Publications/icmc01-paf.dir/).

9. “Sound in Multiples: Synchrony and Interaction Design using Coupled-Oscillator Networks” (SMC 2019) explores synchrony, interaction design and coupled oscillator networks. This is relevant prior art to PHOTON and ANIMA's phase/voice-interaction framing. [Proceedings paper](https://nolanlem.com/pdfs/smc_2019-FINAL.pdf).

10. François, Resonate (2025–2026), presents resonator-based temporal accumulation with exponentially decaying memory, low memory footprint and sample-by-sample updates. This overlaps VESPER's broad resonant-memory direction. [Project and publications](https://alexandrefrancois.org/Resonate/).

This is a targeted, not exhaustive, literature scan. It does not cover all patents, commercial instruments, unpublished work, theses or source-code projects. Dates and versions should be rechecked before any formal novelty assessment.

## Per-concept narrowing / Différenciation à établir

| Concept | Broad prior art already present | Candidate engineering distinction to test |
|---|---|---|
| VESPER | Resonant memory, resonator banks and impulse responses | Very short per-voice excitation stencil, moving/reversible read heads coupled to evolving modal ratios within 256 bytes |
| MYCEL | Coupled modal networks and interconnected physical models | Fixed four-node sparse graph with topology morphing, bounded energy transfer and a strict low-CPU Digitone mapping |
| ORBITAL | Gesture control, scanned synthesis, time-varying spectra | Path-dependent harmonic field with explicit same-endpoint/different-path hysteresis and fixed-point resource bounds |
| LITHOS | Spectral envelopes, partial evolution and energy-transfer models | Low-dimensional erosion/deposition/fracture reservoirs with conservative energy bookkeeping and audibly testable material states |
| PHOTON | Impulse-pattern systems and coupled oscillator phase networks | Sparse note-event phase injection with a bounded small-state reflection graph, deterministic event cadence and anti-alias constraints |
| ANIMA | Coupled oscillators, distributed systems and ensemble interaction | Voice-order-independent shared state, only if the actual SDK offers safe global-state and synchronization; otherwise a voice-local fallback |

## Experimental acceptance gates

Each proposal must have: a source and patent search log; a sound-design question existing engines do not answer; a minimal host prototype; a control-usefulness test; bounded state, feedback, aliasing and energy; worst-case operations and memory estimates; comparison to a baseline engine; deterministic regression tests; and an SDK feasibility review. No concept is promoted to the installable catalogue before target-build, emulator and hardware evidence.

## Important design warning

ANIMA is system-level, whereas the currently documented SDK callback primarily exposes per-voice state. Do not implement cross-voice coupling with an assumed static global variable. First prove the SDK's global lifetime, threading and note-stealing semantics. The proposed fallback is a voice-local four-agent ecology, which is a narrower and different design.
