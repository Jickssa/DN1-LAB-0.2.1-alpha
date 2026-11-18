# GitHub repository metadata / Métadonnées GitHub

This file records the recommended public-facing GitHub repository fields. GitHub's repository "About" fields are managed in the web interface; they are not changed by commits to this repository.

## Recommended About description

**English (recommended, concise):**

> Experimental synthesis engines and DSP research for Elektron Digitone MKI: 3 shared SDK-alpha machines, 6 host-tested DSP prototypes, validation tools and a local-first Workbench. Not hardware-certified.

**Français (alternative):**

> Recherche en synthèse expérimentale et DSP pour Elektron Digitone MKI : 3 machines SDK alpha, 6 prototypes hôte, outils de validation et Workbench local. Non certifié matériel.

## Recommended homepage

`https://jickssa.github.io/DN1-LAB-0.2.1-alpha/`

Set this only after GitHub Pages has been enabled and the deployment URL has been confirmed.

## Recommended topics

`digitone`, `digitone-mk1`, `elektron`, `synthesizer`, `sound-synthesis`, `audio-dsp`, `digital-signal-processing`, `embedded-audio`, `c`, `firmware-research`, `music-technology`, `modal-synthesis`, `additive-synthesis`, `phase-modulation`

Topics should describe the repository's actual content. Do not add unrelated tags merely to increase search visibility.

## Repository presentation checklist

1. Set the About description and topics on the repository page.
2. Set the homepage only after Pages deployment works.
3. Enable Discussions if the project will accept community discussions.
4. Enable private vulnerability reporting in Settings > Code security and analysis if maintainers can monitor it.
5. Configure GitHub Pages to deploy from the Pages Actions workflow.
6. Confirm that the default branch is the intended stable branch. Keep the feature branch PR unmerged until target build gates are met.
7. Add a social preview image only when a project-specific asset is prepared and the branding has been reviewed.

## What this repository already contains

- Root README in French and English.
- Technical overview, getting-started guide, machine guides and prototype research.
- Validation reports, roadmap, changelog and firmware-pipeline notes.
- Contributing guide, Support, FAQ, Governance, Code of Conduct and Security policy.
- Issue forms, Discussion idea form and Pull Request template.
- Static landing page and Workbench under `site/`.

## Manual settings that code cannot apply

The connected GitHub actions available for this task can commit files and update the open Pull Request, but they do not expose write operations for the repository About description, topics, Discussion feature toggle or Pages settings. Those fields must be changed in GitHub's repository Settings / About interface.
