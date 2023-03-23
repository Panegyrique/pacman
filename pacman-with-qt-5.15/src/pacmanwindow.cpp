#include <iostream>
#include "pacmanwindow.hpp"

using namespace std;

PacmanWindow::PacmanWindow(QWidget *pParent, Qt::WindowFlags flags):QFrame(pParent, flags)
{
    // Taille des cases en pixels
    int largeurCase, hauteurCase;
	int decalage = 50;

    if (pixmapPacman.load("./data/pacman.png")==false)
    {
        cout<<"Impossible d'ouvrir pacman.png"<<endl;
        exit(-1);
    }

    if (pixmapFantome.load("./data/fantome.png")==false)
    {
        cout<<"Impossible d'ouvrir fantome.png"<<endl;
        exit(-1);
    }
	
	if (pixmapFantomePeur.load("./data/fantome_peur.png")==false)
    {
        cout<<"Impossible d'ouvrir fantome_peur.png"<<endl;
        exit(-1);
    }
	
	if (pixmapDot.load("./data/dot.png")==false)
    {
        cout<<"Impossible d'ouvrir dot.png"<<endl;
        exit(-1);
    }
	
	if (pixmapEnergizer.load("./data/energizer.png")==false)
    {
        cout<<"Impossible d'ouvrir energizer.png"<<endl;
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
	QFont Arcade("arcadepix", 20, 1);
	
	QLabel *TagLife = new QLabel(this);
    TagLife->setStyleSheet("background-color:black");
    TagLife->setGeometry(10,8+jeu.getNbCasesY()*hauteurCase,140,40);
    TagLife->setText("<font color='white'>LIFES<\font>");
    TagLife->setFont(Arcade);
	
	QLabel *TagScore = new QLabel(this);
    TagScore->setStyleSheet("background-color:black");
    TagScore->setGeometry(280,8+jeu.getNbCasesY()*hauteurCase,140,40);
    TagScore->setText("<font color='white'>SCORE<\font>");
    TagScore->setFont(Arcade);
	
	printScore = new QLabel(this);
    printScore->setStyleSheet("background-color:black");
    printScore->setGeometry(420,8+jeu.getNbCasesY()*hauteurCase,140,40);
	score = QString("<font color='yellow'>") + QString::number(jeu.getScore()) + QString("<\font>");
    printScore->setText(score);
    printScore->setFont(Arcade);
	
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

	// Dessine les pac-gommes
    const list<Dot> &dots = jeu.getDots();
    list<Dot>::const_iterator itDot;
    for (itDot=dots.begin(); itDot!=dots.end(); itDot++)
        painter.drawPixmap(itDot->getPosX()*largeurCase, itDot->getPosY()*hauteurCase, pixmapDot);

	// Dessine les super pac-gommes
    const list<Energizer> &energizers = jeu.getEnergizers();
    list<Energizer>::const_iterator itEnergizer;
    for (itEnergizer=energizers.begin(); itEnergizer!=energizers.end(); itEnergizer++)
        painter.drawPixmap(itEnergizer->getPosX()*largeurCase, itEnergizer->getPosY()*hauteurCase, pixmapEnergizer);

    // Dessine les fantomes
    const list<Fantome> &fantomes = jeu.getFantomes();
    list<Fantome>::const_iterator itFantome;
    for (itFantome=fantomes.begin(); itFantome!=fantomes.end(); itFantome++){
		if(jeu.getPowerTime() == 0)
			painter.drawPixmap(itFantome->getPosX()*largeurCase, itFantome->getPosY()*hauteurCase, pixmapFantome);
		else
			painter.drawPixmap(itFantome->getPosX()*largeurCase, itFantome->getPosY()*hauteurCase, pixmapFantomePeur);
	}

	// Dessine Pacman
	painter.drawPixmap(jeu.getPacmanX()*largeurCase, jeu.getPacmanY()*hauteurCase, pixmapPacman);

	// Dessine les vies
    for(int i=0;i<jeu.getNbVie();i++)
        painter.drawPixmap(150+i*35, 9+jeu.getNbCasesY()*hauteurCase , pixmapVie);
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
	score = QString("<font color='yellow'>") + QString::number(jeu.getScore()) + QString("<\font>");
    printScore->setText(score);
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