#include <ctime>
#include <iostream>
#include "pacmanwindow.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    cout<<"Lancement du jeu"<<endl;
    QApplication app(argc, argv);

    cout<<"Initialisation du timer"<<endl;
    srand(time(0));

    cout<<"Creation de la fenetre"<<endl;
    PacmanWindow wnd;

    wnd.show();

    return app.exec();
}
