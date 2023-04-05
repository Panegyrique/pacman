#include <iostream>
#include <cassert>
#include <cmath>
#include "jeu.hpp"

using namespace std;

Fantome::Fantome()
{
    posX = 0; posY = 0;
    dir = DROITE;
	pass = false;
}

int Fantome::getPosX() const
{
    return posX;
}

int Fantome::getPosY() const
{
    return posY;
}

Dot::Dot()
{
    posX = 0; posY = 0;
}

int Dot::getPosX() const
{
    return posX;
}

int Dot::getPosY() const
{
    return posY;
}

Energizer::Energizer()
{
    posX = 0; posY = 0;
}

int Energizer::getPosX() const
{
    return posX;
}

int Energizer::getPosY() const
{
    return posY;
}

Jeu::Jeu()
{
    terrain = nullptr;
    largeur = 0; hauteur = 0;
    posPacmanX = 0; posPacmanY = 0;
	nbVie = 0; score = 0;
	timePower = 0;
}

Jeu::Jeu(const Jeu &jeu):fantomes(jeu.fantomes), dots(jeu.dots), energizers(jeu.energizers)
{
    largeur = jeu.largeur;
    hauteur = jeu.hauteur;
    posPacmanX = jeu.posPacmanX;
    posPacmanY = jeu.posPacmanY;
	nbVie = jeu.nbVie;
	score = jeu.score;
	timePower = jeu.timePower;
	
    if (jeu.terrain!=nullptr)
    {
        terrain = new Case[largeur*hauteur];
        for (int c=0; c<largeur*hauteur; c++)
            terrain[c] = jeu.terrain[c];
    }
    else
        terrain = nullptr;
}

Jeu::~Jeu()
{
    if (terrain!=nullptr)
        delete[] terrain;
}

Jeu &Jeu::operator=(const Jeu &jeu)
{
    if (terrain!=nullptr)
        delete[] terrain;

    largeur = jeu.largeur;
    hauteur = jeu.hauteur;
    posPacmanX = jeu.posPacmanX;
    posPacmanY = jeu.posPacmanY;
	nbVie = jeu.nbVie;
	score = jeu.score;
	timePower = jeu.timePower;
    fantomes = jeu.fantomes;
	dots = jeu.dots;
	energizers = jeu.energizers;

    if (jeu.terrain!=nullptr)
    {
        terrain = new Case[largeur*hauteur];
        for (int c=0; c<largeur*hauteur; c++)
            terrain[c] = jeu.terrain[c];
    }
    else
        terrain = nullptr;
    return *this;
}

bool Jeu::init()
{
	int x, y;
	list<Fantome>::iterator itFantome;
	list<Dot>::iterator itDot;
	list<Energizer>::iterator itEnergizer;
	
	largeur = 27;
	hauteur = 19;
	nbVie = 3;
	score = 0;
	nbDot = 0;
	timePower = 0;

	const char terrain_original[19][28] = {
		"1&&&&&&&&&&&&B&&&&&&&&&&&&2",
		"#............#............#",
		"#./&&*./&&&*.-./&&&*./&&*.#",
		"#.........................#",
		"#./&&*.+./&&&B&&&*.+./&&*.#",
		"#......#.....#.....#......#",
		"4&&&&2.4&&*..-../&&3.1&&&&3",
		"IIIII#...............#IIIII",
		"&&&&&3...1&*PPP/&2...4&&&&&",
		".........#SSSSSSS#.........",
		"&&&&&2...4&&&&&&&3...1&&&&&",
		"IIIII#...............#IIIII",
		"1&&&&3.1&&*..+../&&2.4&&&&2",
		"#......#.....#.....#......#",
		"#./&&*.-./&&&H&&&*.-./&&*.#",
		"#.........................#",
		"#./&&*./&&&*.+./&&&*./&&*.#",
		"#............#............#",
        "4&&&&&&&&&&&&H&&&&&&&&&&&&3"
    };

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
					nbDot += 1;
            }
        }
    }
			
    fantomes.resize(4);
	for (itFantome=fantomes.begin(); itFantome!=fantomes.end(); itFantome++)
    {
        do {
            x = rand()%largeur;
            y = rand()%hauteur;
        } while (terrain[y*largeur+x]!=SPAWN);

        itFantome->posX = x;
        itFantome->posY = y;
        itFantome->pass = false;
        itFantome->dir = (Direction)(rand()%4);
    }
	
	energizers.resize(4);
	int compteur = 1;
	for(itEnergizer=energizers.begin(); itEnergizer!=energizers.end(); itEnergizer++)
	{
		switch(compteur){
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
	
	dots.resize(nbDot);
	itDot=dots.begin();
    do
    {
        for(y=0;y<hauteur;y++)
            for(x=0;x<largeur;x++)
            {
                if (terrain[y*largeur+x] == VIDE)
                {
                    itDot->posX = x;
                    itDot->posY = y;
                    itDot++;
                }
            }
    }
    while(itDot!=dots.end());
	
    do {
        x = rand()%largeur;
        y = rand()%hauteur;
    } while (terrain[y*largeur+x]!=VIDE);

    posPacmanX = x,
    posPacmanY = y;

    return true;
}

void Jeu::evolue()
{
	if(getNbVie() > 0){
		int testX, testY;
		list<Fantome>::iterator itFantome;
		list<Dot>::iterator itDot;
		list<Energizer>::iterator itEnergizer;

		int depX[] = {-1, 1, 0, 0};
		int depY[] = {0, 0, -1, 1};
		
		// Gestion des super pac-gommes
		for (itEnergizer=energizers.begin(); itEnergizer!=energizers.end(); itEnergizer++)
        {
            testY = itEnergizer->posY;
            testX = itEnergizer->posX;
            if (testY==posPacmanY&&testX==posPacmanX)
            {
                score += 50 - 10;
                energizers.erase(itEnergizer);
				timePower = 800; eatenPower = 0; 
                break;
            }
        }
		
		// Gestion des pac-gommes
		for (itDot=dots.begin(); itDot!=dots.end(); itDot++)
        {
            testY = itDot->posY;
            testX = itDot->posX;
            if (testX==posPacmanX&&testY==posPacmanY)
            {
                score += 10;
				nbDot -= 1;
                dots.erase(itDot);
                break;
            }
        }
		collision();
	}
}

void Jeu::moveGhost(){
	int testX, testY;
	list<Fantome>::iterator itFantome;
	
	int depX[] = {-1, 1, 0, 0};
	int depY[] = {0, 0, -1, 1};
		
		
	// Gestion des fantômes
		for (itFantome=fantomes.begin(); itFantome!=fantomes.end(); itFantome++){
			for(int i=0; i<3; i++){
				if(itFantome->posX == 12+i && itFantome->posY == 9)
					itFantome->dir = (Direction)(rand()%4);
			}

			testX = itFantome->posX + depX[itFantome->dir];
			testY = itFantome->posY + depY[itFantome->dir];

			if(terrain[testY*largeur+testX] == PASS && itFantome->pass == false){
				itFantome->posX = testX;
				itFantome->posY = testY;
				itFantome->pass = true;
			}
			else if (terrain[testY*largeur+testX] == VIDE || terrain[testY*largeur+testX] == SPAWN){
				itFantome->posX = testX;
				itFantome->posY = testY;
			}
			else
				// Changement de direction
				itFantome->dir = (Direction)(rand()%4);
		}
}

int Jeu::getNbCasesX() const
{
    return largeur;
}

int Jeu::getNbCasesY() const
{
    return hauteur;
}

int Jeu::getPacmanX() const
{
    return posPacmanX;
}

int Jeu::getPacmanY() const
{
    return posPacmanY;
}

int Jeu::getNbVie() const
{
    return nbVie;
}

void Jeu::setNbVie(int vie)
{
    nbVie=vie;
}

int Jeu::getScore() const
{
	return score;
}

int Jeu::getNbDot() const
{
	return nbDot;
}

int Jeu::getPowerTime() const
{ 
	return timePower;
}

Case Jeu::getCase(int x, int y) const
{
    assert(x>=0 && x<largeur && y>=0 && y<hauteur);
    return terrain[y*largeur+x];
}

const std::list<Fantome> &Jeu::getFantomes() const
{
    return fantomes;
}

const std::list<Dot> &Jeu::getDots() const
{
	return dots;
}

const std::list<Energizer> &Jeu::getEnergizers() const
{
	return energizers;
}

bool Jeu::posValide(int x, int y) const
{
    return (x>=0 && x<largeur && y>=0 && y<hauteur && terrain[y*largeur+x]==VIDE);
}

bool Jeu::deplacePacman(Direction dir)
{
    int depX[] = {-1, 1, 0, 0};
    int depY[] = {0, 0, -1, 1};
    int testX, testY;

    testX = posPacmanX + depX[dir];
    testY = posPacmanY + depY[dir];

    if(posPacmanX == 0 && posPacmanY == 9 && dir == GAUCHE)
    {
        posPacmanX = 26;
        posPacmanY = 9;
		return true;
    }
    else if(posPacmanX == 26 && posPacmanY == 9 && dir == DROITE)
    {
        posPacmanX = 0;
        posPacmanY = 9;
		return true;
    }
    else if (posValide(testX, testY)){
        posPacmanX = testX;
        posPacmanY = testY;
        return true;
    }
    else
		return false;
}

void Jeu::deadPacman()
{
	nbVie -= 1;
	posPacmanX = 13;
	posPacmanY = 11; 
}

// Collision entre Pacman et Fantôme
void Jeu::collision()
{
    list<Fantome>::iterator itFantome;

    for(itFantome = fantomes.begin(); itFantome != fantomes.end(); itFantome++) {
        if(posPacmanX == itFantome->posX && posPacmanY == itFantome->posY){
			if(timePower>0){
				eatenPower++;
				score += 100*pow(2,eatenPower);
                fantomes.erase(itFantome);
                break;
			}
			else
				deadPacman();
		}
    }
	
	if(timePower > 0) // Timer de 10 secondes
		timePower -= 1;
}