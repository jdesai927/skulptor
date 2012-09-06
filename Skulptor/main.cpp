#include "my3dviewer.h"
#include <QtGui/QApplication>
#include <qsplashscreen.h>
#include <qmenubar.h>
#include <qsizepolicy.h>
#include <iostream>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QPixmap pixmap("splash.png");
    QSplashScreen splash(pixmap);
    splash.show();
	a.processEvents();
	My3DViewer w;
	
	//menuBar.show();
	w.show();
	splash.finish(&w);
	return a.exec();
}
