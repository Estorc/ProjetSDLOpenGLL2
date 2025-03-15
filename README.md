# ProjetSDLOpenGLL2

## Context

- This project is a group assignment for the 2nd year of the Computer Science degree at the University of Le Mans. The objective is to create a game coded exclusively in C, with only SDL/OpenGL allowed (no pre-existing game engines like Unity).
- The initial project duration set by the professors is approximately two months, starting on January 26th and ending in late April. However, we have taken the initiative to start early, working on elements such as the 3D engine, models, and textures.

## Team

- Loup (Estorc)
- Lucien (Mimar)
- Ekrem (Ekamyl)
- Medhi (MH-le-D2)

## Project Overview

- Claude Chappe: The Infernal Curse of the Silent Shadows is a first-person horror game set in the Claude Chappe computer science building in Le Mans. In this eerie setting, terrifying professors attempt to prevent the player from collecting the right grades, which are necessary to pass their degree—and maybe even their Master’s! (Will you succeed?? Impossible).
- The player must find hidden grades scattered throughout the building while evading the lurking monsters.
- The game is 3D, displayed through a custom 3D engine based on OpenGL, fully coded in C. It was strongly inspired by Godot.
- 3D models and textures are created in Blender.
- Music and sound effects are composed using FL Studio 21.

## Gantt
https://docs.google.com/spreadsheets/d/13zt-Oj6qx9QxYYYmqX78YSamAOPGoE6hBWrT9ppttbs/edit?usp=sharing

## Documentation
https://estorc.github.io/ProjetSDLOpenGLL2/html/

## Librairies
- SDL2
- SDL2 Image
- SDL2 TTF
- SDL2 Mixer
- OpenGL

## Installation
### Dependencies
Debian based distro: ``sudo apt install build-essential libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev python3 python3-venv python3-pip libglu1-mesa-dev``


Arch based distro: ``pacman -S --needed base-devel kdialog sdl2 sdl2_image sdl2_ttf sdl2_mixer``


MSYS2 based (Windows): ``pacman -S make mingw-w64-x86_64-python pacman -S mingw-w64-x86_64-python-pip mingw-w64-x86_64-SDL2 mingw-w64-x86_64-SDL2_gfx mingw-w64-x86_64-SDL2_ttf mingw-w64-x86_64-SDL2_image mingw-w64-x86_64-SDL2_mixer mingw-w64-x86_64-glew mingw-w64-x86_64-freeglut rsync``

## Compilation
```
make install    # Installe toutes les librairies nécessaires.
make tools      # Construit les outils de compilation.
make            # Construit le projet avec la configuration par défaut.
make clear      # Efface tous les fichiers du build.
make debug      # Construit le projet avec la configuration de déboggage.
```

## GIT
### Clone repository
```sh
cd <EMPLACEMENT_DU_PROJET>
git clone https://github.com/Estorc/ProjetSDLOpenGLL2.git
```

### Récupérer les changements
```git
git pull                # Permet de récupérer les dernières mises à jour du projet.
```
### Envoyer les changements
```git
git commit -a           # Prépare le commit.
git push -u origin main # Permet d'envoyer les changements locaux au git.
```

