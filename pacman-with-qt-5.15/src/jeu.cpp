#include <iostream>
#include <cassert>
#include "jeu.hpp"

using namespace std;

Fantome::Fantome()
{
    posX = 0; posY = 0;
    dir = BAS;
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
	int nbDot = 0;
	list<Fantome>::iterator itFantome;
	list<Dot>::iterator itDot;
	list<Energizer>::iterator itEnergizer;
	
	largeur = 20;
	hauteur = 15;
	nbVie = 3;
	score = 0;
	timePower = 0;

	const char terrain_defaut[15][21] = {
		"####################",
		"#........##........#",
		"#.#####..##...####.#",
		"#........##........#",
		"#..................#",
		"#......#....#......#",
		"#......#...##......#",
		"#####..#....#..#####",
		"#......##...#......#",
		"#......#....#......#",
		"#..................#",
		"#..................#",
		"#.....#......#.....#",
		"#.....#......#.....#",
        "####################"
    };

	terrain = new Case[largeur*hauteur];
	for(y=0;y<hauteur;++y)
		for(x=0;x<largeur;++x)
            if (terrain_defaut[y][x]=='#')
                terrain[y*largeur+x] = MUR;
            else
			{
                terrain[y*largeur+x] = VIDE;
				nbDot += 1;
			}
			
    fantomes.resize(4);
	for (itFantome=fantomes.begin(); itFantome!=fantomes.end(); itFantome++)
    {
        do {
            x = rand()%largeur;
            y = rand()%hauteur;
        } while (terrain[y*largeur+x]!=VIDE);

        itFantome->posX = x;
        itFantome->posY = y;
        itFantome->dir = (Direction)(rand()%4);
    }
	
	energizers.resize(4);
	for(itEnergizer=energizers.begin(); itEnergizer!=energizers.end(); itEnergizer++)
	{
		do{
			x = rand()%largeur;
			y = rand()%hauteur;
		} while (terrain[y*largeur+x]!=VIDE);

		itEnergizer->posX = x;
		itEnergizer->posY = y;
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
				timePower = 80;
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
                dots.erase(itDot);
                break;
            }
        }
		
		// Gestion des fantômes
		for (itFantome=fantomes.begin(); itFantome!=fantomes.end(); itFantome++)
		{
			testX = itFantome->posX + depX[itFantome->dir];
			testY = itFantome->posY + depY[itFantome->dir];

			if (terrain[testY*largeur+testX]==VIDE)
			{
				itFantome->posX = testX;
				itFantome->posY = testY;
			}
			else
				itFantome->dir = (Direction)(rand()%4);
		}
		
		collision();
	}
	else
		exit(-1);
	
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

    if (posValide(testX, testY))
    {
        posPacmanX = testX;
        posPacmanY = testY;
        return true;
    }
    else
        return false;
}

// Collision entre Pacman et Fantôme
void Jeu::collision()
{
    list<Fantome>::iterator itFantome;

    for(itFantome = fantomes.begin(); itFantome != fantomes.end(); itFantome++) {
        if(posPacmanX == itFantome->posX && posPacmanY == itFantome->posY){
			if(timePower>0){
				score += 250;
                fantomes.erase(itFantome);
                break;
			}
			else
				nbVie -= 1;
		}
    }
	
	if(timePower > 0) // Timer de 10 secondes
		timePower -= 1;
}