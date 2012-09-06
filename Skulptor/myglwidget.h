#ifndef MYGLWIDGET
#define MYGLWIDGET

#include "camera.h"
#include "mesh.h"
#include "face.h"
#include "glew.h"
#include <QGLWidget>
#include <QTimer>
#include <QKeyEvent>
#include "../glm/glm.hpp"
#include "MyCamera.h"
#include <qfiledialog.h>

class MyGLWidget : public QGLWidget {
	Q_OBJECT

private:
	//vertex arrays needed for drawing
	unsigned int vbo;
	unsigned int cbo;
	unsigned int nbo;
	unsigned int ibo;

	//attributes
	unsigned int positionLocation;
	unsigned int normalLocation;
	unsigned int colorLocation;

	// uniforms
	unsigned int u_projMatrixLocation;
	unsigned int u_modelMatrixLocation;
	unsigned int u_lightPosLocation;
	
	//needed to compile and link and use the shaders
	unsigned int vertexShader;
	unsigned int fragmentShader;
	unsigned int shaderProgram;


	//helper function to read shader source and put it in a char array
	//thanks to Swiftless
	char* textFileRead(const char*);

	//some other helper functions from CIS 565
	void printLinkInfoLog(int);
	void printShaderInfoLog(int);

	//stores the place where a mouse movement began
	int* lastX;
	int* lastY;
	int* button;

	//field of view
	float* fov;

	//light position
	glm::vec3* currentLightPos;

	//Camera
	Camera* cam;

	//Model and view matrices
	glm::mat4* view;
	glm::mat4* currentModel;

	//Spinning cube timer
	QTimer* timer;

	//RITVIK'S  VARIABLES
	//camera stuff
	MyCamera* mycam;
	int horizangle;
	int vertangle;
	int radius;
	void updatelightposition();
	bool spin;
	float cubeangle;
	//for face selection later
	float eyex;
	float eyey;
	float eyez;
	//mouse vars to keep track of its movement
	float prevx;
	float prevy;
	//input for refxyz vars
	float refx;
	float refy;
	float refz;
	//selectionmode
	int selectionmode; //0=face,1=edge, 2=vertex,3=mesh+
	//lightsource stuff
	float lightx;
	float lighty;
	float lightz;
	//shader file names:

	const char* fragFile; 
	const char* vertFile;

	//for onscreen selection 
	float clickedx;
	float clickedy;
	float clickedz;
	bool firstclick;
	bool newclick;
	float camx;  
	float camy; 
	float camz;
	float edge1x;
	float edge1y; 
	float edge1z;
	float edge2x;
	float edge2y; 
	float edge2z;
	float vertx;
	float verty; 
	float vertz;
	//control point selection
	int controlx;
	int controly;
	int controlz;
	Vertex* currentcontrolvert;


	float areaoft(glm::vec3* v0, glm::vec3* v1, glm::vec3* v2);
	bool doesfaceintersect(Face* f, glm::vec3* P);
	glm::vec3* pforinsertion;
	//for triangle area
	glm::vec3* v1heap;
	glm::vec3* v2heap;
	glm::vec3* v3heap;
	glm::vec3* pointofintersection;
			//RITVIK 22nd APRIL
	bool doestriangleintersect(float,float,float,
		float,float,float,
		float,float,float,
		glm::vec3* P);

public:
	float closestt;
	float currentt;
	bool* selectionMode;
	bool movementMode;
	glm::vec3* P;
	glm::vec3* F;
	glm::vec3* E;

	//Face* selectedface;//RITVIKS VARS
	Face* previousface;
	Face* currentface;
	//Vertex* previousvertex;
	HalfEdge* currenthevert;
	Vertex* currentvertex;
	//HalfEdge* previousedge;
	HalfEdge* currentedge;
	Face* mouseOverFace;
	
	Mesh* cube;
	MyGLWidget(QWidget*);
	~MyGLWidget(void);
	glm::vec3 getP(float x, float y);
	void initializeGL(void);
	void paintGL(void);
	void resizeGL(int, int);
	void createCube(glm::mat4);
	void keyPressEvent(QKeyEvent*);
	void mousePressEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	float distbetween(glm::vec3*, glm::vec3*);
	vector<Vertex*> boundingBox;
	void createBoundingBox(vector<Vertex*>);
	int boundingBox_i;
	int boundingBox_j;
	int boundingBox_k;
	Vertex* getPointFromIJK(int i, int  j, int k);
	int getCombination(int n, int r);
	void FFD();

public slots:
	//ritvik's slots
		//for setting reference
	void setrefx(int);
	void setrefy(int);
	void setrefz(int);
	void setnewref();
	void snaptoorigin();
	void setoverheadview();
	void setperspectiveview();
	void setfrontview();
	void setbackview();
	void setrightview();
	void setleftview();
	void setcustomview();
	void savecustomview();
	void setselectionmode(int);
	void saveObjOver();
	void loadShaderFile();
	void twistX(int);
	void twistY(int);
	void twistZ(int);

	void setlightx(int);
	void setlighty(int);
	void setlightz(int);
	void setlightsource();
	void snapViewSelected();
	void subdivide();
	void subdivideMesh();
	void resetedgeline();
	//mansha's slots
	void loadObjFile();
	void saveObjFile();
	void setdeformi(int);
	void setdeformj(int);
	void setdeformk(int);
	void setdeformdimensions();
	void twist();
	//selectcontrolpoints
	void selectcontrolx(int);
	void selectcontroly(int);
	void selectcontrolz(int);
	void setcurrentcontrolvertex();

	signals:

		void sendcurrentface(Face*);


};

#endif