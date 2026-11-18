# Workbench / Atelier de préparation

## État implémenté dans ce dépôt

Le Workbench statique est livré dans `site/workbench/`. Il permet :

- de sélectionner KARST, MORPHANT et TESSERA (0.2.2-alpha) ;
- d'afficher leur cible Digitone MKI OS 1.44 et leur niveau de preuve L2 ;
- de charger un fichier firmware dans le navigateur uniquement et calculer son SHA-256 ;
- de comparer ce hash au profil stock OS 1.44 documenté ;
- d'exporter un manifeste JSON de préparation listant cible, modules, version et état de validation.

Les rendus audio WAV de référence sont conservés dans l'archive source DN1-LAB 0.2.2-alpha ; le site Workbench actuel ne propose pas encore de lecteur audio intégré.

Le navigateur ne transmet pas le firmware. Le Workbench ne parse pas encore complètement le conteneur ELE3, ne résout pas les dépendances upstream, ne compile pas, ne patch pas et ne flashe aucun fichier. Le JSON exporté est un plan, pas un firmware.

## Choix d'architecture

DN1-LAB construit une interface de préparation modulaire, complémentaire à elekloader, plutôt qu'une copie de son interface ou de son service. Le lien vers le Mod Shop et le dépôt elekloader reste disponible pour les fonctions qu'il fournit déjà.

## Parcours utilisateur actuel

1. Ouvrir le Workbench, choisir un ou plusieurs moteurs source.
2. Confirmer la cible Digitone MKI / OS 1.44.
3. Facultativement sélectionner le fichier stock local pour calculer et vérifier son empreinte connue.
4. Examiner le résumé et exporter le plan JSON.
5. Utiliser les dépôts et outils upstream appropriés pour les opérations non fournies par cette interface.

## Étapes suivantes

**Préflight V2 :** parseur en lecture seule du conteneur ELE3, validation stricte de la cible, résolution du graphe de dépendances, contrôles de collisions et limites du build combiné.

**Builder V3 :** builder SDK versionné, environnement isolé, intégration du core et du compagnon Digitone OS 1.44, lint combiné, patcher, vérification déterministe, rapport de provenance et validation par émulateur. L'interface ne doit afficher cette fonction qu'une fois les prérequis et tests effectivement disponibles.
