#ifndef PACMANWINDOW_HPP
#define PACMANWINDOW_HPP

#include <QtCore/QtCore>
#include <QtWidgets/QtWidgets>
#include "jeu.hpp"

class PacmanWindow : public QFrame
{
  protected:
    Jeu jeu;
    QPixmap pixmapPacman, pixmapFantome, pixmapFantomePeur, pixmapDot, pixmapEnergizer, pixmapMur, pixmapVie, pixmapCerise;
	QLabel *printScore;
	QString score;

  public:
    PacmanWindow(QWidget *pParent=0, Qt::WindowFlags flags=Qt::WindowFlags());
    void handleTimer();
	void clickMonBouton();

  protected:
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *);
};

class PacmanButton : public QPushButton
{
	public:
	PacmanButton(QWidget *);
	
	private:
	void keyPressEvent(QKeyEvent *);
};

#endif
