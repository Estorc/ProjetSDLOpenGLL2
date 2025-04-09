# ProjetSDLOpenGLL2

## Context

This project is a group assignment for the 2nd year of the Computer Science degree at the University of Le Mans. The objective is to create a game coded exclusively in C, with only SDL/OpenGL allowed (no pre-existing game engines like Unity).
The initial project duration set by the professors is approximately two months, starting on January 26th and ending in late April. However, we have taken the initiative to start early, working on elements such as the 3D engine, models, and textures.

## Team

- Loup (Estorc)
- Lucien (Mimar)
- Ekrem (Ekamyl)
- Medhi (MH-le-D2)

## Project Overview

*Chapper's Fallout* is a 3D horror game set within the Claude Chappe Institute. In this game, you step into a meticulously recreated environment where every shadow hides a potential threat. As you navigate the darkened hallways and sprawling rooms of the institute, your primary objectives are to solve a series of 2D mini-games displayed on in-game computers and to evade a furious, menacing creature that is going after you. The mini-games provide tools for repairing a malfunctioning AI that plays a central role in the game.

The story unfolds as you explore various sections of the institute. Early in the game, you receive a message from a student, Souleymane, who urgently requests your assistance with a practical assignment in one of the institute’s buildings. Accepting his request sets off a chain of events that lead you deeper into the mystery of the institute, where each mini-game you complete advances the plot. The gameplay seamlessly combines exploration, puzzle-solving, and survival elements to create a tense and immersive experience.

### Controls & Game Start

At the start of the game, a brief tutorial introduces you to the essential controls and gameplay mechanics to ensure you’re well-prepared for the challenges ahead. You can use either the ZQSD keys or the arrow keys to move around, with the SPACEBAR dedicated to jumping over obstacles. Additionally, a flashlight tool is available to illuminate dark areas with the F key, which is crucial for both exploration and avoiding hazards. The tutorial guides you through the initial exploration of the institute, explaining how to interact with in-game computers to launch mini-games and how to respond to the various notifications that appear on-screen. This setup ensures that you are never left guessing what to do next, allowing you to dive straight into the adventure of Chapper's Fallout.

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
make test       # Build tout les fichiers de test dans le repertoire bin avec un fichier
                # test global pour exécuté tout les fichiers tests d'une traite 
make            # Construit le projet avec la configuration par défaut.
make clear      # Efface tous les fichiers du build.
make debug      # Construit le projet avec la configuration de déboggage.
```

## GIT
### Large git
The .a libs use large git : https://git-lfs.github.com

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

