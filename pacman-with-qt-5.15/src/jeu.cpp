#include <iostream>
#include <cassert>
#include <cmath>
#include <fstream>
#include <string>
#include "jeu.hpp"
#include "pacmanwindow.hpp"

using namespace std;

// Initialisation du jeu 
bool Jeu::init()
{
	int x, y;
	list<Fantome>::iterator itFantome;
	list<Dot>::iterator itDot;
	list<Energizer>::iterator itEnergizer;
	
	largeur = LARGEUR;
	hauteur = HAUTEUR;
	nbVie = NB_VIE;
	score = -10;
	nbDot = 0;
	timePower = 0;
	isCerise = false;
	eatenCerise = 0;

	// Chaque symbole correspond a un attribut different du terrain
	char terrain_original[HAUTEUR][LARGEUR+1];
	
	ifstream infile("./data/map/terrain.txt");
    if(infile.is_open()) {
        for(int i=0; i<HAUTEUR; i++) {
            string line;
            if (getline(infile, line))
                // Copie la ligne dans l'élément correspondant de la matrice
                strncpy(terrain_original[i], line.c_str(), LARGEUR+1);
        }
        infile.close();
    }
    else cout << "Erreur : impossible d'ouvrir le fichier terrain.txt" << endl;

	// Configuration du terrain 
	terrain = new Case[largeur*hauteur];
	for(y=0;y<hauteur;++y)
    {
        for(x=0;x<largeur;++x)
        {
            switch(terrain_original[y][x])
            {
                case '#':
                    terrain[y*largeur+x] = MUR_VERTICAL;
                    break;
                case '&':
                    terrain[y*largeur+x] = MUR_HORIZONTAL;
                    break;
                case '*':
                    terrain[y*largeur+x] = ARRONDI_DROIT;
                    break;
                case '/':
                    terrain[y*largeur+x] = ARRONDI_GAUCHE;
                    break;
                case '+':
                    terrain[y*largeur+x] = ARRONDI_HAUT;
                    break;
                case '-':
                    terrain[y*largeur+x] = ARRONDI_BAS;
                    break;
                case 'D':
                    terrain[y*largeur+x] = JONCTION_DROITE;
                    break;
                case 'G':
                    terrain[y*largeur+x] = JONCTION_GAUCHE;
                    break;
                case 'H':
                    terrain[y*largeur+x] = JONCTION_HAUTE;
                    break;
                case 'B':
                    terrain[y*largeur+x] = JONCTION_BASSE;
                    break;
                case '1':
                    terrain[y*largeur+x] = COIN_HAUT_GAUCHE;
                    break;
                case '2':
                    terrain[y*largeur+x] = COIN_HAUT_DROIT;
                    break;
                case '3':
                    terrain[y*largeur+x] = COIN_BAS_DROIT;
                    break;
                case '4':
                    terrain[y*largeur+x] = COIN_BAS_GAUCHE;
                    break;
                case 'S':
                    terrain[y*largeur+x] = SPAWN;
                    break;
                case 'I':
                    terrain[y*largeur+x] = INTERDIT;
                    break;
                case 'P':
                    terrain[y*largeur+x] = PASS;
                    break;
                default:
                    terrain[y*largeur+x] = VIDE;
					nbDot += 1; // On incrémente le nombre de pac-gomme (une pour chaque case vide)
            }
        }
    }
	
	// Creation des fantomes
    fantomes.resize(NB_FANTOME);
	for (itFantome=fantomes.begin(); itFantome!=fantomes.end(); itFantome++)
    {
        do {
            x = rand()%largeur;
            y = rand()%hauteur;
        } while (terrain[y*largeur+x]!=SPAWN); // On veut une position aleatoire dans le spawn

        itFantome->posX = x;
        itFantome->posY = y;
		itFantome->pass = false;
        itFantome->dir = (Direction)(rand()%4); // Direction aleatoire 
    }
	
	// Creation des super pac-gommes
	energizers.resize(4);
	int compteur = 1;
	for(itEnergizer=energizers.begin(); itEnergizer!=energizers.end(); itEnergizer++)
	{
		switch(compteur) // Un super pac-gomme dans chaque "coin"
        {
            case 1:
                itEnergizer->posX = 1;
                itEnergizer->posY = 3;
                break;
		
			case 2:
                itEnergizer->posX = 1;
                itEnergizer->posY = 15;
                break;
		
			case 3:
                itEnergizer->posX = 25;
                itEnergizer->posY = 3;
                break;
                    
			case 4:
                itEnergizer->posX = 25;
                itEnergizer->posY = 15;
		}
		compteur++;
	}	
	
	// Creation des pac-gommes
	dots.resize(nbDot);
	itDot=dots.begin();
    do
    {
        for(y=0;y<hauteur;y++)
            for(x=0;x<largeur;x++)
            {
                if (terrain[y*largeur+x] == VIDE) // Un pac-gomme sur chaque case vide
                {
                    itDot->posX = x;
                    itDot->posY = y;
                    itDot++;
                }
            }
    }
    while(itDot!=dots.end());
	
	// Position initiale du Pacman
    posPacmanX = POSX_init;
	posPacmanY = POSY_init;

    return true;
}

// Evolution du cours du jeu 
void Jeu::evolue()
{
	if(getNbVie() > 0){ // Si Pacman est encore en vie
		int testX, testY;
		list<Fantome>::iterator itFantome;
		list<Dot>::iterator itDot;
		list<Energizer>::iterator itEnergizer;
		
		// Gestion des super pac-gommes
		for (itEnergizer=energizers.begin(); itEnergizer!=energizers.end(); itEnergizer++)
        {
            testY = itEnergizer->posY;
            testX = itEnergizer->posX;
            if (testY==posPacmanY&&testX==posPacmanX) // Si Pacman mange une super pac-gomme
            {
                score += 50 - 10; // On augmente le score de 50 (on enleve 10 car pac-gomme en dessous)
                energizers.erase(itEnergizer); // On supprime le super pac-gomme
				timePower = POWERTIME; eatenPower = 0; // On declenche le timer qui définit la duree de peur des fantomes
													   // eatenPower représente le nb de fantomes mange pendant ce timer
				for (itFantome=fantomes.begin(); itFantome!=fantomes.end(); itFantome++){
					itFantome->fear = true; // On change l'etat des fantomes
				}
                break;
            }
        }
		
		// Gestion des pac-gommes
		for (itDot=dots.begin(); itDot!=dots.end(); itDot++)
        {
            testY = itDot->posY;
            testX = itDot->posX;
            if (testX==posPacmanX&&testY==posPacmanY) // Si Pacman mange une pac-gomme
            {
                score += 10; // On augmente le score de 10
				nbDot -= 1; // Il y a un pac-gomme de moins sur le terrain
                dots.erase(itDot); // On supprime le pac-gomme
                break;
            }
        }
		
		// Gestion du bonus fruit 
		if(rand()%Proba_cerise==0 && !isCerise){ // 1 chance sur la proba pour chaque frame du programme de voir apparaitre un bonus fruit
			isCerise = true; // On affiche une cerise 
		}
		if(isCerise && posPacmanX == POSX_FRUIT && posPacmanY == POSY_FRUIT){ // On verifie si Pacman mange la cerise
			isCerise = false; // On enleve la cerise
			eatenCerise++; // On incremente le nb de cerise mange
			score += 100; // On augmente le score de 100
		}
			
		collision(); // On verifie la collision entre Pacman et un fantome
	}
}

// Gestion du deplacement des fantomes
void Jeu::moveGhost(){
	int testX, testY;
	list<Fantome>::iterator itFantome;
	
	int depX[] = {-1, 1, 0, 0};
	int depY[] = {0, 0, -1, 1};
		
	for (itFantome=fantomes.begin(); itFantome!=fantomes.end(); itFantome++){
		for(int i=0; i<3; i++){
			if(itFantome->posX == 12+i && itFantome->posY == 9) // Donne la possibilite de changer de direction dans le spawn sans rencontrer un mur
				itFantome->dir = (Direction)(rand()%4);
		}

		testX = itFantome->posX + depX[itFantome->dir];
		testY = itFantome->posY + depY[itFantome->dir];

		if(terrain[testY*largeur+testX] == PASS && itFantome->pass == false){ // Si le fantome veut sortir du spawn
			itFantome->posX = testX;
			itFantome->posY = testY;
			itFantome->pass = true; // Fantome ne pourra plus rentrer dans le spawn
		}
		else if (terrain[testY*largeur+testX] == VIDE || terrain[testY*largeur+testX] == SPAWN){ // Fantome se deplace si direction valide
			itFantome->posX = testX;
			itFantome->posY = testY;
		}
		else
			// Changement de direction
			itFantome->dir = (Direction)(rand()%4);
	}
}

// Gestion du deplacement du Pacman
Direction Jeu::deplacePacman(Direction dir)
{
	static Direction actualDir; // Variable pour garder en memoire la direction actuelle 
    int depX[] = {-1, 1, 0, 0, 0};
    int depY[] = {0, 0, -1, 1, 0};
    int testX, testY;
	
    testX = posPacmanX + depX[dir];
    testY = posPacmanY + depY[dir];

    if(posPacmanX == 0 && posPacmanY == 9 && dir == GAUCHE) // Pour se teleporter de gauche a droite
    {
        posPacmanX = 26;
        posPacmanY = 9;
		actualDir = GAUCHE;
    }
    else if(posPacmanX == 26 && posPacmanY == 9 && dir == DROITE) // Pour se teleporter de droite a gauche 
    {
        posPacmanX = 0;
        posPacmanY = 9;
		actualDir = DROITE;
    }
    else if (posValide(testX, testY)) // Si position valide, le pacman peut se deplacer dans la direction voulue par le joueur 
    {								  // Des que la position sera valide, le pacman changera de position dans la direction voulue
        posPacmanX = testX;
        posPacmanY = testY;
		actualDir = dir;
    }
    else 
	{
		testX = posPacmanX + depX[actualDir];
		testY = posPacmanY + depY[actualDir];
		if (posValide(testX, testY)){ // Sinon on continue toujours dans la direction actuelle si possible 
			posPacmanX = testX;
			posPacmanY = testY;
		}
	}
	return actualDir;
}

// Collision entre Pacman et Fantôme
void Jeu::collision()
{
    list<Fantome>::iterator itFantome;

    for(itFantome = fantomes.begin(); itFantome != fantomes.end(); itFantome++) {
        if(posPacmanX == itFantome->posX && posPacmanY == itFantome->posY){
			if(timePower>0 && itFantome->fear == true){ // Si Pacman a le Power-up et que le fantome a peur 
				eatenPower++; // Le nb de fantome mange pdt le power-up augmente 
				score += 100*pow(2,eatenPower); // On augmente le score de 200/400/800/1600
				eatGhost = true; // On signale que Pacman vient de manger un fantome
				itFantome->posX = 12; // On remet le fantome mange dans le spawn
				itFantome->posY = 9;
				itFantome->pass = false; // Il a de nouveau l'autorisation de sortir du spawn
				itFantome->fear = false; // Il n'a plus peur
                break;
			}
			else deadPacman(); // Si le Pacman n'a pas de power-up, il meurt
		}
    }
	
	if(timePower > 0) // Timer de 10 secondes environ (power-up)
		timePower -= 1; // On decremente le timer 
	else{
		for(itFantome = fantomes.begin(); itFantome != fantomes.end(); itFantome++){
			itFantome->fear = false; // Fantome n'ont plus peur quand le power-up est terminé 
		}
	}
}

// Gestion de la mort du Pacman
void Jeu::deadPacman()
{
	nbVie -= 1; // On enleve une vie au Pacman
	posPacmanX = POSX_init; // On le remet a sa position initiale
	posPacmanY = POSY_init; 
	
	list<Fantome>::iterator itFantome;
    for(itFantome = fantomes.begin(); itFantome != fantomes.end(); itFantome++) 
		itFantome->fear == false; // Les fantomes redeviennent normaux si certains avaient encore peur 
	timePower = 0;
}

// Verifie si une position est valide sur le terrain 
bool Jeu::posValide(int x, int y) const
{
    return (x>=0 && x<largeur && y>=0 && y<hauteur && terrain[y*largeur+x]==VIDE);
}

//------------CONSTRUCTEURS.DESTRUCTEURS------------//

// Constructeur de la classe Jeu
Jeu::Jeu()
{
    terrain = nullptr;
    largeur = 0; hauteur = 0;
    posPacmanX = 0; posPacmanY = 0;
	nbVie = 0; score = 0;
	timePower = 0;
	isCerise = false;
	eatenCerise = 0;
}

// Destructeur de la classe Jeu
Jeu::~Jeu()
{
    if (terrain!=nullptr)
        delete[] terrain;
}

// Constructeur de la classe Fantome 
Fantome::Fantome()
{
    posX = 0; posY = 0;
    dir = DROITE; // direction par defaut
	pass = false; // n'est pas encore sorti du spawn
}

// Constructeur de la classe Pac-gomme
Dot::Dot()
{
    posX = 0; posY = 0;
}

// Constructeur de la classe Super Pac-gomme
Energizer::Energizer()
{
    posX = 0; posY = 0;
}


//------------ACCESSEURS.MUTATEURS------------//

// Accesseur de l'abscisse d'un fantome
int Fantome::getPosX() const
{
    return posX;
}

// Accesseur de l'ordonnee d'un fantome
int Fantome::getPosY() const
{
    return posY;
}

// Accesseur de l'etat du fantome (peur ou non)
bool Fantome::getFear() const
{
	return fear;
}

// Accesseur de la direction d'un fantome
Direction Fantome::getDirection() const
{
    return dir;
}

// Accesseur de l'abscisse d'un pac-gomme
int Dot::getPosX() const
{
    return posX;
}

// Accesseur de l'ordonee d'un pac-gomme
int Dot::getPosY() const
{
    return posY;
}

// Accesseur de l'abscisse d'un super pac-gomme
int Energizer::getPosX() const
{
    return posX;
}

// Accesseur de l'ordonnee d'un super pac-gomme
int Energizer::getPosY() const
{
    return posY;
}

// Accesseur du nombre de cases dans le terrain en abscisse 
int Jeu::getNbCasesX() const
{
    return largeur;
}

// Accesseur du nombre de cases dans le terrain en ordonnee 
int Jeu::getNbCasesY() const
{
    return hauteur;
}

// Accesseur de la position du Pacman en abscisse
int Jeu::getPacmanX() const
{
    return posPacmanX;
}

// Accesseur de la position du Pacman en ordonnee
int Jeu::getPacmanY() const
{
    return posPacmanY;
}

// Accesseur du nombre de vie restant du Pacman
int Jeu::getNbVie() const
{
    return nbVie;
}

// Mutateur du nombre de vie du Pacman
void Jeu::setNbVie(int vie)
{
    nbVie=vie;
}

// Accesseur du score actuel du joueur
int Jeu::getScore() const
{
	return score;
}

// Mutateur du score le plus eleve 
void Jeu::setHighscores()
{
    int scoreActuel = getScore();

    if(nbVie <= 0)
    {
        ifstream fichierLecture("./data/highscores.txt");
        int highscores;

        if (fichierLecture.is_open()) 
        {
            fichierLecture >> highscores;

            if(scoreActuel > highscores) // Si le joueur a battu le record
            {
                ofstream fichierEcriture("./data/highscores.txt");
                fichierEcriture << scoreActuel << endl; // On ecrit dans le fichier le nouveau score
                fichierEcriture.close();
            }

            fichierLecture.close();
        } 
        else 
        {
            cout << "Impossible d'ouvrir le fichier !" << endl;
        }
    }
}

// Accesseur du score le plus eleve
int Jeu::getHighscores()
{
    ifstream fichierLecture("./data/highscores.txt");
    int highscores;

    if (fichierLecture.is_open()) 
    {
        fichierLecture >> highscores;
    } 
    else 
    {
        cout << "Impossible d'ouvrir le fichier !" << endl;
    }

    fichierLecture.close();

    return highscores;
}

// Accesseur du nombre de pac-gomme
int Jeu::getNbDot() const
{
	return nbDot;
}

// Accesseur du timer du power-up de Pacman
int Jeu::getPowerTime() const
{ 
	return timePower;
}

// Accesseur pour savoir si une cerise est sur le terrain
bool Jeu::getCerise() const
{ 
	return isCerise;
}

// Accesseur pour connaitre le nb de cerises mange
int Jeu::getEatenCerise() const
{
	return eatenCerise;
}

// Accesseur pour savoir si Pacman a mange un fantome
bool Jeu::getEatGhost() const
{
	return eatGhost;
}

// Mutateur pour reset la variable eatGhost
void Jeu::setEatGhost(bool setghost)
{
	eatGhost = setghost;
}

// Accesseur qui retourne la case du terrain selon position
Case Jeu::getCase(int x, int y) const
{
    assert(x>=0 && x<largeur && y>=0 && y<hauteur);
    return terrain[y*largeur+x];
}

// Accesseur de la liste de fantomes
const std::list<Fantome> &Jeu::getFantomes() const
{
    return fantomes;
}

// Accesseur de la liste de pac-gommes
const std::list<Dot> &Jeu::getDots() const
{
	return dots;
}

// Accesseur de la liste de super pac-gommes
const std::list<Energizer> &Jeu::getEnergizers() const
{
	return energizers;
}