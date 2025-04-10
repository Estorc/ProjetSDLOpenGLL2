# MiniJeu_Survie - Projet Chapper's Fallout

## Contexte

Ce projet est un dérivé du projet principal *Chapper's Fallout*, développé dans le cadre de la 2e année de Licence Informatique à l’Université du Mans. Le mini-jeu présenté ici n’a **pas vocation à être un jeu autonome**, mais devait initialement être **intégré dans l’environnement 3D principal** via un ordinateur interactif dans le bâtiment Claude Chappe.

Cependant, en raison de problèmes techniques survenus en fin de développement, l’intégration dans la scène principale n’a **pas pu aboutir à temps**. Pour cette raison, le mini-jeu a été placé dans une **branche dédiée du dépôt GitHub**, sous le nom de `Ekamyl`, et reste jouable **en standalone** via une exécution directe.

Mini-jeu réalisé par **Ekrem (Ekamyl)**.

## Mini-jeu : Survie face aux boules de feu

Le jeu se déroule dans une fausse interface système simulée, inspirée d’un bureau d’ordinateur. Dès le lancement, une animation initie un faux système d’exploitation, sur lequel une fenêtre s’ouvre pour laisser place au gameplay.

Le principe est simple :  
Vous incarnez un petit personnage qui doit **survivre pendant un temps donné** en esquivant des **boules de feu** qui tombent aléatoirement du ciel. Ce mini-jeu mêle réflexes, concentration et une ambiance légère soutenue par quelques **effets visuels et animations dynamiques**.

## Fonctionnement

Le mini-jeu utilise SDL2 et repose sur une **gestion complète des fenêtres et des événements**. Un `event manager` dédié capte les entrées clavier/souris en temps réel, et orchestre les transitions entre les différentes phases du jeu (lancement, gameplay, fin de partie).

Les textures, animations et interactions sont gérées via un système de spritesheets avec un compteur de frames, garantissant des animations fluides et cohérentes.

## Accès au mini-jeu

Le mini-jeu est accessible de deux façons :

- **En lançant le jeu principal** avec une **touche spéciale** (définie dans le code) qui permet de **sauter directement au mini-jeu** sans passer par l’environnement 3D.
- **Depuis la branche GitHub `Ekamyl`**, qui contient une version isolée et autonome du mini-jeu, destinée aux tests et démonstrations.

## Dépendances

Même environnement que le projet principal :

- SDL2
- SDL2 Image
- SDL2 TTF
- SDL2 Mixer

Installation (Debian) :
```sh
sudo apt install build-essential libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev
```

Compilation (unix systemes) :
```sh
make            # Compile le mini-jeu.
make run         # Compile et lance le mini-jeu.
make clear      # Nettoie les fichiers générés.
```
