# Pacman avec Qt

## Setup
Installer la police de caractère Arcadepix disponible dans le dossier data ou la télécharger : [Aracdepix](https://www.dafont.com/arcadepix.font).

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

Ne pas oublier d'insérer les bibliothèques dynamiques fmodex.dll 
                                                      Qt5Core.dll
                                                      Qt5Gui.dll
                                                      Qt5Widgets.dll
                                                      
# Compiler le programme
``` bash
# Se rendre dans le dossier contenant le Makefile avec le cmd
cd: chemin/vers/fichier/Makefile

# Compiler en executant
mingw32-make

# Lancer le Pacman 
pacman.exe
```
