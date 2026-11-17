# DN1-LAB module catalogue / Catalogue de modules

The machine records in `machines/` are the catalogue source of truth for identity, targets, evidence level and capabilities. They are validated against the SDK module descriptors and C machine IDs by `python3 tools/validate_catalogue.py`.

Les fiches de `machines/` constituent la référence du catalogue pour l'identité, les cibles, le niveau de preuve et les capacités. Elles sont comparées aux manifestes SDK et aux IDs C par `python3 tools/validate_catalogue.py`.

`proposals/` contains exploratory concepts. A proposal is not a module and must not appear as selectable/installable firmware until an implementation passes the validation gates.
