#include <iostream>
#include <cmath>
#include <thread>
#include "pacmanwindow.hpp"

using namespace std;

// Constructeur de notre class PacmanWindow
PacmanWindow::PacmanWindow(QWidget *pParent, Qt::WindowFlags flags):QFrame(pParent, flags)
{
    // Chargement des images
    loadImages();

    // Largeur et hauteur de case
    int largeurCase, hauteurCase;
    largeurCase = pixmapMurVertical.width();
    hauteurCase = pixmapMurVertical.height();
	
    // Initialisation du jeu
    jeu.init();
	GameOver = false;

    // Initialisation du timer gerant le rafraichissement de la fenetre
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &PacmanWindow::handleTimer);
    timer->start(0.1);
	
    // Initialisation du timer propre au deplacement des entites 
	move = new QTimer(this);
    connect(move, &QTimer::timeout, this, &PacmanWindow::moveTimer);
	move->start(10);

    // Redimensionnement de la fenetre au dimension de notre carte de jeu
    resize(jeu.getNbCasesX()*largeurCase, DECALAGE_Y_BANNIERE + jeu.getNbCasesY()*hauteurCase);
	
    
    // Initialisation d'un police d'ecriture dansle style retro
	QFontDatabase *MyFontDatabase = new QFontDatabase();
	MyFontDatabase->addApplicationFont("./data/arcadepi.ttf");
	QFont Arcade("arcadepix", 20, 1);

    // Creation d'un autre QFont de la meme police mais de taille plus importante (utilite pour l'affichage du highscores)
	QFont ArcadeHighscores("arcadepix", 26); 
	
    // Initialisation de tout les labels et string qui permettent de comprendre l'avancement du jeu
    // Label LIFES
	TagLife = new QLabel(this);
    TagLife->setStyleSheet("background-color:black");
    TagLife->setGeometry(10,8+jeu.getNbCasesY()*hauteurCase,140,40);
    TagLife->setText("<font color='white'>LIFES<\font>");
    TagLife->setFont(Arcade);
	
    // Label SCORE
	TagScore = new QLabel(this);
    TagScore->setStyleSheet("background-color:black");
    TagScore->setGeometry(360,8+jeu.getNbCasesY()*hauteurCase,140,40);
    TagScore->setText("<font color='white'>SCORE<\font>");
    TagScore->setFont(Arcade);
	
    // Label du score reel (un nombre)
	printScore = new QLabel(this);
    printScore->setStyleSheet("background-color:black");
    printScore->setGeometry(500,8+jeu.getNbCasesY()*hauteurCase,170,40);
	score = QString("<font color='yellow'>") + QString::number(jeu.getScore()) + QString("<\font>");
    printScore->setText(score);
    printScore->setFont(Arcade);
	
    // Label de la cerise (dessin plus parlant qu'un texte)
	Cerise = new QLabel(this);
	Cerise->setGeometry(jeu.getNbCasesX()*largeurCase-80,8+jeu.getNbCasesY()*hauteurCase,PIXELS,PIXELS);
	Cerise->setPixmap(pixmapCerise);
	
    // Label du nombre de cerise mangee
	printCerise = new QLabel(this);
	printCerise->setStyleSheet("background-color:black");
    printCerise->setGeometry(jeu.getNbCasesX()*largeurCase-40,8+jeu.getNbCasesY()*hauteurCase,PIXELS,PIXELS);
	cerise = QString("<font color='white'>") + QString::number(jeu.getEatenCerise()) + QString("<\font>");
    printCerise->setText(cerise);
    printCerise->setFont(Arcade);
	
	// Creation de variables afin de centrer le label du highscores au centre de la fenetre
    QRectF target(0, 0, 864, 608); // Target de la dimension de notre fenetre
    int highscoresWidth = 170;
    int highscoresHeight = 40;
    int highscoresX;
    if(jeu.getHighscores() >= 1000) // Condition pour centrer correctement le highscore en fonction du nombre de chiffre
        highscoresX = (target.width() - highscoresWidth) / 2 + 20;
    else
        highscoresX = (target.width() - highscoresWidth) / 2 + 40;
    int highscoresY = (3.6 * target.height() / 4) - (highscoresHeight / 2);

    // Label du highscores (un nombre)
    printHighscores = new QLabel(this);
    printHighscores->setStyleSheet("background-color:black");
    printHighscores->setGeometry(highscoresX, highscoresY, highscoresWidth, highscoresHeight);
	highscores = QString("<font color='yellow'>") + QString::number(jeu.getHighscores()) + QString("<\font>");
    printHighscores->setText(highscores);
    printHighscores->setFont(ArcadeHighscores);
}

// Methode permettant de gerer le changement de l'affichage
void PacmanWindow::paintEvent(QPaintEvent *)
{
    // Objet QPainter permettant de dessiner dans la fenetre
    QPainter painter(this);
	
	//Fond du jeu en noir (car utilisation d'image avec fond transparent)
    painter.fillRect(0, 0, 27*PIXELS, 19*PIXELS+50, Qt::black);
    painter.beginNativePainting();
    
    
    // Largeur et hauteur de case
    int largeurCase, hauteurCase;
    largeurCase = pixmapMurVertical.width();
    hauteurCase = pixmapMurVertical.height();

    // Dessine les cases
    int x, y;
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
				case PASS:
					painter.drawPixmap(x*largeurCase, y*hauteurCase, pixmapBarreSpawn);
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
	static int temps = 0;
    const list<Fantome> &fantomes = jeu.getFantomes();
    list<Fantome>::const_iterator itFantome;
    for (itFantome=fantomes.begin(); itFantome!=fantomes.end(); itFantome++){
		if(itFantome->getFear() == false) // Condition pour differencier l'etat mangeable ou non des fantomes
        {
            // Compteur permettant de boucler dans un ordre precis la creation des fantomes par couleur
			if(compteur >= 4)
				compteur = 0;

			switch(compteur)
			{
				case 0: // 0 correspond a Azure
					painter.drawPixmap(itFantome->getPosX()*largeurCase, itFantome->getPosY()*hauteurCase, pixmapGhostAzure);
					break;
				case 1: // 1 correspond a Rose
					painter.drawPixmap(itFantome->getPosX()*largeurCase, itFantome->getPosY()*hauteurCase, pixmapGhostPink);
					break;
				case 2: // 2 correspond a Rouge
					painter.drawPixmap(itFantome->getPosX()*largeurCase, itFantome->getPosY()*hauteurCase, pixmapGhostRed);
					break;
				case 3: // 3 correspond a Jaune
					painter.drawPixmap(itFantome->getPosX()*largeurCase, itFantome->getPosY()*hauteurCase, pixmapGhostYellow);
					break;
			}
		}
        else // Fantomes mangeable par le pacman
		{
			temps++;
            // A la fin de la duree de l'etat mangeable implementation d'une condition permettant le cignotement des fantomes
            // Alternance entre bleu et blanc
			if(temps%1000<1000/2 && jeu.getPowerTime()<2000)
				painter.drawPixmap(itFantome->getPosX()*largeurCase, itFantome->getPosY()*hauteurCase, pixmapGhostWhite);
			else
				painter.drawPixmap(itFantome->getPosX()*largeurCase, itFantome->getPosY()*hauteurCase, pixmapGhostBlue);
		}
		compteur++; // Incrementation du compteur pour passer a la couleur de fantome suivante
	}
	
	// Dessine Pacman
	painter.drawPixmap(jeu.getPacmanX()*largeurCase, jeu.getPacmanY()*hauteurCase, pixmapPacman);

	// Dessine la cerise
	if(jeu.getCerise() == true)
		painter.drawPixmap(POSX_FRUIT*largeurCase, POSY_FRUIT*hauteurCase, pixmapCerise);
	
	// Dessine les vies
    for(int i=0;i<jeu.getNbVie();i++)
        painter.drawPixmap(150+i*35, 9+jeu.getNbCasesY()*hauteurCase , pixmapVie);

    // Changement de direction des fantomes ?
    directionGhosts();

    // Menu Init ?
    if(doWelcome == true)
        welcome(&painter);
	else
	{
		// Game Over ?
		if(jeu.getNbVie() == 0 && YouWin == false)
        {
			winOrGameOver(&painter, GAMEOVER);
			if(!GameOver)
            {
				generateSound("./data/sound/death.mp3"); // Lancement du son correspondant à une fin de partie
				GameOver = true;
			}
		}

		// You Win ?
		if(jeu.getNbDot() == 0 && GameOver == false) // Partie gagne quand tout les dots sont manges
        {
			winOrGameOver(&painter, WIN);
            if(!YouWin)
            {
				generateSound("./data/sound/youwin.mp3"); // Lancement du son correspondant à une fin de partie
				YouWin = true;
			}
        }
	}
}


// Gestion de la direction du pacman au travers du clavier
void PacmanWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key()==Qt::Key_Left)
        directionPacman = GAUCHE;
    else if (event->key()==Qt::Key_Right)
        directionPacman = DROITE;
    else if (event->key()==Qt::Key_Up)
        directionPacman = HAUT;
    else if (event->key()==Qt::Key_Down)
        directionPacman = BAS;
    update();
}


// Gestion de la direction des yeux en fonction de leur direction de deplacement
void PacmanWindow::directionGhosts()
{
    const list<Fantome> &fantomes = jeu.getFantomes();
    list<Fantome>::const_iterator itFantome;
    int compteur = 0;

    for (itFantome=fantomes.begin(); itFantome!=fantomes.end(); itFantome++)
    {
		if(itFantome->getFear() == false)
        {
			if(compteur >= 4) // Meme principe que precedement pour respecter l'ordre des couleurs
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
                            break;
                        case 1:
                            if(pixmapGhostPink.load("./data/ghosts/pink/ghostPinkLeft.png")==false)
                            {
                                cout<<"Impossible d'ouvrir ghostPinkLeft.png"<<endl;
                                exit(-1);
                            }
                            break;
                        case 2:
                            if(pixmapGhostRed.load("./data/ghosts/red/ghostRedLeft.png")==false)
                            {
                                cout<<"Impossible d'ouvrir ghostRedLeft.png"<<endl;
                                exit(-1);
                            }
                            break;
                        case 3:
                            if(pixmapGhostYellow.load("./data/ghosts/yellow/ghostYellowLeft.png")==false)
                            {
                                cout<<"Impossible d'ouvrir ghostYellowLeft.png"<<endl;
                                exit(-1);
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
                            break;
                        case 1:
                            if(pixmapGhostPink.load("./data/ghosts/pink/ghostPinkRight.png")==false)
                            {
                                cout<<"Impossible d'ouvrir ghostPinkRight.png"<<endl;
                                exit(-1);
                            }
                            break;
                        case 2:
                            if(pixmapGhostRed.load("./data/ghosts/red/ghostRedRight.png")==false)
                            {
                                cout<<"Impossible d'ouvrir ghostRedRight.png"<<endl;
                                exit(-1);
                            }
                            break;
                        case 3:
                            if(pixmapGhostYellow.load("./data/ghosts/yellow/ghostYellowRight.png")==false)
                            {
                                cout<<"Impossible d'ouvrir ghostYellowRight.png"<<endl;
                                exit(-1);
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
                            break;
                        case 1:
                            if(pixmapGhostPink.load("./data/ghosts/pink/ghostPinkUp.png")==false)
                            {
                                cout<<"Impossible d'ouvrir ghostPinkUp.png"<<endl;
                                exit(-1);
                            }
                            break;
                        case 2:
                            if(pixmapGhostRed.load("./data/ghosts/red/ghostRedUp.png")==false)
                            {
                                cout<<"Impossible d'ouvrir ghostRedUp.png"<<endl;
                                exit(-1);
                            }
                            break;
                        case 3:
                            if(pixmapGhostYellow.load("./data/ghosts/yellow/ghostYellowUp.png")==false)
                            {
                                cout<<"Impossible d'ouvrir ghostYellowUp.png"<<endl;
                                exit(-1);
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
                            break;
                        case 1:
                            if(pixmapGhostPink.load("./data/ghosts/pink/ghostPinkDown.png")==false)
                            {
                                cout<<"Impossible d'ouvrir ghostPinkDown.png"<<endl;
                                exit(-1);
                            }
                            break;
                        case 2:
                            if(pixmapGhostRed.load("./data/ghosts/red/ghostRedDown.png")==false)
                            {
                                cout<<"Impossible d'ouvrir ghostRedDown.png"<<endl;
                                exit(-1);
                            }
                            break;
                        case 3:
                            if(pixmapGhostYellow.load("./data/ghosts/yellow/ghostYellowDown.png")==false)
                            {
                                cout<<"Impossible d'ouvrir ghostYellowDown.png"<<endl;
                                exit(-1);
                            }
                            break;
                    }
					break;
			}
			compteur++; // Incrementation du compteur pour passer a la couleur de fantome suivante
		}
    }
}


// Timer gerant le rafraichissement de la fenetre 
void PacmanWindow::handleTimer()
{
	static int memoireCerise = 0; // Servira de compteur pour ne pas repeter le son en boucle
    jeu.evolue();

    // Mise a jour du score global
	score = QString("<font color='yellow'>") + QString::number(jeu.getScore()) + QString("<\font>");
    printScore->setText(score);

    // Mise a jour du nombre de cerise mangee
	cerise = QString("<font color='white'>") + QString::number(jeu.getEatenCerise()) + QString("<\font>");
    printCerise->setText(cerise);

    // Lancement d'un son quand un super pac-gomme est mange
	if(jeu.getPowerTime() == POWERTIME - 1) 
        generateSound("./data/sound/voila-j-aime-bien.mp3");

    // Lancement d'un son quand une cerise est mangee
	if(jeu.getEatenCerise()>memoireCerise){
		generateSound("./data/sound/eatfruit.mp3");
		memoireCerise++;
	}

    // Lancement d'un son quand un fantome est mangee
	if(jeu.getEatGhost()){
		generateSound("./data/sound/eatghost.mp3");
	    jeu.setEatGhost(false);
	}
		
    update();
}


// Timer propre au deplacement des entites
void PacmanWindow::moveTimer()
{
	int vitesseFantome, vitessePacman;
	if(jeu.getPowerTime() > 0) // Definition d'un vitesse par defaut pour les fantomes et le pacman quand un super pac gomme est mange
    {
		move->setInterval(10);
		vitesseFantome = 25; vitessePacman = 16;
	}
	else // Sinon vitesse evolutive en fonction de l'avancement dans le jeu (nombre de dot mange)
    {
		move->setInterval(7-5/238*(238-jeu.getNbDot()));
		vitesseFantome = 20; vitessePacman = 20;
	}

    static int temps = 0;
    temps++;
	
	if(temps % vitesseFantome == 0) // Permet de dissocier la vitesse du pacman et des fantomes
		jeu.moveGhost();   
    if (temps % vitessePacman < vitessePacman/2) // Permet d'alterner entre pacman plein et un pacman ouvert donne illusion de manger
        pixmapPacman.load("./data/pacman/pacmanFull.png");
    else if (temps % vitessePacman == vitessePacman/2)
    {
		Direction actualDir = jeu.deplacePacman(directionPacman); // Recuperation du deplacment pour alterner dans le bon sens
        switch(actualDir)
        {
            case GAUCHE:
                if(pixmapPacman.load("./data/pacman/pacmanLeft.png")==false)
				{
					cout<<"Impossible d'ouvrir pacmanLeft.png"<<endl;
					exit(-1);
				}
                break;
            case DROITE:
                if(pixmapPacman.load("./data/pacman/pacmanRight.png")==false)
				{
					cout<<"Impossible d'ouvrir pacmanRight.png"<<endl;
					exit(-1);
				}
                break;
            case HAUT:
                if(pixmapPacman.load("./data/pacman/pacmanUp.png")==false)
				{
					cout<<"Impossible d'ouvrir pacmanUp.png"<<endl;
					exit(-1);
				}
                break;
            case BAS:
                if(pixmapPacman.load("./data/pacman/pacmanDown.png")==false)
				{
					cout<<"Impossible d'ouvrir pacmanDown.png"<<endl;
					exit(-1);
				}
                break;
        }
    }
    update();
}


// Permet de lancer et de jouer un son
void PacmanWindow::generateSound(const char* filename) 
{
	SoundGenerator sg;
    FMOD_SOUND *sound = nullptr;
    sg.result = FMOD_System_CreateSound(sg.system, filename, FMOD_DEFAULT, 0, &sound);
	if (sg.result != FMOD_OK) 
    {
        // Affiche une erreur si le fichier audio n'a pas pu être chargee
        cout<<"Impossible de charger le fichier audio : "<<FMOD_ErrorString(sg.result)<<endl;
        return;
    }

    FMOD_CHANNEL *channel = nullptr;
    sg.result = FMOD_System_PlaySound(sg.system, sound, sg.channelgroup, 0, &channel);
	if (sg.result != FMOD_OK) 
    {
        // Affiche une erreur si le son n'a pas pu être jouee
        cout<<"Impossible de jouer le son : "<<FMOD_ErrorString(sg.result)<<endl;
        return;
    }

    // Lancement d'un thread permettant de continuer le programme le son se lance et le programme continu
    std::thread musicThread(&FMOD_Channel_SetPaused, channel, false);

    // A la fin du son on arrete le thread
    musicThread.detach();
}


// Permet d'afficher le menu d'accueil
void PacmanWindow::welcome(QPainter *painter)
{
    // Suppression des labels et strings inutile
    TagLife->hide();
    TagScore->hide();
    Cerise->hide();
    printScore->hide();
	printCerise->hide();
    jeu.setNbVie(0);

    // Affiche de l'image d'accueil
    QRectF target(0, 0, 864, 608);
    QRectF source(0, 0, 864, 608);
    QImage imageWelcome("./data/play_game.png");
    painter->drawImage(target, imageWelcome, source);

    // Creeation du bouton Play Game
    createButtonPlayGame(painter, &target);

    // Update
    update();
}


// Permet la creation d'un bouton Play Game
void PacmanWindow::createButtonPlayGame(QPainter *painter, QRectF *target)
{
    // Creation d'un rectangle qui sera plus tard le bouton
    int buttonWidth = 260;
    int buttonHeight = 80;
    int buttonPlayGameX = (target->width() - buttonWidth) / 2;
    int buttonPlayGameY = (2.8* target->height() / 4) - (buttonHeight / 2);
    QRect buttonPlayGameRect(buttonPlayGameX, buttonPlayGameY, buttonWidth, buttonHeight);

    // Ajout de design et de texte au rectangle
    QBrush brushPlayGame(QColor(0, 0, 0));
    QPen penPlayGame(Qt::NoPen);
    QFont font("arcadepix", 22);
    painter->setBrush(brushPlayGame);
    painter->setPen(penPlayGame);
    painter->setFont(font);
    painter->drawRoundedRect(buttonPlayGameRect, 5, 5);
    painter->setPen(QColor(255, 255, 255));
    painter->drawText(buttonPlayGameRect, Qt::AlignCenter, "Play Game");

    // Assimilation du rectangle creee a un vrai bouton
    PacmanButton *buttonPlayGame = new PacmanButton(this);
    buttonPlayGame->setGeometry(buttonPlayGameRect);
    buttonPlayGame->setFlat(true);
    buttonPlayGame->setCursor(Qt::PointingHandCursor);
    buttonPlayGame->show();

    // Connexion du bouton a une fonction
    connect(buttonPlayGame, &QPushButton::clicked, this, &PacmanWindow::clickButtonPlayGame);
}


// Permet de lancer le jeu
void PacmanWindow::clickButtonPlayGame()
{
    // On ne souhaite plus afficher le menu d'accueil pour la suite
    doWelcome = false;

    // Re situe le score au bon endroit
    QFont Arcade("arcadepix", 20, 1);
    int printScoreX = 500;
    int printScoreY = 8 + jeu.getNbCasesY() * pixmapMurVertical.height();
    printScore->move(printScoreX, printScoreY);
    printScore->setFont(Arcade);

    // Affichage des label et string utile pour le suivie du jeu
    TagLife->show();
    TagScore->show();
    Cerise->show();
    printScore->show();
	printCerise -> show();

    // On cache l'affichage du highscore
    printHighscores->hide();

    // Lancement du son d'intro de pacman au lancement du jeu
	generateSound("./data/sound/intro.mp3");

    // Initialisation du jeu
    jeu.init();
    update();
}


// Permet d'afficher l'image Game Over ou You Win en fonction du parametre reçu
void PacmanWindow::winOrGameOver(QPainter *painter, endGame end)
{	
    // Mise a jour du nouveau record si record
    jeu.setHighscores();

    // Suppression des labels et strings inutile 
    TagLife->hide();
    TagScore->hide();
    Cerise->hide();
	printCerise->hide();
    jeu.setNbVie(0);

    // Image de l'image de fond
    QRectF target(0, 0, 864, 608);
    QRectF source(0, 0, 864, 608);
    QImage imageEnd;

    if(end == WIN) // Si victoire on charge l'image You Win
    {
        imageEnd.load("./data/you_win.png");
    }
    else // Sinon Game Over
    {
        imageEnd.load("./data/game_over.png");
    }

    // Affichage de l'image chargee
    painter->drawImage(target, imageEnd, source);

    // Deplacement du score pour le centrer
    QFont font("arcadepix", PIXELS);
    int printScoreWidth = 170;
    int printScoreHeight = 40;
    int printScoreX;
    if(jeu.getScore() >= 1000) // Condition pour centrer correctement le score en fonction du nombre de chiffre
        printScoreX = (target.width() - printScoreWidth) / 2 - 10;
    else
        printScoreX = (target.width() - printScoreWidth) / 2 + 10;
    int printScoreY = (target.height() - printScoreHeight) / 2 + 15;
    printScore->move(printScoreX, printScoreY);
    printScore->setFont(font);

    // Creeation de bouton Yes et No
    createButtonYesNo(painter, &target);

    // Update
    update();
}


// Permet la creation d'un bouton Yes et d'un bouton No
void PacmanWindow::createButtonYesNo(QPainter *painter, QRectF *target)
{
	// Creation d'un rectangle qui sera plus tard le premier bouton
    int buttonWidth = 150;
    int buttonHeight = 60;
    int buttonYesX = (target->width() - buttonWidth) / 2 - 100;
    int buttonYesY = (3.3 * target->height() / 4) - (buttonHeight / 2);
    QRect buttonYesRect(buttonYesX, buttonYesY, buttonWidth, buttonHeight);
    // Creation d'un rectangle qui sera plus tard le deuxieme bouton
    int buttonNoX = (target->width() - buttonWidth) / 2 + 100;
    int buttonNoY = (3.3 * target->height() / 4) - (buttonHeight / 2);
    QRect buttonNoRect(buttonNoX, buttonNoY, buttonWidth, buttonHeight);

    // Ajout de design et de texte au premier rectangle
    QBrush brushYes(QColor(0, 0, 0));
    QPen penYes(Qt::NoPen);
    QFont font("arcadepix", 16);
    painter->setBrush(brushYes);
    painter->setPen(penYes);
    painter->setFont(font);
    painter->drawRoundedRect(buttonYesRect, 5, 5);
    painter->setPen(QColor(255, 255, 0));
    painter->drawText(buttonYesRect, Qt::AlignCenter, "Yes");
    // Ajout de design et de texte au deuxieme rectangle
    QBrush brushNo(QColor(0, 0, 0));
    QPen penNo(Qt::NoPen);
    painter->setBrush(brushNo);
    painter->setPen(penNo);
    painter->setFont(font);
    painter->drawRoundedRect(buttonNoRect, 5, 5);
    painter->setPen(QColor(255, 255, 0));
    painter->drawText(buttonNoRect, Qt::AlignCenter, "No");

    // Assimilation du premier rectangle cree a un vrai bouton
    PacmanButton *buttonYes = new PacmanButton(this);
    buttonYes->setGeometry(buttonYesRect);
    buttonYes->setFlat(true);
    buttonYes->setCursor(Qt::PointingHandCursor);
    buttonYes->show();
    // Assimilation du deuxieme rectangle cree a un vrai bouton
    PacmanButton *buttonNo = new PacmanButton(this);
    buttonNo->setGeometry(buttonNoRect);
    buttonNo->setFlat(true);
    buttonNo->setCursor(Qt::PointingHandCursor);
    buttonNo->show();

    // Connexion du premier bouton a une fonction
    connect(buttonYes, &QPushButton::clicked, this, &PacmanWindow::clickButtonYes);
    // Connexion du deuxieme bouton a une fonction
    connect(buttonNo, &QPushButton::clicked, this, &PacmanWindow::clickButtonNo);
}


// Permet de relancer le jeu
void PacmanWindow::clickButtonYes()
{
    // Re situe le score au bon endroit
    QFont Arcade("arcadepix", 20, 1);
    int printScoreX = 500;
    int printScoreY = 8 + jeu.getNbCasesY() * pixmapMurVertical.height();
    printScore->move(printScoreX, printScoreY);
    printScore->setFont(Arcade);

    // Affichage des label et string utile pour le suivie du jeu
    TagLife->show();
    TagScore->show();
    Cerise->show();
	printCerise->show();

    // Lancement du son d'intro de pacman au lancement du jeu
	generateSound("./data/sound/intro.mp3");

    // Initialisation du jeu
    YouWin = false;
    GameOver = false;
    jeu.init();
    update();
}


// Permet de quitter le jeu
void PacmanWindow::clickButtonNo()
{
	exit(-1);
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
	
	if (pixmapGhostWhite.load("./data/ghosts/ghost_white.png")==false)
    {
        cout<<"Impossible d'ouvrir ghost_white.png"<<endl;
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
	
	if (pixmapBarreSpawn.load("./data/map/barre_spawn.png")==false)
    {
        cout<<"Impossible d'ouvrir barre_spawn.png"<<endl;
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

// Constructeur de la class PacmanButton
PacmanButton::PacmanButton(QWidget *pParent): QPushButton(pParent)
{
	
}

// Permet de ne pas interferer avec le KeyPressEvent du deplacment du pacman
void PacmanButton::keyPressEvent(QKeyEvent *e)
{
	if (parent() != nullptr)
		QCoreApplication::sendEvent(parent(), e);
}