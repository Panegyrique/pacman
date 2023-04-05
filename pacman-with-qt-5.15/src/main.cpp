#include <ctime>
#include <iostream>
#include "pacmanwindow.hpp"

using namespace std;

int main(int argc, char *argv[])
{
	cout<<"Lancement du programme..."<<endl;
	QApplication app(argc, argv);
    srand(time(0));
	cout<<"Chargement Pacman..."<<endl;
    PacmanWindow wnd;
    wnd.show();
    cout<<"Game 1"<<endl;
    cout<<"Game 2"<<endl;
    cout<<"Game 1"<<endl;
    cout<<"Game 1"<<endl;

    return app.exec();
}
