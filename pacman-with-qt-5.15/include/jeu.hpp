#ifndef JEU_HPP
#define JEU_HPP

#include <list>

#define LARGEUR 27
#define HAUTEUR 19
#define NB_VIE 3
#define NB_FANTOME 4
#define Proba_cerise 70000
#define POWERTIME 8000
#define POSX_init 13
#define POSY_init 15
#define POSX_FRUIT 13
#define POSY_FRUIT 11

typedef enum
{   MUR_VERTICAL, MUR_HORIZONTAL,
        ARRONDI_DROIT, ARRONDI_GAUCHE, ARRONDI_HAUT, ARRONDI_BAS,
        JONCTION_DROITE, JONCTION_GAUCHE, JONCTION_HAUTE, JONCTION_BASSE,
        COIN_HAUT_GAUCHE, COIN_HAUT_DROIT, COIN_BAS_DROIT, COIN_BAS_GAUCHE,
    VIDE, SPAWN, PASS, INTERDIT
} Case; // Differents types de case

typedef enum {GAUCHE, DROITE, HAUT, BAS, STATIC} Direction; // Differentes directions 

class Jeu;

class Fantome
{
    friend class Jeu;

  protected:
    int posX, posY;
    Direction dir;
	bool pass, fear;

  public:
    Fantome();
    int getPosX() const;
    int getPosY() const;
	bool getFear() const;
    Direction getDirection() const;
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
    Case *terrain; // Tableau de case qui contient le terrain 
    int largeur, hauteur; // Nombre de cases en largeur et en hauteur
    int posPacmanX, posPacmanY; // Position pacman
	int nbVie, score; // Nombre de vie et score
	int nbDot; // Nombre de pac-gomme
	int timePower, eatenPower; // Power-up
	bool isCerise; // Presence du fruit
	int eatenCerise;
	bool eatGhost = false;
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
	
	// Retourne et modifie le meilleur score
	void setHighscores();
    int getHighscores();
	
	// Retourne le nombre de pac-gommes
	int getNbDot() const;
	
	// Retourne le temps du power-up
	int getPowerTime() const;
	
	// Retourne si cerise ou pas et le nb de cerises mangees 
	bool getCerise() const;
	int getEatenCerise() const;
	
	// Retourne et modifie si pacman vient de manger un fantome
	bool getEatGhost() const;
	void setEatGhost(bool);

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
    Direction deplacePacman(Direction);
	
	// Mort du Pacman
	void deadPacman();
	
	// Collision Pacman/Fantome
	void collision();	
};

#endif
