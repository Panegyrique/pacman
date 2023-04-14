# Pacman avec Qt

## Setup
Installer la police de caractère Arcadepix disponible dans le dossier data ou la télécharger : [Arcadepix](https://www.dafont.com/arcadepix.font).

## Arborescence du répertoire
``` bash
∟ pacman-with-qt-5.15
   ∟ data
   ∟ include
   ∟ obj
   ∟ platforms
   ∟ src 
   Makefile
∟ qtbase-small-5.15.2-x64
   ∟ include 
   ∟ lib
   ∟ src
∟ FMOD 
   ∟ include 
   ∟ lib
```

Ne pas oublier d'insérer les bibliothèques dynamiques 
``` bash
fmodex.dll 
Qt5Core.dll
Qt5Gui.dll
Qt5Widgets.dll
 ```
 
## Lancer le jeu
``` bash
# Ouvrir le cmd et se rendre dans le dossier contenant le Makefile
cd: chemin/vers/fichier/Makefile

# Compiler en executant
mingw32-make

# Lancer le Pacman 
pacman.exe
```

Maxence Goubaud & Mathias Amichaud
