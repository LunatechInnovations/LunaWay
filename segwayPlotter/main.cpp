#include "segwayplotter.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setGraphicsSystem( "opengl" );
    segwayPlotter w;
    w.show();

    return a.exec();
}
