#include <iostream>
#include "pacmanwindow.hpp"

using namespace std;

PacmanWindow::PacmanWindow(QWidget *pParent, Qt::WindowFlags flags):QFrame(pParent, flags)
{
    // Chargement des images
    loadImages();

    // Taille des cases en pixels
    int largeurCase, hauteurCase;
	int decalage = 50;

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
    QFont ArcadeHighscores("arcadepix", 26);
	
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

    // Record
    QRectF target(0, 0, 864, 608);
    int highscoresWidth = 170;
    int highscoresHeight = 40;
    int highscoresX;
    if(jeu.getHighscores() >= 1000)
        highscoresX = (target.width() - highscoresWidth) / 2 + 20;
    else
        highscoresX = (target.width() - highscoresWidth) / 2 + 40;
    int highscoresY = (3.6 * target.height() / 4) - (highscoresHeight / 2);

    printHighscores = new QLabel(this);
    printHighscores->setStyleSheet("background-color:black");
    printHighscores->setGeometry(highscoresX, highscoresY, highscoresWidth, highscoresHeight);
	highscores = QString("<font color='yellow'>") + QString::number(jeu.getHighscores()) + QString("<\font>");
    printHighscores->setText(highscores);
    printHighscores->setFont(ArcadeHighscores);
}

void PacmanWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
	
	//Fond du jeu en noir
    painter.fillRect(0, 0, 27*32, 19*32+50, Qt::black);
    painter.beginNativePainting();

    // Position
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
		if(jeu.getPowerTime() == 0)
        {
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
			compteur++;
		}
        else
			painter.drawPixmap(itFantome->getPosX()*largeurCase, itFantome->getPosY()*hauteurCase, pixmapGhostBlue);
	}

	// Dessine Pacman
	painter.drawPixmap(jeu.getPacmanX()*largeurCase, jeu.getPacmanY()*hauteurCase, pixmapPacman);

	// Dessine les vies
    for(int i=0;i<jeu.getNbVie();i++)
        painter.drawPixmap(150+i*35, 9+jeu.getNbCasesY()*hauteurCase , pixmapVie);

    // Changement de direction des fantomes ?
    directionGhosts();

    // Game Over ?
    if(jeu.getNbVie() <= 0)
        winOrGameOver(&painter, GAMEOVER);

    // Win ?
    if(jeu.getNbDot() == 0)
        winOrGameOver(&painter, WIN);

    // Menu Init ?
    if(doWelcome == true)
        welcome(&painter);
}

void PacmanWindow::directionGhosts()
{
    const list<Fantome> &fantomes = jeu.getFantomes();
    list<Fantome>::const_iterator itFantome;
    int compteur = 0;

    for (itFantome=fantomes.begin(); itFantome!=fantomes.end(); itFantome++)
    {
		if(jeu.getPowerTime() == 0)
        {
			if(compteur >= 4)
				compteur = 0;

            Direction dirGhost = itFantome->getDirection();

			switch(dirGhost)
			{
				case GAUCHE:
					switch(compteur)
                    {
                        case 0:
                            if(pixmapGhostAzure.load("./data/ghosts/azure/ghostAzureLeft.png")==false)
                            {
                                cout<<"Impossible d'ouvrir ghostAzureLeft.png"<<endl;
                                exit(-1);
                            }
                            else
                            {
                                pixmapGhostAzure.load("./data/ghosts/azure/ghostAzureLeft.png");
                            }
                            break;
                        case 1:
                            if(pixmapGhostPink.load("./data/ghosts/pink/ghostPinkLeft.png")==false)
                            {
                                cout<<"Impossible d'ouvrir ghostPinkLeft.png"<<endl;
                                exit(-1);
                            }
                            else
                            {
                                pixmapGhostPink.load("./data/ghosts/pink/ghostPinkLeft.png");
                            }
                            break;
                        case 2:
                            if(pixmapGhostRed.load("./data/ghosts/red/ghostRedLeft.png")==false)
                            {
                                cout<<"Impossible d'ouvrir ghostRedLeft.png"<<endl;
                                exit(-1);
                            }
                            else
                            {
                                pixmapGhostRed.load("./data/ghosts/red/ghostRedLeft.png");
                            }
                            break;
                        case 3:
                            if(pixmapGhostYellow.load("./data/ghosts/yellow/ghostYellowLeft.png")==false)
                            {
                                cout<<"Impossible d'ouvrir ghostYellowLeft.png"<<endl;
                                exit(-1);
                            }
                            else
                            {
                                pixmapGhostYellow.load("./data/ghosts/yellow/ghostYellowLeft.png");
                            }
                            break;
                    }
					break;
				case DROITE:
                    switch(compteur)
                    {
                        case 0:
                            if(pixmapGhostAzure.load("./data/ghosts/azure/ghostAzureRight.png")==false)
                            {
                                cout<<"Impossible d'ouvrir ghostAzureRight.png"<<endl;
                                exit(-1);
                            }
                            else
                            {
                                pixmapGhostAzure.load("./data/ghosts/azure/ghostAzureRight.png");
                            }
                            break;
                        case 1:
                            if(pixmapGhostPink.load("./data/ghosts/pink/ghostPinkRight.png")==false)
                            {
                                cout<<"Impossible d'ouvrir ghostPinkRight.png"<<endl;
                                exit(-1);
                            }
                            else
                            {
                                pixmapGhostPink.load("./data/ghosts/pink/ghostPinkRight.png");
                            }
                            break;
                        case 2:
                            if(pixmapGhostRed.load("./data/ghosts/red/ghostRedRight.png")==false)
                            {
                                cout<<"Impossible d'ouvrir ghostRedRight.png"<<endl;
                                exit(-1);
                            }
                            else
                            {
                                pixmapGhostRed.load("./data/ghosts/red/ghostRedRight.png");
                            }
                            break;
                        case 3:
                            if(pixmapGhostYellow.load("./data/ghosts/yellow/ghostYellowRight.png")==false)
                            {
                                cout<<"Impossible d'ouvrir ghostYellowRight.png"<<endl;
                                exit(-1);
                            }
                            else
                            {
                                pixmapGhostYellow.load("./data/ghosts/yellow/ghostYellowRight.png");
                            }
                            break;
                    }
					break;
				case HAUT:
                    switch(compteur)
                    {
                        case 0:
                            if(pixmapGhostAzure.load("./data/ghosts/azure/ghostAzureUp.png")==false)
                            {
                                cout<<"Impossible d'ouvrir ghostAzureUp.png"<<endl;
                                exit(-1);
                            }
                            else
                            {
                                pixmapGhostAzure.load("./data/ghosts/azure/ghostAzureUp.png");
                            }
                            break;
                        case 1:
                            if(pixmapGhostPink.load("./data/ghosts/pink/ghostPinkUp.png")==false)
                            {
                                cout<<"Impossible d'ouvrir ghostPinkUp.png"<<endl;
                                exit(-1);
                            }
                            else
                            {
                                pixmapGhostPink.load("./data/ghosts/pink/ghostPinkUp.png");
                            }
                            break;
                        case 2:
                            if(pixmapGhostRed.load("./data/ghosts/red/ghostRedUp.png")==false)
                            {
                                cout<<"Impossible d'ouvrir ghostRedUp.png"<<endl;
                                exit(-1);
                            }
                            else
                            {
                                pixmapGhostRed.load("./data/ghosts/red/ghostRedUp.png");
                            }
                            break;
                        case 3:
                            if(pixmapGhostYellow.load("./data/ghosts/yellow/ghostYellowUp.png")==false)
                            {
                                cout<<"Impossible d'ouvrir ghostYellowUp.png"<<endl;
                                exit(-1);
                            }
                            else
                            {
                                pixmapGhostYellow.load("./data/ghosts/yellow/ghostYellowUp.png");
                            }
                            break;
                    }
					break;
				case BAS:
                    switch(compteur)
                    {
                        case 0:
                            if(pixmapGhostAzure.load("./data/ghosts/azure/ghostAzureDown.png")==false)
                            {
                                cout<<"Impossible d'ouvrir ghostAzureDown.png"<<endl;
                                exit(-1);
                            }
                            else
                            {
                                pixmapGhostAzure.load("./data/ghosts/azure/ghostAzureDown.png");
                            }
                            break;
                        case 1:
                            if(pixmapGhostPink.load("./data/ghosts/pink/ghostPinkDown.png")==false)
                            {
                                cout<<"Impossible d'ouvrir ghostPinkDown.png"<<endl;
                                exit(-1);
                            }
                            else
                            {
                                pixmapGhostPink.load("./data/ghosts/pink/ghostPinkDown.png");
                            }
                            break;
                        case 2:
                            if(pixmapGhostRed.load("./data/ghosts/red/ghostRedDown.png")==false)
                            {
                                cout<<"Impossible d'ouvrir ghostRedDown.png"<<endl;
                                exit(-1);
                            }
                            else
                            {
                                pixmapGhostRed.load("./data/ghosts/red/ghostRedDown.png");
                            }
                            break;
                        case 3:
                            if(pixmapGhostYellow.load("./data/ghosts/yellow/ghostYellowDown.png")==false)
                            {
                                cout<<"Impossible d'ouvrir ghostYellowDown.png"<<endl;
                                exit(-1);
                            }
                            else
                            {
                                pixmapGhostYellow.load("./data/ghosts/yellow/ghostYellowDown.png");
                            }
                            break;
                    }
					break;
			}
			compteur++;
		}
    }
}

void PacmanWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key()==Qt::Key_Left)
    {
        directionPacman = GAUCHE;
        if(pixmapPacman.load("./data/pacman/pacmanLeft.png")==false)
        {
            cout<<"Impossible d'ouvrir pacmanLeft.png"<<endl;
            exit(-1);
        }
        else
        {
            pixmapPacman.load("./data/pacman/pacmanLeft.png");
        }
    }
    else if (event->key()==Qt::Key_Right)
    {
        directionPacman = DROITE;
        if(pixmapPacman.load("./data/pacman/pacmanRight.png")==false)
        {
            cout<<"Impossible d'ouvrir pacmanRight.png"<<endl;
            exit(-1);
        }
        else
        {
            pixmapPacman.load("./data/pacman/pacmanRight.png");
        }
    }
    else if (event->key()==Qt::Key_Up)
    {
        directionPacman = HAUT;
        if(pixmapPacman.load("./data/pacman/pacmanUp.png")==false)
        {
            cout<<"Impossible d'ouvrir pacmanUp.png"<<endl;
            exit(-1);
        }
        else
        {
            pixmapPacman.load("./data/pacman/pacmanUp.png");
        }
    }
    else if (event->key()==Qt::Key_Down)
    {
        directionPacman = BAS;
        if(pixmapPacman.load("./data/pacman/pacmanDown.png")==false)
        {
            cout<<"Impossible d'ouvrir pacmanDown.png"<<endl;
            exit(-1);
        }
        else
        {
            pixmapPacman.load("./data/pacman/pacmanDown.png");
        }
    }

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

    static int temps = 0;
    temps++;
    if (temps % 2 == 0)
    {
        pixmapPacman.load("./data/pacman/pacmanFull.png");
    }
    else
    {
        switch(directionPacman)
        {
            case GAUCHE:
                pixmapPacman.load("./data/pacman/pacmanLeft.png");
                break;
            case DROITE:
                pixmapPacman.load("./data/pacman/pacmanRight.png");
                break;
            case HAUT:
                pixmapPacman.load("./data/pacman/pacmanUp.png");
                break;
            case BAS:
                pixmapPacman.load("./data/pacman/pacmanDown.png");
                break;
        }
    }

    jeu.deplacePacman(directionPacman);

    update();
}

void PacmanWindow::winOrGameOver(QPainter *painter, endGame end)
{
    // Ajout du nouveau record si record
    jeu.setHighscores();

    // Suppression texte
    TagLife->hide();
    TagScore->hide();
    Cerise->hide();
    jeu.setNbVie(0);

    // Image de fond game over
    QRectF target(0, 0, 864, 608); // 27*32 & 19*32
    QRectF source(0, 0, 864, 608);
    QImage imageEnd;

    if(end == WIN)
    {
        imageEnd.load("./data/you_win.png");
    }
    else
    {
        imageEnd.load("./data/game_over.png");
    }
    
    painter->drawImage(target, imageEnd, source);

    // Bouger score
    QFont font("arcadepix", 32);
    int printScoreWidth = 170;
    int printScoreHeight = 40;
    int printScoreX;
    if(jeu.getScore() >= 1000)
        printScoreX = (target.width() - printScoreWidth) / 2 - 10;
    else
        printScoreX = (target.width() - printScoreWidth) / 2 + 10;
    int printScoreY = (target.height() - printScoreHeight) / 2 + 15;
    printScore->move(printScoreX, printScoreY);
    printScore->setFont(font);

    // Création de bouton Yes et No
    createButtonYesNo(painter, &target);

    // Update
    update();
}

void PacmanWindow::createButtonYesNo(QPainter *painter, QRectF *target)
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

void PacmanWindow::welcome(QPainter *painter)
{
    // Suppression texte
    TagLife->hide();
    TagScore->hide();
    Cerise->hide();
    printScore->hide();
    jeu.setNbVie(0);

    // Image de fond game over
    QRectF target(0, 0, 864, 608); // 27*32 & 19*32
    QRectF source(0, 0, 864, 608);
    QImage imageWelcome("./data/play_game.png");
    painter->drawImage(target, imageWelcome, source);

    // Création du bouton play game
    createButtonPlayGame(painter, &target);

    // Update
    update();
}

void PacmanWindow::createButtonPlayGame(QPainter *painter, QRectF *target)
{
    // Ajout du bouton
    int buttonWidth = 260;
    int buttonHeight = 80;
    int buttonPlayGameX = (target->width() - buttonWidth) / 2;
    int buttonPlayGameY = (2.8* target->height() / 4) - (buttonHeight / 2);
    QRect buttonPlayGameRect(buttonPlayGameX, buttonPlayGameY, buttonWidth, buttonHeight);

    // Style du bouton
    QBrush brushPlayGame(QColor(0, 0, 0));
    QPen penPlayGame(Qt::NoPen);
    QFont font("arcadepix", 22);
    painter->setBrush(brushPlayGame);
    painter->setPen(penPlayGame);
    painter->setFont(font);
    painter->drawRoundedRect(buttonPlayGameRect, 5, 5);
    painter->setPen(QColor(255, 255, 255));
    painter->drawText(buttonPlayGameRect, Qt::AlignCenter, "Play Game");

    // Assimilation du bouton visuel crée à un vrai bouton
    PacmanButton *buttonPlayGame = new PacmanButton(this);
    buttonPlayGame->setGeometry(buttonPlayGameRect);
    buttonPlayGame->setFlat(true);
    buttonPlayGame->setCursor(Qt::PointingHandCursor);
    buttonPlayGame->show();

    // Connexion du premier bouton à une fonction
    connect(buttonPlayGame, &QPushButton::clicked, this, &PacmanWindow::clickButtonPlayGame);
}

void PacmanWindow::clickButtonPlayGame()
{
    doWelcome = false;

    QFont Arcade("arcadepix", 20, 1);
    int printScoreX = 500;
    int printScoreY = 8 + jeu.getNbCasesY() * pixmapMurVertical.height();
    printScore->move(printScoreX, printScoreY);
    printScore->setFont(Arcade);

    TagLife->show();
    TagScore->show();
    Cerise->show();
    printScore->show();
    printHighscores->hide();

    jeu.init();
    update();
}

void PacmanWindow::loadImages()
{
    if (pixmapPacman.load("./data/pacman/pacmanFull.png")==false)
    {
        cout<<"Impossible d'ouvrir pacman.png"<<endl;
        exit(-1);
    }

    if (pixmapGhostAzure.load("./data/ghosts/azure/ghostAzureDown.png")==false)
    {
        cout<<"Impossible d'ouvrir ghostAzureDown.png"<<endl;
        exit(-1);
    }

    if (pixmapGhostPink.load("./data/ghosts/pink/ghostPinkDown.png")==false)
    {
        cout<<"Impossible d'ouvrir ghostPinkDown.png"<<endl;
        exit(-1);
    }

	if (pixmapGhostRed.load("./data/ghosts/red/ghostRedDown.png")==false)
    {
        cout<<"Impossible d'ouvrir ghostRedDown.png"<<endl;
        exit(-1);
    }

	if (pixmapGhostYellow.load("./data/ghosts/yellow/ghostYellowDown.png")==false)
    {
        cout<<"Impossible d'ouvrir ghostYellowDown.png"<<endl;
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
}

PacmanButton::PacmanButton(QWidget *pParent): QPushButton(pParent)
{
	
}

void PacmanButton::keyPressEvent(QKeyEvent *e)
{
	if (parent() != nullptr)
		QCoreApplication::sendEvent(parent(), e);
}