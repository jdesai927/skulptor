#include "my3dviewer.h"
#include <qmenubar.h>

#include <iostream>

My3DViewer::My3DViewer(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	ui.mainWindowClass->setFocus();
	ui.mainWindowClass->setFocusPolicy(Qt::StrongFocus);
	this->setWindowTitle("Skulptor");
	QPixmap pixm("SkulptorLogo.png");
	QIcon ic(pixm);
	this->setWindowIcon(ic);
	menuBar = new QMenuBar();
	menuBar->setParent(this);
	//menuBar->setStyle();
	QRect rect(0, 0, this->geometry().width(), 25);
	menuBar->setGeometry(rect);
	//this->setStyleSheet("background-image: url(./background.png)");
	QMenu* file = menuBar->addMenu("File");
	QAction* open = file->addAction("Open...");
	file->setStyleSheet("color: #FFFFFF");
	connect(open, SIGNAL(triggered()), ui.mainWindowClass, SLOT(loadObjFile()));
	/*
	QAction* save = file->addAction("Save");
	connect(save, SIGNAL(triggered()), ui.mainWindowClass, SLOT(saveObjFile()));
	*/
	QAction* saveAs = file->addAction("Save...");
	connect(saveAs, SIGNAL(triggered()), ui.mainWindowClass, SLOT(saveObjFile()));
	QAction* loadShad = file->addAction("Load Shaders...");
	connect(loadShad, SIGNAL(triggered()), ui.mainWindowClass, SLOT(loadShaderFile()));
	QMenu* views = menuBar->addMenu("View");
	views->setStyleSheet("color: #FFFFFF");
	QAction* persp = views->addAction("Perspective");
	connect(persp, SIGNAL(triggered()), ui.mainWindowClass, SLOT(setperspectiveview()));
	QAction* top = views->addAction("Overhead");
	connect(top, SIGNAL(triggered()), ui.mainWindowClass, SLOT(setoverheadview()));
	QAction* front = views->addAction("Front");
	connect(front, SIGNAL(triggered()), ui.mainWindowClass, SLOT(setfrontview()));
	QAction* back = views->addAction("Back");
	connect(back, SIGNAL(triggered()), ui.mainWindowClass, SLOT(setbackview()));
	QAction* right = views->addAction("Right");
	connect(right, SIGNAL(triggered()), ui.mainWindowClass, SLOT(setrightview()));
	QAction* left = views->addAction("Left");
	connect(left, SIGNAL(triggered()), ui.mainWindowClass, SLOT(setleftview()));
	QAction* snaporg = views->addAction("Snap View to Origin");
	connect(snaporg, SIGNAL(triggered()), ui.mainWindowClass, SLOT(snaptoorigin()));
	QAction* setcustom = views->addAction("Save Current View");
	connect(setcustom, SIGNAL(triggered()), ui.mainWindowClass, SLOT(savecustomview()));
	QAction* snapcust = views->addAction("Snap to Saved View");
	connect(snapcust, SIGNAL(triggered()), ui.mainWindowClass, SLOT(setcustomview()));
	QAction* snapselect = views->addAction("Snap to Selected View");
	connect(snapselect, SIGNAL(triggered()), ui.mainWindowClass, SLOT(snapViewSelected()));
	QMenu* geomop = menuBar->addMenu("Geometric Operations");
	geomop->setStyleSheet("color: #FFFFFF");
	QMenu* vertmop = geomop->addMenu("Vertex");
	QAction* addvert = vertmop->addAction("Add Vertex in Edge");
	connect(addvert, SIGNAL(triggered()), this, SLOT(addVert()));
	QAction* deletevert = vertmop->addAction("Delete Selected Vertex");
	connect(deletevert, SIGNAL(triggered()), this, SLOT(deleteVert()));
	QMenu* edgemop = geomop->addMenu("Edge");
	QAction* addedge = edgemop->addAction("Add Edge between Faces");
	connect(addedge, SIGNAL(triggered()), this, SLOT(edgeAdd()));
	QMenu* facemop = geomop->addMenu("Face");
	QAction* splitface = facemop->addAction("Split Face");
	connect(splitface, SIGNAL(triggered()), this, SLOT(splitFace()));
	QAction* subdiv = facemop->addAction("Catmull-Clark Subdivision");
	connect(subdiv, SIGNAL(triggered()), ui.mainWindowClass, SLOT(subdivideMesh()));
	QMenu* help = menuBar->addMenu("Help");
	help->setStyleSheet("color: #FFFFFF");
	QAction* camhelp = help->addAction("Camera Instructions...");
	connect(camhelp, SIGNAL(triggered()), this, SLOT(showHelpInfo()));
	QAction* ghelp = help->addAction("Geometric Operation Instructions...");
	connect(ghelp, SIGNAL(triggered()), this, SLOT(showHelpInfo()));
	QAction* dhelp = help->addAction("Deformation Operation Instructions...");
	connect(dhelp, SIGNAL(triggered()), this, SLOT(showHelpInfo()));
	menuBar->setVisible(true);


	camhelpbox= new QMessageBox(this);
	camhelpbox->setGeometry(this->geometry().x()+10,this->geometry().y()+10,600,600);
	camhelpbox->setText("Camera Controls: \n\n	To rotate the camera, left click and drag \n\n	To zoom in, right click and drag \n\n Light Source Controls: \n\n	Set the X,Y,Z values of the tick boxes to set the light location \n\n Camera Reference Point: \n\n	Set the X,Y,Z values of the tick boxes to set the camera reference point\n\n Face/Edge/Vertex Selection:\n\n	Use the drop down menu below the OpenGL window to change the selection mode. \n\n	Use the mouse to click and select a face/edge/vertex \n\nViews:\n\n	Use the view menu to Set the current viewing angle and to set the camera reference point to preselected targets. \n\nShaders :\n\n	Load arbitrary shaders under the File Menu");
	camhelpbox->setStyleSheet("color: #FFFFFF");
	connect(camhelp, SIGNAL(triggered()), camhelpbox, SLOT(show()));

	ghelpbox= new QMessageBox(this);
	ghelpbox->setGeometry(this->geometry().x(),this->geometry().y(),600,600);
	ghelpbox->setText("To add a vertex in an edge: \n\n Change selection mode to edge and select an edge. Select Geometric Operations->Vertex->Add Vertex in Edge to add a vertex. You will be switched to vertex selection mode with the new vertex selected.\n\nTo delete a vertex:\n\nChange selection mode to vertex and select a vertex. Select Geometric Operations->Vertex->Delete Selected Vertex.\n\nTo move a vertex:\n\nChange selection mode to vertex. Select a vertex. Press the S key to turn on movement mode. Click and drag the mouse to move the vertex as desired. Press S again to turn off movement mode when finished. Note that when moving a vertex, faces may be split to maintain planarity/convexity.\n\nTo add an edge between two faces:\n\nChange selection mode to edge. Select an edge. Select Geometric Operations->Edge->Add Edge between the face on which the selected edge is (this is highlighted) and the nearest face containing the vertex pointed at by the edge.\n\nTo move an edge:\n\nChange selection mode to edge. Select an edge. \n\\n\nPress the S key to turn on movemement mode. Click and drag the mouse to move the edge as desired. Press S again to turn off movement mode when finished. Note that when moving an edge, faces may be split to maintain planarity/convexity.\n\nTo move a face:\n\nChange selection mode to face. Select a face. Press the S key to turn on movement mode. Click and drag the mouse to move the face as desired. Press S again to turn off movement mode when finished. Note that when moving a face, faces may be split to maintain planarity/convexity.\n\nTo split a face:\n\nChange selection mode to face. Select a face. Select Geometric Operations->Face->Split Face.\n\nTo change the color of a face:\n\nChange selection mode to face. Select a face. When selected, a face turns red, but the 'Face Color' sliders are updated to the values of the actual color of the face. Move them in order to set the RGB color values of the face. When the sliders are first moved, the red color of the selected face will be replaced by its original color.\n\nTo smoooth the mesh:\n\nSelect Geometric Operations->Face->Subdivide Mesh.");
	ghelpbox->setStyleSheet("color: #FFFFFF");
	connect(ghelp, SIGNAL(triggered()), ghelpbox, SLOT(show()));

	dhelpbox= new QMessageBox(this);
	dhelpbox->setGeometry(this->geometry().x(),this->geometry().y(),600,600);
	dhelpbox->setText("Loading an obj \n\n File->Open and select the obj file you wish to load. \n\nSaving an obj \n\nFile->Save and type in the file name you wish to save the obj file as.\n\nDeformation\n\n Choose Free Form Deformation mode. To set number of control points, set values on the spin boxes to the right. Here you have set your divisions for your control points in i, j, k. Then select a point by inputing values into Select Control Point. To start the deformation simply go over the screen and drag very slowly.\n\n Twist\n\nDefine your Control points as above and Move the bars depending on which axis you wish to twist around. ");
	dhelpbox->setStyleSheet("color: #FFFFFF");
	connect(dhelp, SIGNAL(triggered()), dhelpbox, SLOT(show()));


}

My3DViewer::~My3DViewer()
{

}

void My3DViewer::changeFaceColor() {
	Face* f = ui.mainWindowClass->currentface;
	if (f == NULL) {
		return;
	}
	float newR = QInputDialog::getDouble(NULL, "New Red Value", "Input red value: ");
	float newG = QInputDialog::getDouble(NULL, "New Green Value", "Input green value: ");
	float newB = QInputDialog::getDouble(NULL, "New Blue Value", "Input blue value: ");
	f->color = new glm::vec3(newR/255.0f, newG/255.0f, newB/255.0f);
	f->oldcolor = new glm::vec3(newR/255.0f, newG/255.0f, newB/255.0f);
	ui.mainWindowClass->currentface = NULL;
	ui.mainWindowClass->mouseOverFace = NULL;
	ui.mainWindowClass->cube->generateColorArray();
}

void My3DViewer::deleteVert() {
	Vertex* v = ui.mainWindowClass->currentvertex;
	if (v == NULL) {
		return;
	}
	int i = 0;
	vector<int> indexvec;
	while (i < ui.mainWindowClass->cube->faces->size()) {
		if (ui.mainWindowClass->cube->faces->at(i)->containsVert(v)) {
			indexvec.push_back(i);
		}
		i++;
	}
	while (!indexvec.empty()) {
		ui.mainWindowClass->cube->faces->erase(ui.mainWindowClass->cube->faces->begin() + indexvec.back());
		indexvec.pop_back();
	}
	ui.mainWindowClass->currentvertex = NULL;
	ui.mainWindowClass->cube->generateVertArray();
	ui.mainWindowClass->cube->generateIndArray();
	ui.mainWindowClass->cube->generateNormArray();
	ui.mainWindowClass->cube->generateColorArray();
	ui.mainWindowClass->updateGL();
}

void My3DViewer::splitFace() {
	Face* f = ui.mainWindowClass->currentface;
	if (f == NULL) {
		return;
	}
	ui.mainWindowClass->cube->splitQuad(f->first);
	ui.mainWindowClass->cube->generateVertArray();
	ui.mainWindowClass->cube->generateIndArray();
	ui.mainWindowClass->cube->generateColorArray();
	ui.mainWindowClass->cube->generateNormArray();
}

void My3DViewer::addVert() {
	HalfEdge* he = ui.mainWindowClass->currentedge;
	if (he == NULL) {
		return;
	}
	int facSize = ui.mainWindowClass->cube->faces->size();
	HalfEdge* ret = ui.mainWindowClass->cube->addVertex(he);
	ui.mainWindowClass->cube->verts->push_back(ret->v);
	ui.mainWindowClass->cube->halfedges->push_back(ret);
	ui.selectionModes->setCurrentIndex(2);
	ui.mainWindowClass->setselectionmode(2);
	ui.mainWindowClass->currentvertex = ret->v;
	ui.mainWindowClass->currenthevert = ret;
	ui.mainWindowClass->setFocus();
	ui.mainWindowClass->cube->generateVertArray();
	ui.mainWindowClass->cube->generateNormArray();
	ui.mainWindowClass->cube->generateColorArray();
	ui.mainWindowClass->cube->generateIndArray();
	ui.mainWindowClass->updateGL();
}

void My3DViewer::changeVertPos() {

	/* JINESH'S CODE FOR POPUP VERTEX COORDINATE SELECTION */
	Vertex* v = ui.mainWindowClass->currentvertex;
	if (v == NULL) {
		return;
	}
	int k = ui.mainWindowClass->cube->faces->size();
	float newX = QInputDialog::getDouble(NULL, "New X-Coord", "Input x: ");
	float newY = QInputDialog::getDouble(NULL, "New Y-Coord", "Input y: ");
	float newZ = QInputDialog::getDouble(NULL, "New Z-Coord", "Input z: ");
	ui.mainWindowClass->cube->moveVertex(v, newX, newY, newZ);
	ui.mainWindowClass->cube->generateVertArray();
	ui.mainWindowClass->cube->generateNormArray();
	ui.mainWindowClass->cube->generateIndArray();
	ui.mainWindowClass->cube->generateColorArray();

}

void My3DViewer::edgeAdd() {
	HalfEdge* he = ui.mainWindowClass->currentedge;
	he = ui.mainWindowClass->cube->addEdge(he->sym->f, he->f, he->sym->getPrev(), he);
	ui.selectionModes->setCurrentIndex(2);
	ui.mainWindowClass->setselectionmode(2);
	ui.mainWindowClass->currentvertex = he->v;
	ui.mainWindowClass->currenthevert = he;
	ui.mainWindowClass->movementMode = true;
	ui.mainWindowClass->setFocus();
	ui.mainWindowClass->cube->generateColorArray();
	ui.mainWindowClass->cube->generateVertArray();
	ui.mainWindowClass->cube->generateNormArray();
	ui.mainWindowClass->cube->generateIndArray();
}

void My3DViewer::showHelpInfo() {

}

void My3DViewer::changeFaceR(int r) {
	if (ui.mainWindowClass->currentface == NULL || ui.selectionModes->currentIndex() != 0) {
		return;
	}
	ui.mainWindowClass->currentface->oldcolor->x = static_cast<float>(r)/255.0f;
	*ui.mainWindowClass->currentface->color = glm::vec3(ui.mainWindowClass->currentface->oldcolor->x, ui.mainWindowClass->currentface->oldcolor->y, ui.mainWindowClass->currentface->oldcolor->z);
	ui.mainWindowClass->cube->generateColorArray();
	ui.mainWindowClass->updateGL();
}

void My3DViewer::changeFaceG(int g) {
	if (ui.mainWindowClass->currentface == NULL || ui.selectionModes->currentIndex() != 0) {
		return;
	}
	ui.mainWindowClass->currentface->oldcolor->y = static_cast<float>(g)/255.0f;
	*ui.mainWindowClass->currentface->color = glm::vec3(ui.mainWindowClass->currentface->oldcolor->x, ui.mainWindowClass->currentface->oldcolor->y, ui.mainWindowClass->currentface->oldcolor->z);
	ui.mainWindowClass->cube->generateColorArray();
	ui.mainWindowClass->updateGL();
}

void My3DViewer::changeFaceB(int b) {
	if (ui.mainWindowClass->currentface == NULL || ui.selectionModes->currentIndex() != 0) {
		return;
	}
	//ui.mainWindowClass->currentface->deselect();
	ui.mainWindowClass->currentface->oldcolor->z = static_cast<float>(b)/255.0f;
	*ui.mainWindowClass->currentface->color = glm::vec3(ui.mainWindowClass->currentface->oldcolor->x, ui.mainWindowClass->currentface->oldcolor->y, ui.mainWindowClass->currentface->oldcolor->z);
	ui.mainWindowClass->cube->generateColorArray();
	ui.mainWindowClass->updateGL();
}

void My3DViewer::setColorSliders(Face* f) {
	ui.redVal->setValue(static_cast<int>(f->oldcolor->x * 255.0f));
	ui.greenVal->setValue(static_cast<int>(f->oldcolor->y * 255.0f));
	ui.blueVal->setValue(static_cast<int>(f->oldcolor->z * 255.0f));
	*f->color = glm::vec3(255.0f, 0.0f, 0.0f);
}