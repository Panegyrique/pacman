#ifndef JEU_HPP
#define JEU_HPP

#include <list>

typedef enum
{   MUR_VERTICAL, MUR_HORIZONTAL,
        ARRONDI_DROIT, ARRONDI_GAUCHE, ARRONDI_HAUT, ARRONDI_BAS,
        JONCTION_DROITE, JONCTION_GAUCHE, JONCTION_HAUTE, JONCTION_BASSE,
        COIN_HAUT_GAUCHE, COIN_HAUT_DROIT, COIN_BAS_DROIT, COIN_BAS_GAUCHE,
    VIDE, SPAWN, PASS, INTERDIT
} Case;
typedef enum {GAUCHE, DROITE, HAUT, BAS} Direction;

class Jeu;

class Fantome
{
    friend class Jeu;

  protected:
    int posX, posY;
    Direction dir;
	bool pass;

  public:
    Fantome();
    int getPosX() const;
    int getPosY() const;
};

class Dot
{
        friend class Jeu;

    protected:
        int posX, posY;

    public:
        Dot();
        int getPosX() const;
        int getPosY() const;
};

class Energizer
{
        friend class Jeu;

    protected:
        int posX, posY;

    public:
        Energizer();
        int getPosX() const;
        int getPosY() const;
};

class Jeu
{
  protected:
    Case *terrain;
    int largeur, hauteur; // Nombre de cases en largeur et en hauteur
    int posPacmanX, posPacmanY;
	int nbVie, score;
	int nbDot;
	int timePower, eatenPower;
    std::list<Fantome> fantomes;
	std::list<Dot> dots;
	std::list<Energizer> energizers;

  public:
    Jeu();
    Jeu(const Jeu &);
    ~Jeu();

    Jeu &operator=(const Jeu &);

    bool init();
    void evolue();
	void moveGhost();

    // Retourne les dimensions (en nombre de cases)
    int getNbCasesX() const;
    int getNbCasesY() const;

    // Retourne la position du Pacman
    int getPacmanX() const;
    int getPacmanY() const;
	
	// Retourne et modifie le nombre de vie
	int getNbVie() const;
	void setNbVie(int);
	
	// Retourne le score actuel
	int getScore() const;
	
	int getNbDot() const;
	
	int getPowerTime() const;

    // Retourne la case � une position donn�e
    Case getCase(int, int) const;

    // Retourne la liste de fantomes en lecture seule
    const std::list<Fantome> &getFantomes() const;

	// Retourne la liste de pac-gommes en lecture seule
    const std::list<Dot> &getDots() const;
	
	// Retourne la liste de super pac-gommes en lecture seule
    const std::list<Energizer> &getEnergizers() const;
	
    // Indique si la case � une position donn�e existe et est vide
    // (Pacman ou les fantomes peuvent l'occuper)
    bool posValide(int, int) const;

    // D�place Pacman dans une direction (si la case � atteindre est valide)
    bool deplacePacman(Direction);
	
	void deadPacman();
	
	void collision();	
};

#endif
