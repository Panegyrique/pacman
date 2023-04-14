#ifndef PACMANWINDOW_HPP
#define PACMANWINDOW_HPP

#include <QtCore/QtCore>
#include <QtWidgets/QtWidgets>
#include "soundgenerator.hpp"
#include "jeu.hpp"

#define DECALAGE_Y_BANNIERE 50
#define PIXELS 32

typedef enum {WIN, GAMEOVER} endGame;

class PacmanWindow : public QFrame
{
  protected:
    Jeu jeu;

	// Variables pour les futures images chargees
    QPixmap pixmapPacman, 
		pixmapGhostAzure, pixmapGhostPink, pixmapGhostRed, pixmapGhostYellow, pixmapGhostBlue, pixmapGhostWhite,
		pixmapMurVertical, pixmapMurHorizontal,
            pixmapArrondiDroit, pixmapArrondiGauche, pixmapArrondiHaut, pixmapArrondiBas,
            pixmapJonctionDroite, pixmapJonctionGauche, pixmapJonctionHaute, pixmapJonctionBasse,
			pixmapCoinHautGauche, pixmapCoinHautDroit, pixmapCoinBasDroit, pixmapCoinBasGauche, pixmapBarreSpawn,
		pixmapDot, pixmapEnergizer, pixmapVie, pixmapCerise;
	
	// Timer propre au deplacement des entites (different de Handletimer qui rafraichit le jeu)
	QTimer *move;

	// Label et string pour comprendre notre avancement dans le jeu
	QLabel *printHighscores, *printScore, *TagLife, *TagScore, *Cerise, *printCerise;
	QString score, highscores, cerise;

	// Direction par defaut au lancement du jeu
	Direction directionPacman = STATIC;

	// Booleen pour eviter de rejouer la musique de fin en boucle
	bool GameOver;
	bool YouWin;

	// Booleen pour n'afficher le menu qu'au lancement
	bool doWelcome = true;

  public:
    PacmanWindow(QWidget *pParent=0, Qt::WindowFlags flags=Qt::WindowFlags());
	void directionGhosts();
    void handleTimer();
	void moveTimer();
	void generateSound(const char* filename);

  protected:
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *);
	void welcome(QPainter *);
	void createButtonPlayGame(QPainter *, QRectF *);
	void clickButtonPlayGame();
	void winOrGameOver(QPainter *, endGame);
	void createButtonYesNo(QPainter *, QRectF *);
	void clickButtonYes();
	void clickButtonNo();
	void loadImages();
};

class PacmanButton : public QPushButton
{
	public:
	PacmanButton(QWidget *);
	
	private:
	void keyPressEvent(QKeyEvent *);
};

#endif
