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

    if (pixmapGhostAzure.load("./data/ghosts/ghost_azure.png")==false)
    {
        cout<<"Impossible d'ouvrir ghost_azure.png"<<endl;
        exit(-1);
    }

    if (pixmapGhostPink.load("./data/ghosts/ghost_pink.png")==false)
    {
        cout<<"Impossible d'ouvrir ghost_pink.png"<<endl;
        exit(-1);
    }

	if (pixmapGhostRed.load("./data/ghosts/ghost_red.png")==false)
    {
        cout<<"Impossible d'ouvrir ghost_red.png"<<endl;
        exit(-1);
    }

	if (pixmapGhostYellow.load("./data/ghosts/ghost_yellow.png")==false)
    {
        cout<<"Impossible d'ouvrir ghost_yellow.png"<<endl;
        exit(-1);
    }

    if (pixmapGhostBlue.load("./data/ghosts/ghost_blue.png")==false)
    {
        cout<<"Impossible d'ouvrir ghost_blue.png"<<endl;
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

    if (pixmapMurVertical.load("./data/map/mur_vertical.png")==false)
    {
        cout<<"Impossible d'ouvrir mur_vertical.png"<<endl;
        exit(-1);
    }

    if (pixmapMurHorizontal.load("./data/map/mur_horizontal.png")==false)
    {
        cout<<"Impossible d'ouvrir mur_horizontal.png"<<endl;
        exit(-1);
    }

    if (pixmapArrondiGauche.load("./data/map/arrondi_gauche.png")==false)
    {
        cout<<"Impossible d'ouvrir arrondi_gauche.png"<<endl;
        exit(-1);
    }

    if (pixmapArrondiDroit.load("./data/map/arrondi_droit.png")==false)
    {
        cout<<"Impossible d'ouvrir arrondi_droit.png"<<endl;
        exit(-1);
    }

    if (pixmapArrondiHaut.load("./data/map/arrondi_haut.png")==false)
    {
        cout<<"Impossible d'ouvrir coin_haut.png"<<endl;
        exit(-1);
    }

    if (pixmapArrondiBas.load("./data/map/arrondi_bas.png")==false)
    {
        cout<<"Impossible d'ouvrir coin_bas.png"<<endl;
        exit(-1);
    }

    if (pixmapJonctionDroite.load("./data/map/jonction_droite.png")==false)
    {
        cout<<"Impossible d'ouvrir jonction_droite.png"<<endl;
        exit(-1);
    }

    if (pixmapJonctionGauche.load("./data/map/jonction_gauche.png")==false)
    {
        cout<<"Impossible d'ouvrir jonction_gauche.png"<<endl;
        exit(-1);
    }

    if (pixmapJonctionHaute.load("./data/map/jonction_haute.png")==false)
    {
        cout<<"Impossible d'ouvrir jonction_haute.png"<<endl;
        exit(-1);
    }

    if (pixmapJonctionBasse.load("./data/map/jonction_basse.png")==false)
    {
        cout<<"Impossible d'ouvrir jonction_basse.png"<<endl;
        exit(-1);
    }

    if (pixmapCoinHautGauche.load("./data/map/coin_haut_gauche.png")==false)
    {
        cout<<"Impossible d'ouvrir coin_haut_gauche.png"<<endl;
        exit(-1);
    }

    if (pixmapCoinHautDroit.load("./data/map/coin_haut_droit.png")==false)
    {
        cout<<"Impossible d'ouvrir coin_haut_droit.png"<<endl;
        exit(-1);
    }

    if (pixmapCoinBasDroit.load("./data/map/coin_bas_droit.png")==false)
    {
        cout<<"Impossible d'ouvrir coin_bas_droit.png"<<endl;
        exit(-1);
    }

    if (pixmapCoinBasGauche.load("./data/map/coin_bas_gauche.png")==false)
    {
        cout<<"Impossible d'ouvrir coin_bas_gauche.png"<<endl;
        exit(-1);
    }
	
	if (pixmapVie.load("./data/lifes.png")==false)
    {
        cout<<"Impossible d'ouvrir lifes.png"<<endl;
        exit(-1);
    }
	
	if (pixmapCerise.load("./data/cerise.png")==false)
    {
        cout<<"Impossible d'ouvrir cerise.png"<<endl;
        exit(-1);
    }

    jeu.init();

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &PacmanWindow::handleTimer);
    timer->start(10);
	
	move = new QTimer(this);
    connect(move, &QTimer::timeout, this, &PacmanWindow::moveTimer);
	move->start(10);

    largeurCase = pixmapMurVertical.width();
    hauteurCase = pixmapMurVertical.height();

    resize(jeu.getNbCasesX()*largeurCase, decalage + jeu.getNbCasesY()*hauteurCase);
	
	QFontDatabase *MyFontDatabase = new QFontDatabase();
	MyFontDatabase->addApplicationFont("./data/arcadepi.ttf");
	QFont Arcade("arcadepix", 20, 1);
	
	TagLife = new QLabel(this);
    TagLife->setStyleSheet("background-color:black");
    TagLife->setGeometry(10,8+jeu.getNbCasesY()*hauteurCase,140,40);
    TagLife->setText("<font color='white'>LIFES<\font>");
    TagLife->setFont(Arcade);
	
	TagScore = new QLabel(this);
    TagScore->setStyleSheet("background-color:black");
    TagScore->setGeometry(360,8+jeu.getNbCasesY()*hauteurCase,140,40);
    TagScore->setText("<font color='white'>SCORE<\font>");
    TagScore->setFont(Arcade);
	
	printScore = new QLabel(this);
    printScore->setStyleSheet("background-color:black");
    printScore->setGeometry(500,8+jeu.getNbCasesY()*hauteurCase,140,40);
	score = QString("<font color='yellow'>") + QString::number(jeu.getScore()) + QString("<\font>");
    printScore->setText(score);
    printScore->setFont(Arcade);
	
	Cerise = new QLabel(this);
	Cerise->setGeometry(jeu.getNbCasesX()*largeurCase-80,8+jeu.getNbCasesY()*hauteurCase,32,32);
	Cerise->setPixmap(pixmapCerise);
}

void PacmanWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
	
	//Fond du jeu en noir
    painter.fillRect(0, 0, 27*32, 19*32+50, Qt::black);
    painter.beginNativePainting();
    
    int x, y;

    // Taille des cases en pixels
    int largeurCase, hauteurCase;

    largeurCase = pixmapMurVertical.width();
    hauteurCase = pixmapMurVertical.height();

    // Dessine les cases
    for (y=0;y<jeu.getNbCasesY();y++)
    {
        for (x=0;x<jeu.getNbCasesX();x++)
        {
            switch(jeu.getCase(x,y))
            {
                case MUR_VERTICAL:
                    painter.drawPixmap(x*largeurCase, y*hauteurCase, pixmapMurVertical);
                    break;
                case MUR_HORIZONTAL:
                    painter.drawPixmap(x*largeurCase, y*hauteurCase, pixmapMurHorizontal);
                    break;
                case ARRONDI_DROIT:
                    painter.drawPixmap(x*largeurCase, y*hauteurCase, pixmapArrondiDroit);
                    break;
                case ARRONDI_GAUCHE:
                    painter.drawPixmap(x*largeurCase, y*hauteurCase, pixmapArrondiGauche);
                    break;
                case ARRONDI_HAUT:
                    painter.drawPixmap(x*largeurCase, y*hauteurCase, pixmapArrondiHaut);
                    break;
                case ARRONDI_BAS:
                    painter.drawPixmap(x*largeurCase, y*hauteurCase, pixmapArrondiBas);
                    break;
                case JONCTION_DROITE:
                    painter.drawPixmap(x*largeurCase, y*hauteurCase, pixmapJonctionDroite);
                    break;
                case JONCTION_GAUCHE:
                    painter.drawPixmap(x*largeurCase, y*hauteurCase, pixmapJonctionGauche);
                    break;
                case JONCTION_HAUTE:
                    painter.drawPixmap(x*largeurCase, y*hauteurCase, pixmapJonctionHaute);
                    break;
                case JONCTION_BASSE:
                    painter.drawPixmap(x*largeurCase, y*hauteurCase, pixmapJonctionBasse);
                    break;
                case COIN_HAUT_GAUCHE:
                    painter.drawPixmap(x*largeurCase, y*hauteurCase, pixmapCoinHautGauche);
                    break;
                case COIN_HAUT_DROIT:
                    painter.drawPixmap(x*largeurCase, y*hauteurCase, pixmapCoinHautDroit);
                    break;
                case COIN_BAS_DROIT:
                    painter.drawPixmap(x*largeurCase, y*hauteurCase, pixmapCoinBasDroit);
                    break;
                case COIN_BAS_GAUCHE:
                    painter.drawPixmap(x*largeurCase, y*hauteurCase, pixmapCoinBasGauche);
                    break;
            }
        }
    }
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
	int compteur = 0;
    const list<Fantome> &fantomes = jeu.getFantomes();
    list<Fantome>::const_iterator itFantome;
    for (itFantome=fantomes.begin(); itFantome!=fantomes.end(); itFantome++){
		if(itFantome->getFear() == false){
			if(compteur >= 4)
						compteur = 0;

			switch(compteur)
			{
				case 0:
					painter.drawPixmap(itFantome->getPosX()*largeurCase, itFantome->getPosY()*hauteurCase, pixmapGhostAzure);
					break;
				case 1:
					painter.drawPixmap(itFantome->getPosX()*largeurCase, itFantome->getPosY()*hauteurCase, pixmapGhostPink);
					break;
				case 2:
					painter.drawPixmap(itFantome->getPosX()*largeurCase, itFantome->getPosY()*hauteurCase, pixmapGhostRed);
					break;
				case 3:
					painter.drawPixmap(itFantome->getPosX()*largeurCase, itFantome->getPosY()*hauteurCase, pixmapGhostYellow);
					break;
			}
		}else
			painter.drawPixmap(itFantome->getPosX()*largeurCase, itFantome->getPosY()*hauteurCase, pixmapGhostBlue);
		compteur++;
	}
	// Dessine Pacman
	painter.drawPixmap(jeu.getPacmanX()*largeurCase, jeu.getPacmanY()*hauteurCase, pixmapPacman);

	// Dessine les vies
    for(int i=0;i<jeu.getNbVie();i++)
        painter.drawPixmap(150+i*35, 9+jeu.getNbCasesY()*hauteurCase , pixmapVie);

    // Game Over ?
    if(jeu.getNbVie() <= 0)
        gameOver(&painter);
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

void PacmanWindow::moveTimer()
{
	move->setInterval(50+jeu.getNbDot());
    jeu.moveGhost();
    update();
}

void PacmanWindow::gameOver(QPainter *painter)
{
    // Suppression texte
    TagLife->hide();
    TagScore->hide();
    Cerise->hide();

    // Image de fond game over
    QRectF target(0, 0, 864, 608); // 27*32 & 19*32
    QRectF source(0, 0, 864, 608);
    QImage game_over("./data/game_over.png");
    painter->drawImage(target, game_over, source);

    // Bouger score
    QFont font("arcadepix", 32);
    int printScoreWidth = 140;
    int printScoreHeight = 40;
    int printScoreX = (target.width() - printScoreWidth) / 2 + 10;
    int printScoreY = (target.height() - printScoreHeight) / 2 + 15;
    printScore->move(printScoreX, printScoreY);
    printScore->setFont(font);

    // Creation des boutons
    createButton(painter, &target);

    // Update
    update();
}

void PacmanWindow::createButton(QPainter *painter, QRectF *target)
{
	// Ajout du premier bouton
    int buttonWidth = 150;
    int buttonHeight = 60;
    int buttonYesX = (target->width() - buttonWidth) / 2 - 100;
    int buttonYesY = (3.3 * target->height() / 4) - (buttonHeight / 2);
    QRect buttonYesRect(buttonYesX, buttonYesY, buttonWidth, buttonHeight);
    // Ajout du deuxième bouton
    int buttonNoX = (target->width() - buttonWidth) / 2 + 100;
    int buttonNoY = (3.3 * target->height() / 4) - (buttonHeight / 2);
    QRect buttonNoRect(buttonNoX, buttonNoY, buttonWidth, buttonHeight);

    // Style du premier bouton
    QBrush brushYes(QColor(0, 0, 0));
    QPen penYes(Qt::NoPen);
    QFont font("arcadepix", 16);
    painter->setBrush(brushYes);
    painter->setPen(penYes);
    painter->setFont(font);
    painter->drawRoundedRect(buttonYesRect, 5, 5);
    painter->setPen(QColor(255, 255, 0));
    painter->drawText(buttonYesRect, Qt::AlignCenter, "Yes");
    // Style du deuxième bouton
    QBrush brushNo(QColor(0, 0, 0));
    QPen penNo(Qt::NoPen);
    painter->setBrush(brushNo);
    painter->setPen(penNo);
    painter->setFont(font);
    painter->drawRoundedRect(buttonNoRect, 5, 5);
    painter->setPen(QColor(255, 255, 0));
    painter->drawText(buttonNoRect, Qt::AlignCenter, "No");

    // Assimilation du bouton visuel crée à un vrai bouton
    PacmanButton *buttonYes = new PacmanButton(this);
    buttonYes->setGeometry(buttonYesRect);
    buttonYes->setFlat(true);
    buttonYes->setCursor(Qt::PointingHandCursor);
    buttonYes->show();
    // Assimilation du deuxième bouton visuel créé à un vrai bouton
    PacmanButton *buttonNo = new PacmanButton(this);
    buttonNo->setGeometry(buttonNoRect);
    buttonNo->setFlat(true);
    buttonNo->setCursor(Qt::PointingHandCursor);
    buttonNo->show();

    // Connexion du premier bouton à une fonction
    connect(buttonYes, &QPushButton::clicked, this, &PacmanWindow::clickButtonYes);
    // Connexion du deuxième bouton à une fonction
    connect(buttonNo, &QPushButton::clicked, this, &PacmanWindow::clickButtonNo);
}

void PacmanWindow::clickButtonYes()
{
    QFont Arcade("arcadepix", 20, 1);
    int printScoreX = 500;
    int printScoreY = 8 + jeu.getNbCasesY() * pixmapMurVertical.height();
    printScore->move(printScoreX, printScoreY);
    printScore->setFont(Arcade);

    TagLife->show();
    TagScore->show();
    Cerise->show();

    jeu.init();
    update();
}

void PacmanWindow::clickButtonNo()
{
	exit(-1);
}

PacmanButton::PacmanButton(QWidget *pParent): QPushButton(pParent)
{
	
}

void PacmanButton::keyPressEvent(QKeyEvent *e)
{
	if (parent() != nullptr)
		QCoreApplication::sendEvent(parent(), e);
}