# Roadmap / Feuille de route

## Phase 0 — Public foundation and source import
- [x] Bilingual project identity, purpose and contributor documentation.
- [x] Import DN1-LAB 0.2.2-alpha source trees and preserve source provenance.
- [x] Retain legacy 0.1.1 and add the SDK-alpha 0.2.2 branch without mixing APIs.
- [x] Re-run host tests, numerical checks, sanitizers and static analysis locally.
- [x] Publish source-backed catalogue entries and honest L2 evidence statements.

## Phase 1 — Reproducible source and CI
- [x] Add manifest/catalogue/site validation scripts.
- [x] Add GCC and Clang GitHub Actions validation matrix.
- [ ] Pin the exact upstream elekloader SDK commit and compatible ColdFire toolchain.
- [ ] Add upstream SDK build and emulator jobs when prerequisites are available.

## Phase 2 — Community catalogue and Workbench
- [x] Machine manifests for KARST, MORPHANT and TESSERA.
- [x] Six bilingual research proposals with feasibility gates.
- [x] Static Workbench: module selection, target declaration, local firmware hashing and JSON plan export.
- [ ] Enable GitHub Discussions and configure FR/EN categories in repository settings.
- [ ] Add community-submitted module review and provenance workflow.

## Phase 3 — Firmware preflight
- [x] Browser-local SHA-256 of a user-selected stock file; no upload.
- [ ] Parse and verify the ELE3/SysEx container read-only against exact known OS profiles.
- [ ] Resolve dependency graph and verify combined machine-ID conflicts against the target linker.
- [ ] Export a fully resolved plan with toolchain and upstream commit pins.

## Phase 4 — Builder and Workbench integration
- [x] Local catalogue and plan-only UI; no firmware patching or flashing.
- [ ] Version-pinned, isolated SDK and companion build.
- [ ] Combined lint, linker and deterministic modified-image verification.
- [ ] Emulator validation and CPU budget profiling with eight simultaneous voices.
- [ ] Optional controlled CI build with explicit user opt-in.

## Phase 5 — Experimental machine research
- [ ] Prototype VESPER, MYCEL, ORBITAL, LITHOS, PHOTON and ANIMA on the host.
- [ ] Conduct prior-art reviews and blind sound-design evaluation.
- [ ] Prove state, aliasing and feedback bounds; profile worst-case CPU/memory.
- [ ] Port only designs supported by verified SDK APIs and real-time budgets.

Roadmap entries marked unchecked are not implemented. A green host test is not an emulator or hardware certification.
