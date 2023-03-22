#include <iostream>
#include "pacmanwindow.hpp"

using namespace std;

PacmanWindow::PacmanWindow(QWidget *pParent, Qt::WindowFlags flags):QFrame(pParent, flags)
{
    // Taille des cases en pixels
    int largeurCase, hauteurCase;
	int decalage = 50;

    if (pixmapPacman.load("./data/pacman.bmp")==false)
    {
        cout<<"Impossible d'ouvrir pacman.png"<<endl;
        exit(-1);
    }

    if (pixmapFantome.load("./data/fantome.bmp")==false)
    {
        cout<<"Impossible d'ouvrir fantome.bmp"<<endl;
        exit(-1);
    }

    if (pixmapMur.load("./data/mur.bmp")==false)
    {
        cout<<"Impossible d'ouvrir mur.bmp"<<endl;
        exit(-1);
    }
	
	if (pixmapVie.load("./data/lifes.png")==false)
    {
        cout<<"Impossible d'ouvrir lifes.png"<<endl;
        exit(-1);
    }

    jeu.init();

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &PacmanWindow::handleTimer);
    timer->start(100);

    largeurCase = pixmapMur.width();
    hauteurCase = pixmapMur.height();

    resize(jeu.getNbCasesX()*largeurCase, decalage + jeu.getNbCasesY()*hauteurCase);
	
	QFontDatabase *MyFontDatabase = new QFontDatabase();
	MyFontDatabase->addApplicationFont("./data/arcadepi.ttf");
	QFont Arcade("arcadepix", 20, 1, true);
	
	QLabel *TagLife = new QLabel(this);
    TagLife->setStyleSheet("background-color:black");
    TagLife->setGeometry(10,8+jeu.getNbCasesY()*hauteurCase,140,40);
    TagLife->setText("<font color='white'>LIFES<\font>");
    TagLife->setFont(Arcade);
	
	/*
	PacmanButton *btn = new PacmanButton(this);
	connect(btn, &QPushButton::clicked, this, &PacmanWindow::clickMonBouton);
	*/
}

void PacmanWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
	
	//Fond du jeu en noir
    painter.fillRect(0, 0, 828, 828, Qt::black);
    painter.beginNativePainting();
    
    int x, y;

    // Taille des cases en pixels
    int largeurCase, hauteurCase;

    largeurCase = pixmapMur.width();
    hauteurCase = pixmapMur.height();

    // Dessine les cases
    for (y=0;y<jeu.getNbCasesY();y++)
        for (x=0;x<jeu.getNbCasesX();x++)
			if (jeu.getCase(x,y)==MUR)
                painter.drawPixmap(x*largeurCase, y*hauteurCase, pixmapMur);

    // Dessine les fantomes
    const list<Fantome> &fantomes = jeu.getFantomes();
    list<Fantome>::const_iterator itFantome;
    for (itFantome=fantomes.begin(); itFantome!=fantomes.end(); itFantome++)
        painter.drawPixmap(itFantome->getPosX()*largeurCase, itFantome->getPosY()*hauteurCase, pixmapFantome);

	// Dessine Pacman
	painter.drawPixmap(jeu.getPacmanX()*largeurCase, jeu.getPacmanY()*hauteurCase, pixmapPacman);

	// Dessine les vies
    for(int i=0;i<jeu.getNbVie();i++)
        painter.drawPixmap(155+i*35, 9+jeu.getNbCasesY()*hauteurCase , pixmapVie);
}

void PacmanWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key()==Qt::Key_Left)
        jeu.deplacePacman(GAUCHE);
    else if (event->key()==Qt::Key_Right)
        jeu.deplacePacman(DROITE);
    else if (event->key()==Qt::Key_Up)
        jeu.deplacePacman(HAUT);
    else if (event->key()==Qt::Key_Down)
        jeu.deplacePacman(BAS);
    update();
}

void PacmanWindow::handleTimer()
{
    jeu.evolue();
    update();
}

void PacmanWindow::clickMonBouton()
{
	
}

PacmanButton::PacmanButton(QWidget *pParent): QPushButton(pParent)
{
	
}

void PacmanButton::keyPressEvent(QKeyEvent *e)
{
	if (parent() != nullptr)
		QCoreApplication::sendEvent(parent(), e);
}