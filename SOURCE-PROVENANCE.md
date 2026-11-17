# Source provenance / Provenance des sources

## Imported DN1-LAB source release

The SDK-alpha engines and documentation are based on the DN1-LAB 0.2.2-alpha source package prepared on 17 November 2026:

- Original ZIP: `DN1-LAB-0.2.2-alpha.zip`
- ZIP SHA-256: `77d467e7a85295880deed7df5016c2026c65a9e5d17a5d297c6439475b4a5f42`
- Repository source-bundle SHA-256: `22ff1b48dcc32b00f3088353b503760f8718a40c614b6e9ec84d5bb0a6e28756`

## Import notes

- The original technical overview and instrument references are retained under `docs/`; the root README is the bilingual community landing page.
- The shared SDK-alpha engines are directly browsable in `sdk-alpha/`.
- The legacy 0.1.1 machine API, host compatibility layer, DSP test harness, build scripts and stock OS inspection tool are preserved in the compressed source bundle under `source-bundles/`. The verified extraction helper is `scripts/unpack_source_bundle.sh`.
- Generated host executables and rendered WAVs are deliberately excluded. Test runners recreate the WAV output.
- The Workbench and its static site are additional repository components.
- The stock OS 1.44 firmware image is not included. Structural fingerprints and hashes in `docs/VALIDATION.md` identify the local analysis input but do not grant redistribution rights.

The firmware source image remains a user-supplied local file. Never commit proprietary firmware or personal backups.
