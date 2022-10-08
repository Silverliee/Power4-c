# Projet: Tartiflette aux Epinards
Projet puissance C du groupe 22 :
- Traore Mohamed Seydou
- Jules Mulet
- Willemain Damien
- Bonjour Gabriel

## 📁 Comment Compiler notre projet (Parce que c'est notre projet !!!!)
> importer le projet dans CLION puis Maj + F10 ou le petit bouton "►" en vert en haut à droite
### Ou
> gcc main.c -o tartiflette_aux_epinards.exe pour Linux
### Ou console sous windows
> cmake -S /chemin/du/projet -B /chemin/du/projet/build
> cmake --build /chemin/du/projet/build --target nomDeLexecutable
## 🔧 Fonctionnalités
On a fait :
- Puissance X 
- Taille de grille variable
- Fichier de configuration (dans le dossier code)
- Multijoueur local
## 👀 Remarques
- Alors on a la gestion des couleurs, mais elle est simplement commenté parce que windows aime pas vraiment;
- On a aussi le refresh de la grille "on supprime le contenu du terminal puis on reaffiche" mais on a commenté because of windows;
- Pour le fichier de param.... Il était tard, on tenait à le faire, mais du coup, il n'est pas intuitif, c'est dans l'ordre :
  - row
  - column
  - player
  - win
- on a essayé de se chauffer pour le "online".... puis on a remarqué que le nombre de mot à la minute sur discord était en dessous de 3~4