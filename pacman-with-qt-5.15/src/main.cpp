#include <ctime>
#include <iostream>
#include "pacmanwindow.hpp"

using namespace std;

int main(int argc, char *argv[])
{
	cout<<"boum"<<endl;
	QApplication app(argc, argv);

    srand(time(0));

    PacmanWindow wnd;
    wnd.show();

    return app.exec();
}
