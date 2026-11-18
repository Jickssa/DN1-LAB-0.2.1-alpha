# Source provenance / Provenance des sources

## Imported DN1-LAB 0.2.2-alpha source package

The three shared SDK-alpha engines and their initial documentation originated in the DN1-LAB 0.2.2-alpha working package prepared on 17 November 2026:

- Original archive SHA-256: `77d467e7a85295880deed7df5016c2026c65a9e5d17a5d297c6439475b4a5f42`
- The stock Digitone OS 1.44 image used during local analysis is not included or redistributed.

## DN1-LAB 0.3.0-alpha additions

- Six host DSP prototype source trees are checked into `prototypes/`: VESPER, MYCEL, ORBITAL, LITHOS, PHOTON and ANIMA.
- Their local test-interface subset is in `host/include/`; it is a host harness, not the upstream SDK header.
- Prototype tests, manifests, validators and documentation are checked into `tests/`, `prototypes/` and `docs/`.
- The prototype sources are deliberately separate from `sdk-alpha/` and are not included in the target firmware builder.
- Generated host executables and WAV renders are not tracked. The WAV previews can be regenerated locally with `tests/run_prototype_demos.sh`.

## Import boundaries

- The full historical 0.1.1 `dnm_machine` API source and the complete 0.2.2-alpha legacy host test package are not fully imported file-by-file into this public repository.
- The three shared SDK-alpha sources are browsable in `sdk-alpha/`.
- The six 0.3.0-alpha DSP prototype sources are browsable in `prototypes/`.
- The Workbench and static site are repository components.
- Firmware hashes in validation documents identify a local analysis input only; they do not grant redistribution rights.

Do not infer that every file from the original working archive exists in this repository. Every external source, binary, algorithmic reference or dependency added later must have its own provenance, license and integrity record.
