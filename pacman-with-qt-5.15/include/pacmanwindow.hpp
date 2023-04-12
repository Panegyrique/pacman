#ifndef PACMANWINDOW_HPP
#define PACMANWINDOW_HPP

#include <QtCore/QtCore>
#include <QtWidgets/QtWidgets>
#include "soundgenerator.hpp"
#include "jeu.hpp"

typedef enum {WIN, GAMEOVER} endGame;

class PacmanWindow : public QFrame
{
  protected:
    Jeu jeu;
    QPixmap pixmapPacman, 
		pixmapGhostAzure, pixmapGhostPink, pixmapGhostRed, pixmapGhostYellow, pixmapGhostBlue,
		pixmapMurVertical, pixmapMurHorizontal,
            pixmapArrondiDroit, pixmapArrondiGauche, pixmapArrondiHaut, pixmapArrondiBas,
            pixmapJonctionDroite, pixmapJonctionGauche, pixmapJonctionHaute, pixmapJonctionBasse,
			pixmapCoinHautGauche, pixmapCoinHautDroit, pixmapCoinBasDroit, pixmapCoinBasGauche,
		pixmapDot, pixmapEnergizer, pixmapVie, pixmapCerise;
	QTimer *move;
	QLabel *printHighscores,*printScore, *TagLife, *TagScore, *Cerise;
	QString score, highscores;
	Direction directionPacman;
	bool GameOver;
	bool doWelcome = true;

  public:
    PacmanWindow(QWidget *pParent=0, Qt::WindowFlags flags=Qt::WindowFlags());
	void directionGhosts();
    void handleTimer();
	void moveTimer();
	void generateSound(const char* filename);

  protected:
	void loadImages();
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *);
	
	void winOrGameOver(QPainter *, endGame);
	void createButtonYesNo(QPainter *, QRectF *);
	void clickButtonYes();
	void clickButtonNo();
	
	void welcome(QPainter *);
	void createButtonPlayGame(QPainter *, QRectF *);
	void clickButtonPlayGame();
};

class PacmanButton : public QPushButton
{
	public:
	PacmanButton(QWidget *);
	
	private:
	void keyPressEvent(QKeyEvent *);
};

#endif
