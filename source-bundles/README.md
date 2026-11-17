# DN1-LAB source bundle / Archive source

The SDK-alpha machine engines are directly browsable under `sdk-alpha/`. This compressed archive preserves the original host harness, test compatibility layer, legacy 0.1.1 machine API branch, build scripts and stock OS inspection tool without expanding dozens of files into the top-level Git tree.

Extract it from the repository root using:

    ./scripts/unpack_source_bundle.sh

The archive is versioned with the SHA-256 recorded in `SHA256SUMS.txt`. The extraction script verifies that digest, checks archive paths, and only then extracts the contents. It does not contain proprietary firmware, generated executables or generated WAV renders.

The archive is a source distribution, not a prebuilt module. Build output and target firmware generation still require the matching upstream SDK, companion and ColdFire toolchain.
