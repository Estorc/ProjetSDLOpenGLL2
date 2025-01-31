# ProjetSDLOpenGLL2

## Contexte :

- Ce projet est un travail de groupe dans le cadre de la 2ème année de licence Informatique de l'université du Mans, consistant à créer un jeu, en codant exclusivement en langage C, avec SDL/OPENGL d'autorisés seulement (pas de moteurs graphiques déjà existants tel que Unity par exemple). Nous avons décidé de créer un jeu d'horreur en 3D (voir description ci-dessous).
- La durée initiale du projet donnée par les professeurs est d'environ deux mois, commençant le 26 janvier, terminant fin avril. Mais nous avons prit de l'avance en commençant de notre côté plusieurs choses, tel que le moteur 3D ainsi que tous les modèles 3D et textures. 

## Equipe :

- Loup (Estorc)
- Lucien (Mimar)
- Ekrem (Ekamyl)
- Medhi (MH-le-D2)

## Présentation du projet :

- Claude Chappe : L’Infernale Malédiction des Ombres Silencieuses, est un jeu d'horreur en première personne prenant place au bâtiment informatique Claude Chappe de la ville du Mans, ou les terrifiants professeurs tentent d'empêcher le joueur de collecter les bonnes notes qui permettront au joueur de passer sa licence, et peut-être même son MASTER (y arriverez vous ????) (impossible).

- Le personnage doit récupérer les notes câchés à plusieurs endroits en se déplaçant dans le bâtiment, tout en essayant de survivre au(x) monstre(s) présent(s).
- Le jeu est en 3D, affiché grâce à un moteur 3D basé sur OPENGL, codé en C.
- Les modèles et textures sont modélisés sur Blender.
- Les musiques et bruitages audios sont créés sur FL STUDIO 21.

## Librairies
- KDialog
- SDL2
- SDL2 Image
- SDL2 TTF

## Installation
### Dependencies
Debian based distro: ``sudo apt install build-essential kdialog libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev``
Arch based distro: ``pacman -S --needed base-devel kdialog sdl2 sdl2_image sdl2_ttf sdl2_mixer``
### Clone repository
```sh
cd <EMPLACEMENT_DU_PROJET>
git clone https://github.com/Estorc/ProjetSDLOpenGLL2.git
```

## Compilation
```
make            # Construit le projet avec la configuration par défaut.
make debug      # Construit le projet avec la configuration de déboggage.
make tools      # Construit les outils de compilation.
```

## GIT
### Récupérer les changements :
```git
git pull                # Permet de récupérer les dernières mises à jour du projet.
```
### Envoyer les changements :
```git
git commit -a           # Prépare le commit (utilisez "git commit -am uncommented" pour passer le commentaire).
git commit -m           # Prépare le commit (utilisez "git commit -am uncommented" pour passer le commentaire).
git push -u origin main # Permet d'envoyer les changements locaux au git.
```

