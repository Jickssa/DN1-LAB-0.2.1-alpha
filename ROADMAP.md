# Roadmap / Feuille de route

## Phase 0 — Public foundation
- [x] Bilingual project identity and introductory documentation.
- [x] Architecture, machine concepts, validation levels and contribution policy.
- [ ] Import the verified DN1-LAB 0.2.x source archive and preserve checksums.
- [ ] Re-run all host tests from a clean checkout.

## Phase 1 — Reproducible source and CI
- [ ] Pin SDK, compiler and build dependencies.
- [ ] Add CI for manifest validation, host tests, sanitizers and static analysis.
- [ ] Publish per-commit validation reports and optional audio artifacts.
- [ ] Validate machine IDs against each combined build.

## Phase 2 — Community catalogue
- [ ] Add machine manifests and schema validation.
- [ ] Publish parameter references, demos and compatibility matrix.
- [ ] Establish community discussion categories and contribution templates.

## Phase 3 — Read-only firmware preflight
- [ ] Local firmware import and hashing.
- [ ] Explicit target detection and inspection.
- [ ] Dependency graph, collision checks and build-plan export.

## Phase 4 — Builder and Workbench
- [ ] Version-pinned isolated builder.
- [ ] Integration tests and deterministic image verification.
- [ ] Local UI for module selection and build configuration.
- [ ] Optional CI workflow with explicit opt-in.

## Phase 5 — Experimental machine research
- [ ] Prototype VESPER, MYCEL, ORBITAL, LITHOS, PHOTON and ANIMA on host.
- [ ] Conduct prior-art and sound-design reviews.
- [ ] Profile against actual target CPU and voice budget.
- [ ] Port only designs supported by verified SDK APIs.

Roadmap items describe goals, not implementation or guaranteed delivery.
