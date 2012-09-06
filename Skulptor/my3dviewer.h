#ifndef STARTERCODE_H
#define STARTERCODE_H

#include <QtGui/QMainWindow>
#include <qmenubar.h>
#include <qinputdialog.h>
#include <qmessagebox.h>
#include "ui_my3dviewer.h"


class My3DViewer : public QMainWindow
{
	Q_OBJECT

public:
	My3DViewer(QWidget *parent = 0, Qt::WFlags flags = 0);
	~My3DViewer();
	bool* edgeAddMode;
	HalfEdge* edgeAddHE;
	QMenuBar* menuBar;
	QMessageBox* camhelpbox;
	QMessageBox* ghelpbox;
	QMessageBox* dhelpbox;

public slots:
	void changeFaceColor();
	void deleteVert();
	void splitFace();
	void addVert();
	void changeVertPos();
	void edgeAdd();
	void showHelpInfo();
	void changeFaceR(int);
	void changeFaceG(int);
	void changeFaceB(int);
	void setColorSliders(Face* f);

private:
	Ui::My3DViewerClass ui;
};

#endif // STARTERCODE_H
