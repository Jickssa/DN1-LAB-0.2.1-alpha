# Portage vers le shared machine SDK alpha

## Pourquoi conserver deux interfaces

DN1 LAB 0.1.1 repose sur struct dnm_machine et la table dn_machines du compagnon historique. L'interface partagée plus récente repose sur struct elek_machine, ELEK_REGISTER_MACHINE() et sdk_alpha_machines. Les deux ne sont pas interchangeables : descripteur, plages de paramètres, rendu, état des voix et emplacement des pages diffèrent.

Le dossier modules/ reste la branche 0.1.1 historique. sdk-alpha/ est une migration séparée qui vise le SDK documenté dans elekloader main et son port Digitone MKI OS 1.44. Aucun manifeste alpha ne prétend fonctionner sur le compagnon v0.1 du Shop.

## Contrat SDK ciblé

D'après le header et la documentation upstream consultés :
- struct elek_machine identifie la machine, son nom/nom court, jusqu'à quinze knobs, deux pages et une fonction render.
- ELEK_REGISTER_MACHINE(machine) exporte l'inscription ; le builder la retrouve et l'ajoute à la table du compagnon. Les mods SDK et leur compagnon compatible ABI doivent être construits ensemble.
- Chaque render reçoit une voix et écrit 32 échantillons mono à 48 kHz. Sur Digitone MKI, elek_sample est Q31 (int32_t), pas Q15 ni float.
- v->knob[i] est déjà mappé dans les plages déclarées par struct elek_knob. Ce ne sont plus des mots SYN bruts 8.8.
- v->state désigne 256 octets réservés à l'état de chaque voix ; v->start indique le premier bloc du nouveau déclenchement.
- elek_voice ne fournit pas le booléen gate de l'ancien dnm_voice. Ne pas lire ce champ ni inventer une détection note-off. Les couches continues restent calculées pendant le rendu ; l'enveloppe AMP native gère le relâchement.
- Les pages personnalisées du Digitone MKI sont présentées dans la zone SYN2 de la piste ; deux pages maximum, huit encodeurs maximum par page.

Limites à traiter comme des contrats : 15 knobs, deux pages, libellés d'encodeur de cinq caractères maximum, nom court de quatre caractères maximum, nom de machine de seize caractères maximum, état par voix ≤256 octets et ID machine 4–127.

## Migration des contrôles

L'ancienne interface exposait seize valeurs SYN logiques, huit SYN1 puis huit SYN2. Le SDK alpha déclare des paramètres indépendants avec une plage propre et n'en accepte que quinze. Les plages brutes du firmware OS 1.44 doivent donc être converties explicitement vers des valeurs logiques.

Les 15 paramètres de chaque port sont rangés dans une nouvelle table. Le harnais alpha contrôle chaque valeur initiale, balaie les notes et contrôle les états de voix indépendants.

### Contrôles retirés ou adaptés
- KARST retire le Q séparé ; la longue décroissance est réglée par DECY.
- MORPHANT retire LVL ; l'enveloppe AMP et le niveau natif déterminent l'amplitude. FINE est en cents et OCT en demi-tons.
- TESSERA retire DUST, qui doublonnait NOIS. DETUN est en cents. Le mixage emploie dn1_lerp_q16 pour éviter un dépassement entier.

## DSP entier et coût

phase_sine.h contient une table de 257 valeurs Q15 pour un quart de sinusoïde, avec interpolation sur 256 intervalles. Les tests hôte comparent un balayage déterministe de phases à sin() ; l'erreur maximale observée est 1,5326 unité Q15. Le helper comprend quantification par décalage, pliage à trois réflexions et calcul de vibrato signé.

Aucun rendu alpha ne fait d'allocation dynamique, d'accès disque, de logging ou d'appel UI. Les boucles par bloc sont bornées à 32 échantillons et les dimensions des boucles de résonateurs/partiels sont constantes. Le compteur de saturation est compilé uniquement dans les binaires de test. Cela ne prouve pas le budget CPU : les coûts ColdFire par voix et la charge de huit voix doivent être mesurés dans le vrai compagnon.

## Dépendances et construction

La matrice du SDK alpha consultée pour digitone-mk1 / 1.44 requiert les compagnons core et machines, avec le core Digitone requis à partir de 5.3. Le checkout upstream doit contenir le port 1.44 alpha du compagnon et les sources SDK partagées. Vérifier les versions réellement livrées.

Depuis la racine DN1 LAB :

    export ELEKLOADER_ROOT=/chemin/vers/elekloader
    export STOCK=/chemin/vers/Digitone_and_Digitone_Keys_OS1.44.syx
    export ELEKLOADER_CROSS=m68k-linux-gnu-
    ./scripts/build_digitone_alpha.sh

Le script construit core et compagnon, puis les trois machines ; il lance ensuite le lint combiné et la liaison. Il ne doit pas contourner une erreur de version, ressource, ABI ou zone de patch. Pour le runner hôte upstream : tests/run_alpha_upstream_host.sh.

## Références upstream
- https://github.com/elekloader/elekloader/blob/main/elekloader/sdk/include/elekloader/sdk_alpha_machine.h
- https://github.com/elekloader/elekloader/blob/main/docs/SDK.md
- https://github.com/elekloader/elekloader/blob/main/elekloader/sdk/sdk_alpha_build.py
- https://github.com/elekloader/elekloader/tree/main/mods/machines-dn1
- https://github.com/elekloader/elekloader/tree/main/mods/core-dn1
- https://github.com/elekloader/elekloader/blob/main/docs/TESTS.md
