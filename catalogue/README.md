# DN1-LAB module catalogue / Catalogue des modules

## Français

Le catalogue distingue trois objets qui ne doivent pas être confondus :

- `machines/` contient les trois machines shared SDK-alpha, source de vérité pour les identités 100–102, leurs cibles, le niveau de preuve et les capacités. Elles sont validées par `python3 tools/validate_catalogue.py`.
- `proposals/` contient les fiches de conception et d'antériorité des six directions de recherche VESPER, MYCEL, ORBITAL, LITHOS, PHOTON et ANIMA. Depuis 0.3.0-alpha, chaque piste possède aussi une implémentation expérimentale hôte sous `../prototypes/`.
- `../prototypes/catalogue.json` et les `prototype.json` décrivent les six prototypes hôte. Leurs IDs 103–108 sont provisoires et ne doivent pas être interprétés comme validés par le linker ou installables.

Les sources dans `prototypes/` restent hors du builder firmware. Tout passage vers `sdk-alpha/` nécessite validation du véritable SDK, mesure du coût ColdFire, lint/linkage combiné, validation des IDs et essais cible.

## English

The catalogue separates three different objects:

- `machines/` contains the three shared SDK-alpha machines and is the source of truth for IDs 100–102, targets, evidence level and capabilities. It is checked by `python3 tools/validate_catalogue.py`.
- `proposals/` contains design and prior-art notes for VESPER, MYCEL, ORBITAL, LITHOS, PHOTON and ANIMA. Since 0.3.0-alpha, each direction also has an experimental host implementation under `../prototypes/`.
- `../prototypes/catalogue.json` and the individual `prototype.json` files describe the six host prototypes. IDs 103–108 are provisional and are not linker-validated or installable.

Sources under `prototypes/` remain outside the firmware builder. Moving one into `sdk-alpha/` requires validation against the actual SDK, ColdFire profiling, combined lint/link, ID validation and target testing.
