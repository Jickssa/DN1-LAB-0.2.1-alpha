# Roadmap / Feuille de route

## Phase 0 — Public foundation and source import
- [x] Bilingual identity, project goals and contributor documentation.
- [x] Import three DN1-LAB 0.2.2-alpha shared SDK machine engines.
- [x] Preserve legacy API/test sources in a SHA-256-verified source bundle.
- [x] Add structured machine metadata and validation against source IDs.
- [x] Add a local Workbench plan exporter and bilingual static site.

## Phase 1 — Reproducible source and CI
- [x] Validate the source bundle hash before extraction.
- [x] Run host DSP and legacy regression suites with GCC in CI.
- [x] Run alpha regression, optimized builds and sanitizers with Clang.
- [x] Run GCC static analysis and catalogue/site validation.
- [ ] Pin the exact upstream elekloader SDK commit and ColdFire toolchain.
- [ ] Add upstream SDK build and emulator jobs when prerequisites are available.

## Phase 2 — Community catalogue and Workbench
- [x] Machine manifests for KARST, MORPHANT and TESSERA.
- [x] Six bilingual research proposals and feasibility gates.
- [x] Static Workbench: selection, local SHA-256 and JSON plan export.
- [ ] Enable GitHub Discussions and configure FR/EN categories in repository settings.
- [ ] Add community module review and provenance workflow.

## Phase 3 — Firmware preflight
- [x] Browser-local SHA-256 for a user-selected stock file; no upload.
- [ ] Read-only ELE3/SysEx parser for exact known OS profiles.
- [ ] Dependency graph and combined machine-ID validation against target linker.
- [ ] Fully resolved, version-pinned build-plan export.

## Phase 4 — Builder and Workbench integration
- [x] Local catalogue and plan-only UI; no firmware patching or flashing.
- [ ] Version-pinned isolated SDK/companion builder.
- [ ] Combined lint, linker and deterministic modified-image verification.
- [ ] Emulator validation and CPU profiling with eight simultaneous voices.
- [ ] Optional CI build with explicit user opt-in.

## Phase 5 — Experimental machine research
- [ ] Prototype VESPER, MYCEL, ORBITAL, LITHOS, PHOTON and ANIMA on host.
- [ ] Conduct prior-art reviews and blind sound-design evaluation.
- [ ] Prove state, aliasing, feedback and energy bounds; profile worst-case CPU/memory.
- [ ] Port only designs supported by verified SDK APIs and real-time budgets.

Unchecked items are not implemented. Green host tests are not emulator or hardware certification.
