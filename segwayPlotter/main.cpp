#include "segwayplotter.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QApplication::setGraphicsSystem( "opengl" );
    QCoreApplication::setOrganizationName( "Lunatech Innovations" );
    QCoreApplication::setOrganizationDomain( "lunatech.se" );
    QCoreApplication::setApplicationName( "segwayPlotter" );

    segwayPlotter w;
    w.show();

    return a.exec();
}
