#include "myglwidget.h"
#include <cmath>
#include <iostream>
#include <qinputdialog.h>
#include "../glm/gtc/matrix_transform.hpp"
using namespace std;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#define DEG2RAD (M_PI/180.0)

MyGLWidget::MyGLWidget(QWidget* parent) : QGLWidget(parent) {
	//This starter code uses a timer to make a square rotate without
	//user intervention.  You can remove this.
	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(spinCube()));

	//cam = new Camera();
	currentModel = new glm::mat4();
	currentLightPos = new glm::vec3(5.0f, 5.0f, 5.0f);

	cube = Mesh::unitCube();

	boundingBox_i = 0;
	boundingBox_j = 0;
	boundingBox_k =0;
		//cube = Mesh::objFileStructure();

	boundingBox = cube->createBoundingBox(boundingBox_i,boundingBox_j,boundingBox_k);

	view = new glm::mat4(1.015, -0.573, -0.533, -0.530,
		0.000, 1.469, -0.533,  -0.530,
		-0.812, -0.717, -0.667, -0.662,
		0.000, 0.000, 7.400, 7.550);

	fov = new float(90.0f);
	P = new glm::vec3();
	E = new glm::vec3();
	F = new glm::vec3();

	selectionMode = new bool(false);



	lastX = new int;
	lastY = new int;
	button = new int;

	//We can set our vertex attributes rather than requesting their locations
	//I chose to do so in this constructor, maybe you would in your init() function?
	positionLocation = 0;
	normalLocation = 1;
	colorLocation = 2;

	//RITVIKS STUFF
	//CAMERA
	mycam = new MyCamera(500,500);
	horizangle=0;
	vertangle=0;
	radius = 10;
	spin =true;
	cubeangle=0;
	//mouse
	prevx =-999;
	prevy =-999;
	//referencepointstuff
	refx=0;
	refy=0;
	refz=0;
	//selectionmoded default = face
	selectionmode=0;
	//lightsource vars

	lightx=5;
	lighty=5;
	lightz=5;
	movementMode = false;
	//shader names
	fragFile = "diffuseFrag.frag";
	vertFile = "diffuseVert.vert";

	//for onscreen selection

	clickedx=0;
	clickedy=0;
	clickedz=0;
	firstclick=false;
	newclick=false;
	previousface=NULL;
	currentface = NULL;
	mouseOverFace = NULL;
	//previousvertex= NULL;
	currentvertex= NULL;
	//previousedge= NULL;
	currentedge= NULL;

		//RITVIK APRIL 23
	edge1x=0;
	edge1y=0; 
	edge1z=0;
	edge2x=0;
	edge2y=0; 
	edge2z=0;

	 controlx=1;
	 controly=1;
	 controlz=1;
	currentcontrolvert=NULL;


}

MyGLWidget::~MyGLWidget() {
	//Release your resources, just like you'd delete pointers
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &cbo);
	glDeleteBuffers(1, &nbo);
	glDeleteBuffers(1, &ibo);

	//Tear down the shader program in reverse of building it
	glDetachShader(shaderProgram, vertexShader);
	glDetachShader(shaderProgram, fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteProgram(shaderProgram);
}

//this is the function i referred to as "init" in my lecture - Cory
void MyGLWidget::initializeGL() {

	//Initialize GLEW to get access to latest OpenGL support on your computer
	//GLEW 1.7.0 supplied with this starter code, it's always a good idea to update
	//about once every year or so as OpenGL is updated
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		cerr << "GLEW is not initialized!";
	}

	//Create the VBOs and IBO we'll be using to render images in OpenGL
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &cbo);
	glGenBuffers(1, &nbo);
	glGenBuffers(1, &ibo);

	//Everybody does this
	glClearColor(0.75, 0.75, 0.8, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);

	//here is stuff for setting up our shaders
	//const char* fragFile = "diffuseFrag.frag";
	//const char* vertFile = "diffuseVert.vert";
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	shaderProgram = glCreateProgram();

	//load up the source, compile and link the shader program
	const char* vertSource = textFileRead(vertFile);
	const char* fragSource = textFileRead(fragFile);
	glShaderSource(vertexShader, 1, &vertSource, 0);
	glShaderSource(fragmentShader, 1, &fragSource, 0);
	glCompileShader(vertexShader);
	glCompileShader(fragmentShader);

	//For your convenience, i decided to throw in some compiler/linker output helper functions
	//from CIS 565
	GLint compiled;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		printShaderInfoLog(vertexShader);
	} 
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		printShaderInfoLog(fragmentShader);
	} 

	//set the attribute locations for our shaders
	glBindAttribLocation(shaderProgram, positionLocation, "vs_position");
	glBindAttribLocation(shaderProgram, normalLocation, "vs_normal");
	glBindAttribLocation(shaderProgram, colorLocation, "vs_color");

	//finish shader setup
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	//check for linking success
	GLint linked;
	glGetProgramiv(shaderProgram,GL_LINK_STATUS, &linked);
	if (!linked) 
	{
		printLinkInfoLog(shaderProgram);
	}

	//Get the uniform locations for our shaders, unfortunately they can not be set by us, we have
	u_modelMatrixLocation = glGetUniformLocation(shaderProgram, "u_modelMatrix");
	u_projMatrixLocation = glGetUniformLocation(shaderProgram, "u_projMatrix");
	u_lightPosLocation = glGetUniformLocation(shaderProgram, "u_lightPos");

	//Always remember that it doesn't do much good if you don't have OpenGL actually use the shaders
	glUseProgram(shaderProgram);

	// Initialize Lighting
}

//this is what i referred to as a "paint" or "draw" function in my lecture - Cory
void MyGLWidget::paintGL() {

	//Always and only do this at the start of a frame, it wipes the slate clean
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Set updated view matrix COMMENTED OUT BY RITVIK
	/*glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 eye = glm::vec3(cam->position->x, cam->position->y, cam->position->z);
	glm::vec3 up = glm::vec3(cam->up->x, cam->up->y, cam->up->z);*/
	//*view = glm::perspective(*fov, 4.0f/3.0f, 0.1f, 100.0f) * glm::lookAt(eye, center, up);
	//RITVIK
	*view = mycam->getFinalMatrix();

	/*
	cube->generateVertArray();
	cube->generateNormArray();
	cube->generateColorArray();
	cube->generateIndArray();
	*/

	//Draw cube
	createCube(*currentModel);	
	createBoundingBox(boundingBox);
	if (P != NULL && E != NULL) {
		glBegin(GL_LINES);
		glVertex3f(P->x, P->y, P->z);
		glVertex3f(E->x, E->y, E->z);
		glEnd();
	}
}
void MyGLWidget::createBoundingBox(vector<Vertex*> bound){
	if (selectionmode != 3) return;
	glEnable(GL_POINT_SMOOTH);
	for(int i =0; i<bound.size(); i++){
		glPointSize(5);
		if(currentcontrolvert!=NULL){
			if(currentcontrolvert==bound[i]){
				
				glColor3f(1.0f, 0.0f, 0.0f);
				glPointSize(15);
			}
		}
		float x = bound[i]->xyz->x;
		float y = bound[i]->xyz->y;
		float z = bound[i]->xyz->z;
		
		glBegin(GL_POINTS);
		glVertex3f(x, y, z);
		glEnd();
	}
}
void MyGLWidget::createCube(glm::mat4 model) {

	// See the art of comments? 
	// cube ///////////////////////////////////////////////////////////////////////
	//    v6----- v5
	//   /|      /|
	//  v1------v0|
	//  | |     | |
	//  | |v7---|-|v4
	//  |/      |/
	//  v2------v3	
	//	cube->generateColorArray();
	////RITVIK'S DRAWING THE RAY
	//if(firstclick){
	//	glLineWidth(1);
	//	glBegin(GL_LINES);
	//	glColor3f(0, 0, 0);
	//	//glVertex3f(0, 0, 0);
	//	//glVertex3f(10,0, 0);
	//	glVertex3f(clickedx, clickedy, clickedz);

	//	if (newclick){//slight offset so you can see when you first click
	//		camx = mycam->eyex+.001; 
	//		camy = mycam->eyey+.001;   
	//		camz = mycam->eyez+.001;}
	//	glVertex3f(camx,camy, camz);
	//	//std::cout << "CAM" << camx <<"  " << camy <<"  " << camz << std::endl;
	//	glEnd();
	//	newclick=false;
	//}

	//if in edge selection mode RITVIK APRIL 23
	if(selectionmode==1 && currentedge != NULL){
	glLineWidth(4);
	glBegin(GL_LINES);
	glColor3f(255, 255, 0);
	//	glVertex3f(0, 0, 0);
	//	glVertex3f(10,0, 0);
	glVertex3f(edge1x, edge1y, edge1z);
	glVertex3f(edge2x, edge2y, edge2z);
	glEnd();
	}

	if(selectionmode==2 && currentvertex != NULL){
  glColor3f(1.0f, 0.0f, 0.0f);
  glPointSize(15.0);
  glEnable(GL_POINT_SMOOTH);
	glBegin(GL_POINTS);
   glVertex3f(currentvertex->xyz->x,currentvertex->xyz->y,currentvertex->xyz->z);
  // glColor3f(1.0f, 0.0f, 0.0f);
   //glVertex3f(4,4,4);
	
   glEnd();

	}

	//drawlargerpointforselectedcontrolpoint
/*
		if(currentcontrolvert!=NULL){
  glColor3f(1.0f, 0.0f, 0.0f);
   glPointSize(35.0);
	glBegin(GL_POINTS);
  
   glVertex3f(currentcontrolvert->getX(),currentcontrolvert->getY(),currentcontrolvert->getZ());
  // glColor3f(1.0f, 0.0f, 0.0f);
   //glVertex3f(4,4,4);
	
   glEnd();
   */
	//glLineWidth(4);
	//glBegin(GL_LINES);
	//glColor3f(255, 255, 0);
	//glVertex3f(currentcontrolvert->getX(),currentcontrolvert->getY(),currentcontrolvert->getZ());
	//glVertex3f(10,0, 0);
	//glEnd();

	

	if (cube->getTotalVerts() == 0) {
		return;
	}

	//now we put the data into the Vertex Buffer Object for the graphics system to use
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, cube->getTotalVerts() * 4 * sizeof(float), cube->vertices, GL_STATIC_DRAW); //the square vertices don't need to change, ever,
	//while the program runs

	glBindBuffer(GL_ARRAY_BUFFER, cbo);
	//always make sure you are telling OpenGL the right size to make the buffer, color data doesn't have as much data!
	glBufferData(GL_ARRAY_BUFFER, cube->getTotalVerts() * 4 * sizeof(float), cube->colors, GL_STATIC_DRAW);	

	glBindBuffer(GL_ARRAY_BUFFER, nbo);
	glBufferData(GL_ARRAY_BUFFER, cube->getTotalVerts() * 4 * sizeof(float), cube->normals, GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, 96 * sizeof(float), normals, GL_STATIC_DRAW); //the square normals don't need to change, ever,
	//while the program runs
	//activate our three kinds of information
	glEnableVertexAttribArray(positionLocation);
	glEnableVertexAttribArray(colorLocation);
	glEnableVertexAttribArray(normalLocation);

	//we're using the vertex data first
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//define how the vertex pointer should work, in our case we're accessing floats 4 at a time with no special pattern
	glVertexAttribPointer(positionLocation, 4, GL_FLOAT, 0, 0, static_cast<char*>(0));

	//now use color data
	glBindBuffer(GL_ARRAY_BUFFER, cbo);
	glVertexAttribPointer(colorLocation, 4, GL_FLOAT, 0, 0, static_cast<char*>(0));

	//one more time with the normals
	glBindBuffer(GL_ARRAY_BUFFER, nbo);
	glVertexAttribPointer(normalLocation, 4, GL_FLOAT, 0, 0, static_cast<char*>(0));

	//the last thing we need to do is setup our indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, cube->getTotalInds() * sizeof(unsigned short), cube->indices, GL_STATIC_DRAW);

	//set the model uniform
	//GL_FALSE is used for not doing transpose operation, because glm has already provided us column major matrices.
	//if the matrices you used is row major, change this parameter to GL_TRUE.
	glUniformMatrix4fv(u_modelMatrixLocation, 1, GL_FALSE, &model[0][0]);

	glUniform3f(u_lightPosLocation, currentLightPos->x, currentLightPos->y, currentLightPos->z);

	// The current view matrix is calculated by hand/magic... you should come up with your own method to get the view matrix.
	// be ware that those glm matrices are all COLUMN MAJOR.
	glm::mat4 projM = *view;

	//set the projection matrix here, it only needs to be changed if the screen is resized otherwise it can stay the same
	glUniformMatrix4fv(u_projMatrixLocation, 1, GL_FALSE, &projM[0][0]);

	//draw the elements
	glDrawElements(GL_TRIANGLES, cube->getTotalInds(), GL_UNSIGNED_SHORT, 0);

	//shut off the information since we're done drawing
	glDisableVertexAttribArray(positionLocation);
	glDisableVertexAttribArray(colorLocation);
	/*
	delete [] cube->vertices;
	delete [] cube->colors;
	delete [] cube->indices;
	delete [] cube->normals;

	cube->vertices = 0;
	cube->colors = 0;
	cube->indices = 0;
	cube->normals = 0;
	*/
}

//This function is called automatically when the widget is resized, including the first "sizing"
//CHANGED BY RITVIK
void MyGLWidget::resizeGL(int width, int height) {
	//set the viewport, more boilerplate
	glViewport(0, 0, width, height);
	float fwidth = (float)width;
	float fheight = (float)height;
	float ratio = fwidth/fheight;
	// The current view matrix is calculated by hand/magic... you should come up with your own method to get the view matrix.
	// be ware that those glm matrices are all COLUMN MAJOR.
	glm::mat4 ProjectionMatrix= glm::mat4(1.0f);
	glm::mat4 ModelViewProjectionMatrix= glm::mat4(1.0f);
	//ProjectionMatrix*= glm::perspective(45.0f, 1.0f, 0.1f, 100.0f);
	glm::mat4 ModelViewProjectionMatrix1= glm::mat4(1.0f);
	ModelViewProjectionMatrix1= mycam->getFinalMatrix();

	glUniformMatrix4fv(u_projMatrixLocation, 1, GL_FALSE, &ModelViewProjectionMatrix1[0][0]);
}

//CHANGED TO RITVIKS METHOD
void MyGLWidget::keyPressEvent(QKeyEvent *e){

	//a good place to process keystrokes
	if (e->key()==0x01000013 ){
		glm::mat4 ModelViewProjectionMatrix= glm::mat4(1.0f);
		vertangle=vertangle-10;
		ModelViewProjectionMatrix= mycam->rotateup(horizangle,vertangle);
		glUniformMatrix4fv(u_projMatrixLocation, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	}
	//RIGHT
	else if (e->key()==0x01000014){
		glm::mat4 ModelViewProjectionMatrix= glm::mat4(1.0f);
		horizangle=horizangle+10;
		ModelViewProjectionMatrix= mycam->rotateright(horizangle,vertangle);
		glUniformMatrix4fv(u_projMatrixLocation, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	}


	//LEFT
	else if (e->key()==0x01000012){
		glm::mat4 ModelViewProjectionMatrix= glm::mat4(1.0f);
		horizangle=horizangle-10;
		ModelViewProjectionMatrix= mycam->rotateright(horizangle,vertangle);
		glUniformMatrix4fv(u_projMatrixLocation, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	}



	//DOWN
	else if (e->key()==0x01000015){
		glm::mat4 ModelViewProjectionMatrix= glm::mat4(1.0f);
		vertangle=vertangle+10;
		ModelViewProjectionMatrix= mycam->rotateup(horizangle,vertangle);
		glUniformMatrix4fv(u_projMatrixLocation, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	}

	//+
	else if (e->key()==0x2d ) {
		glm::mat4 ModelViewProjectionMatrix= glm::mat4(1.0f);
		if(radius>0){
			radius=radius+3;
		}
		ModelViewProjectionMatrix= mycam->setradius(radius);
		glUniformMatrix4fv(u_projMatrixLocation, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	}

	//-
	else if (e->key()==0x2b) {
		glm::mat4 ModelViewProjectionMatrix= glm::mat4(1.0f);
		if(radius>3){
			radius=radius-3;
		}
		ModelViewProjectionMatrix= mycam->setradius(radius);
		glUniformMatrix4fv(u_projMatrixLocation, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	}

	else if (e->key() == Qt::Key_S) {
		movementMode = !movementMode;
	}
	
	else if (e->key() == Qt::Key_L) {
		if (selectionmode != 2) return;
		cout << "mode 2" << endl;
		if (currentvertex == NULL) return;
		cout << "cv exists" << endl;
		if (currenthevert == NULL) return;
		cout << "ch exists" << endl;
		currentvertex = currenthevert->next->v;
		currenthevert = currenthevert->next;
	}

	updateGL();

}

//need to change to fit to Ritvik's Camera
glm::vec3 MyGLWidget::getP(float x, float y) {

	glm::vec3* temp = new glm::vec3(0,1,2) ;
	return *temp;
}

void MyGLWidget::mousePressEvent(QMouseEvent *event) {
    float x = event->x();
    float y = event->y();
	prevx = event->x();
	prevy = event->y();
    //std::cout << "Someone clicked " << x << ", " << y << std::endl;
    firstclick=true;
    newclick=true;
    // change to normalized device coordinates
    x = x / this->width();
    y = 1 - y / this->height();

    // TODO: update the eye position;
    // world coordinate of eye
    glm::vec3 E = glm::vec3(mycam->eyex, mycam->eyey, mycam->eyez);

    // TODO: update the U vector;
    // up vector
    glm::vec3 U = glm::vec3(mycam->upx, mycam->upy, mycam->upz);

    // Viewing vector
    glm::vec3 center = glm::vec3(mycam->truecenter->x,mycam->truecenter->y,mycam->truecenter->z); // assume we are focusing on the origin, if the center moves, it will be different.
    glm::vec3 C = (center - E)*100.0f; // implies viewing distance is |C|

    // TODO: fill the two HALF view angles.
    // Field of view half angles: theta and phi
    // theta is the half angle in x plane. Think about why theta is such a weird value here.
    // phi is the half anlge in y plane. phi = 1/2 * the value you used in glPerspective.
    float theta = glm::radians(45/2.0);
    float phi = glm::radians(45/2.0 );

    // Vector A = C x U
    glm::vec3 A = glm::cross(C, U);
    // The REAL up vector B = A x C
    glm::vec3 B = glm::cross(A, C);
    // View Center M = E + C
    glm::vec3 M = E+C;

    // V || B, but on view plane
    glm::vec3 V = B * (1.0f/(glm::length(B)) * (glm::length(C)*tan(phi)));
    // H || A, but on view plane
    // If you didn't use theta here, you can simply use the ratio between this->width() and this->height() to determine the length of H
    glm::vec3 H = A * (1.0f/(glm::length(A)) * (glm::length(C)*tan(theta)));

    // Clicking point on the screen. World Coordinate.
    glm::vec3 P = M + (2*x -1)*H + (2*y -1)*V;
    //std::cout << "The point is " <<P[0]<<", "<<P[1]<<", "<<P[2]<< std::endl;

    clickedx=P[0];
    clickedy=P[1];
    clickedz=P[2];

    //find t    vec 3 p is the vector through the plane!
    //convert P into heap 
    glm::vec3* pforinsertion = new glm::vec3(P[0],P[1],P[2]) ;
    bool somefaceintersects=false;            

    closestt=-1;

        for(int i=0; i<cube->faces->size();i++){ //for each face
            //MAKE SURE NUMSIDES IS CORRECT!!s
            
            currentt=100000;
            Face*  testface = cube->faces->at(i);
            int numsides= 1;
            HalfEdge* firsthe = testface->getFirst();
            HalfEdge* nexthe = firsthe->getNext();
            while(nexthe!=firsthe){
                numsides++;
                nexthe=nexthe->getNext();

            }

            //store first vertex of each triangle
            HalfEdge* he1 = testface->getFirst();
            Vertex* vert1= he1->getVert();
            glm::vec3 v1(0,0,0);
            v1[0]=vert1->getX();
            v1[1]=vert1->getY();
            v1[2]=vert1->getZ();
            //second vert
            HalfEdge* he2 = he1->getNext();
            Vertex* vert2= he2->getVert();
            glm::vec3 v2(0,0,0);
            v2[0]=vert2->getX();
            v2[1]=vert2->getY();
            v2[2]=vert2->getZ();
            //third vert
            HalfEdge* he3 = he2->getNext();
            Vertex* vert3= he3->getVert();
            glm::vec3 v3(0,0,0);
            v3[0]=vert3->getX();
            v3[1]=vert3->getY();
            v3[2]=vert3->getZ();
            //FEED first triangle into TRIANGLE TEST
            bool sometriangnleintersects=false;

            sometriangnleintersects=doestriangleintersect(    v1[0],
                v1[1],
                v1[2],
                v2[0],
                v2[1],
                v2[2],
                v3[0],
                v3[1],
                v3[2],
                pforinsertion);


            if(sometriangnleintersects) {
                //cout << "SELECTED A FACE IN FIRST LOOP!" <<endl;    
                somefaceintersects=true;
                if(currentface!=NULL)
                    currentface->deselect();
                previousface=currentface;
                if(currentt>closestt){
                    //cout<<"currentt = " << currentt << "closestt = " << closestt << endl;
                    closestt=currentt;
                currentface = cube->faces->at(i);
                }
                //cout<< "INTERSECTION WITH";
                //cout<< cube->faces->at(i);
                if((selectionmode==0 || selectionmode ==1) && currentface!= NULL){
                    currentface->select();
					emit(sendcurrentface(currentface));
                    //cube->generateColorArray();
                }
                updateGL();

                
            }




            //only search through other triangles if you didnt intersect the first one!
            if( !sometriangnleintersects) {
            for(int x=1; x<numsides-2;x++){//for other triangles in the face, test 

                //move the edges along
                HalfEdge* temp = he3;
                he3 = he3->getNext();
                he2 = temp;

                //builds verts 
                Vertex* vertinside2= he2->getVert();
                glm::vec3 v2in(0,0,0);
                v2in[0]=vertinside2->getX();
                v2in[1]=vertinside2->getY();
                v2in[2]=vertinside2->getZ();
                Vertex* vertinside3= he3->getVert();
                glm::vec3 v3in(0,0,0);
                v3in[0]=vertinside3->getX();
                v3in[1]=vertinside3->getY();
                v3in[2]=vertinside3->getZ();
                ///feed into triangleintersection test


                sometriangnleintersects=doestriangleintersect(    v1[0],
                    v1[1],
                    v1[2],
                    v2in[0],
                    v2in[1],
                    v2in[2],
                    v3in[0],
                    v3in[1],
                    v3in[2],
                    pforinsertion);




                if( sometriangnleintersects) {
                    //cout << "SELECTED A FACE IN SECOND LOOP!" <<endl;    
                    somefaceintersects=true;
                    if(currentface!=NULL)
                        currentface->deselect();
                    previousface=currentface;
                        if(currentt>closestt){
                            //cout<<"currentt2 = " << currentt << "closestt2 = " << closestt << endl;
                            closestt=currentt;
                currentface = cube->faces->at(i);
                }
                    //cout<< "INTERSECTION WITH";
                    //cout<< cube->faces->at(i);
                    if((selectionmode==0 || selectionmode ==1) && currentface!= NULL) {
                        currentface->select();
						emit(sendcurrentface(currentface));
                    //cube->generateColorArray();
                    }
                    updateGL();
                    break;  //if  found break out of inner loop 
                }


            }

        }


        }
    //if intersects generate new colour array
    if (somefaceintersects==true)cube->generateColorArray();
    
    if (somefaceintersects==false){
        if(currentface!=NULL)
            currentface->deselect();
        currentface=NULL;
        if(selectionmode==0 || selectionmode==1)cube->generateColorArray();
    }

    //edge + vertex selection
                if( (selectionmode==1 || selectionmode==2) && currentface!=NULL){
                    //calc normal 
            HalfEdge* firsthe = currentface->getFirst();
            HalfEdge* he1 = currentface->getFirst();
            Vertex* vert1= he1->getVert();
            glm::vec3 v1(0,0,0);
            v1[0]=vert1->getX();
            v1[1]=vert1->getY();
            v1[2]=vert1->getZ();
            //second vert
            HalfEdge* he2 = he1->getNext();
            Vertex* vert2= he2->getVert();
            glm::vec3 v2(0,0,0);
            v2[0]=vert2->getX();
            v2[1]=vert2->getY();
            v2[2]=vert2->getZ();
            //third vert
            HalfEdge* he3 = he2->getNext();
            Vertex* vert3= he3->getVert();
            glm::vec3 v3(0,0,0);
            v3[0]=vert3->getX();
            v3[1]=vert3->getY();
            v3[2]=vert3->getZ();
                    
                    glm::vec3 from2to1 = v2-v1;
                    glm::vec3 from3to1 = v3-v1;
                    glm::vec3 normal = glm::cross(from2to1,from3to1);
                    glm::vec3 P1 (pforinsertion->x,pforinsertion->y,pforinsertion->z);
                    glm::vec3 P2 (mycam->eyex,mycam->eyey,mycam->eyez);
                    //calculating t
                    float numerator = glm::dot(normal,(v1-P1));
                    float denominator = glm::dot(normal,(P2-P1));
                    float t = numerator/denominator;
                    //now checks if inside triangle
                    glm::vec3 rayvector(P2[0]-P1[0],P2[1]-P1[1],P2[2]-P1[2]);
                    pointofintersection = new glm::vec3(rayvector[0]*t+P1[0],rayvector[1]*t+P1[1],rayvector[2]*t+P1[2]);


                    //testface 
                    //firsthe = testface->getFirst();
                    
                    HalfEdge* nexthalfedge = firsthe;
                    HalfEdge* closesthe= nexthalfedge;
                    Vertex* closest = firsthe->getVert();
                    glm::vec3 stackclosest(closest->getX(),closest->getY(),closest->getZ());
                    glm::vec3 fromPtoclosest = *pointofintersection-stackclosest;
                    float fromPtoclosestx =fromPtoclosest[0];float fromPtoclosesty =fromPtoclosest[1];float fromPtoclosestz =fromPtoclosest[2];
                    float lengthPtoclosest = sqrt((pow(fromPtoclosestx,2)+pow(fromPtoclosesty,2)+pow(fromPtoclosestz,2)));
                    nexthalfedge=nexthalfedge->getNext();


                    while(nexthalfedge!=firsthe){

                        Vertex* current = nexthalfedge->getVert();
                        glm::vec3 stackcurrent(current->getX(),current->getY(),current->getZ());
                        glm::vec3 fromPtocurrent = *pointofintersection-stackcurrent;
                        float fromPtocurrentx =fromPtocurrent[0];float fromPtocurrenty =fromPtocurrent[1];float fromPtocurrentz =fromPtocurrent[2];
                        float lengthPtocurrent = sqrt((pow(fromPtocurrentx,2)+pow(fromPtocurrenty,2)+pow(fromPtocurrentz,2)));


                        if(lengthPtocurrent<lengthPtoclosest) 
                        {
                            //update shortest dist        
                            lengthPtoclosest=lengthPtocurrent;
                            //update shortest vert
                            closest = nexthalfedge->getVert();
                            closesthe=nexthalfedge;
                        }
                        nexthalfedge=nexthalfedge->getNext();

                    }

                    if(selectionmode==2){
                        currentvertex=closest;
                        currenthevert=closesthe;
                        vertx=currentvertex->getX();
                        verty=currentvertex->getY();
                        vertz=currentvertex->getZ();
                    }
                    //find number of sides in selected face

                                
            int istriangle= 1;
            HalfEdge* firsttrianglehe = currentface->getFirst();
            HalfEdge* nexttrianglehe = firsttrianglehe->getNext();
            while(nexttrianglehe!=firsttrianglehe){
                istriangle++;
                nexttrianglehe=nexttrianglehe->getNext();
            }

            cout << istriangle;
            if (istriangle==3){

                HalfEdge* firsthe= firsttrianglehe;
                HalfEdge* secondhe= firsttrianglehe->getNext();
                HalfEdge* thirdhe= secondhe->getNext();

                Vertex* firstv = firsttrianglehe->getVert();
                Vertex* secondv = secondhe->getVert();
                Vertex* thirdv = thirdhe->getVert();

                glm::vec3* mid1and2 = new glm::vec3((firstv->getX()+secondv->getX())/2,(firstv->getY()+secondv->getY())/2,(firstv->getZ()+secondv->getZ())/2);
                glm::vec3* mid2and3= new glm::vec3((thirdv->getX()+secondv->getX())/2,(thirdv->getY()+secondv->getY())/2,(thirdv->getZ()+secondv->getZ())/2);
                glm::vec3* mid3and1= new glm::vec3((thirdv->getX()+firstv->getX())/2,(thirdv->getY()+firstv->getY())/2,(thirdv->getZ()+firstv->getZ())/2);

                float distpto12 = distbetween(mid1and2,pointofintersection);
                float distpto23 = distbetween(mid2and3,pointofintersection);
                float distpto31 = distbetween(mid3and1,pointofintersection);


                if( distpto12<distpto23 && distpto12<distpto31){
                
                    edge1x=firstv->getX();
                    edge1y=firstv->getY();
                    edge1z=firstv->getZ();
                    edge2x=secondv->getX();
                    edge2y=secondv->getY(); 
                    edge2z=secondv->getZ();
                    cout<< "1" << endl;
                    currentedge=secondhe;
                }
                else if( distpto23<distpto12 && distpto23<distpto31){
                    edge1x=thirdv->getX();
                    edge1y=thirdv->getY();
                    edge1z=thirdv->getZ();
                    edge2x=secondv->getX();
                    edge2y=secondv->getY(); 
                    edge2z=secondv->getZ();

                    cout<< "2" << endl;
                    currentedge=thirdhe;
                }
                else if(distpto31<distpto12 && distpto31<distpto23){
                    edge1x=thirdv->getX();
                    edge1y=thirdv->getY();
                    edge1z=thirdv->getZ();
                    edge2x=firstv->getX();
                    edge2y=firstv->getY(); 
                    edge2z=firstv->getZ();
                    cout<< "3" << endl;
                    currentedge=firsthe;
                }
                    //edge1x=closest->getX();
        //            edge1y=closest->getY();
        //            edge1z=closest->getZ();
        //            edge2x=closest2->getX();
        //            edge2y=closest2->getY(); 
        //            edge2z=closest2->getZ();


            }

            else {
                    HalfEdge* secondhe = closesthe->getSym();
                    Vertex* second = secondhe->getVert();
                    HalfEdge* thirdhe = closesthe->getNext();
                    Vertex* third = thirdhe->getVert();
                    glm::vec3 stacksecond(second->getX(),second->getY(),second->getZ());
                    glm::vec3 fromPtosecond = *pointofintersection-stacksecond;
                    float fromPtosecondx =fromPtosecond[0];float fromPtosecondy =fromPtosecond[1];float fromPtosecondz =fromPtosecond[2];
                    float lengthPtosecond = sqrt((pow(fromPtosecondx,2)+pow(fromPtosecondy,2)+pow(fromPtosecondz,2)));

                    glm::vec3 stackthird(third->getX(),third->getY(),third->getZ());
                    glm::vec3 fromPtothird = *pointofintersection-stackthird;
                    float fromPtothirdx =fromPtothird[0];float fromPtothirdy =fromPtothird[1];float fromPtothirdz =fromPtothird[2];
                    float lengthPtothird = sqrt((pow(fromPtothirdx,2)+pow(fromPtothirdy,2)+pow(fromPtothirdz,2)));
                    Vertex* closest2=second;
                    currentedge=secondhe;
                    if( lengthPtothird<lengthPtosecond){
                        closest2=third;
                        currentedge=thirdhe;
                    }


                    edge1x=closest->getX();
                    edge1y=closest->getY();
                    edge1z=closest->getZ();
                    edge2x=closest2->getX();
                    edge2y=closest2->getY(); 
                    edge2z=closest2->getZ();
                    //cout<< "Edgexyz: " << edge1x<<" "<<edge1x<< " " <<edge1x <<endl;    
                }
            }
    updateGL();

}

bool MyGLWidget::doestriangleintersect(float v1x, 
    float v1y,
    float v1z,
    float v2x, 
    float v2y,
    float v2z,
    float v3x, 
    float v3y,
    float v3z,
    glm::vec3* P){ 
        //first find normal!
        //first vert
        //cout << "input="; //cout << v1x << " , " << v1y << " , " << v1z  << endl ;
        //cout << "input2="; cout << v2x << " , " << v2y << " , " << v2z  << endl;
        //cout << "input3="; cout << v3x << " , " << v3y << " , " << v3z  << endl;
        glm::vec3 v1(0,0,0);
        v1[0]=v1x;
        v1[1]=v1y;
        v1[2]=v1z;
        //second vert    
        glm::vec3 v2(0,0,0);
        v2[0]=v2x;
        v2[1]=v2y;
        v2[2]=v2z;
        //third vert

        glm::vec3 v3(0,0,0);
        v3[0]=v3x;
        v3[1]=v3y;
        v3[2]=v3z;

        //calc normal 
        glm::vec3 from2to1 = v2-v1;
        glm::vec3 from3to1 = v3-v1;
        //cout << "from2to1="; cout << from2to1[0]; cout << " , "; cout << from2to1[1]; cout << " , "; cout << from2to1[2]<< endl;
        //cout << "from3to1="; cout << from3to1[0]; cout << " , "; cout << from3to1[1]; cout << " , "; cout << from3to1[2]<< endl;
        glm::vec3 normal = glm::cross(from2to1,from3to1);

        //FIX THIS PART
        glm::vec3 P1 (P->x,P->y,P->z);
        //cout << "P1!="; cout << P1[0]; cout << " , "; cout << P1[1]; cout << " , "; cout << P1[2]<< endl;
        //glm::vec3 P2 (P1[0]*3,P1[1]*3,P1[2]*3);
        glm::vec3 P2 (mycam->eyex,mycam->eyey,mycam->eyez);

        //calculating t
        float numerator = glm::dot(normal,(v1-P1));
        //cout << "numerator="; cout << numerator<< endl;
        float denominator = glm::dot(normal,(P2-P1));
        //cout << "denominator="; cout << denominator<< endl;
        float t = numerator/denominator;
        
        //cout << "T="; cout << t<< endl;

        if(t<0) return false;
        

        //cout << "T="; cout << t<< endl;
        //now checks if inside triangle
        glm::vec3 rayvector(P2[0]-P1[0],P2[1]-P1[1],P2[2]-P1[2]);
        pointofintersection = new glm::vec3(rayvector[0]*t+P1[0],rayvector[1]*t+P1[1],rayvector[2]*t+P1[2]);
        //float areaoftriangle = glm:: 
        //cout << "INTERSECTION POINT="; //cout << pointofintersection->x; //cout << " , "; //cout << pointofintersection->y; //cout << " , "; //cout << pointofintersection->z << endl;
        float lenghtofray = sqrt((pow(rayvector[0],2)+pow(rayvector[1],2)+pow(rayvector[2],2)));
    



        v1heap = new glm::vec3 (v1[0],v1[1],v1[2]);
        v2heap= new glm::vec3 (v2[0],v2[1],v2[2]);
        v3heap= new glm::vec3 (v3[0],v3[1],v3[2]);
        float s = areaoft(v1heap,v2heap,v3heap);
        ////cout << "HEAP="; //cout << v1heap->x; //cout << " , "; //cout << v1heap->y; //cout << " , "; //cout << v1heap->z<< endl;
        //cout << "AREA="; //cout << s << endl;

        float s1 = (areaoft(pointofintersection,v2heap,v3heap))/s;
        float s2 = (areaoft(pointofintersection,v3heap,v1heap))/s;
        float s3 = (areaoft(pointofintersection,v2heap,v1heap))/s;

        float sum = s1+s2+s3;
        //cout << "SUM="; //cout << sum << endl;
        //cout << "s1="; //cout << s1 << endl;
        //cout << "s2="; //cout << s2 << endl;
        //cout << "s3="; //cout << s3 << endl;
        if(s1<0 || s1>1) return false;
        if(s2<0 || s2>1) return false;
        if(s3<0 || s3>1) return false;
        //cout << "before";
        if(sum>1.001 || sum<.990) {
            //cout << "FAILURE";
            return false;
        }
        currentt=t;
        return true;

}

bool MyGLWidget::doesfaceintersect(Face* f, glm::vec3* P){ 
	//first find normal!
	//first vert
	HalfEdge* he1 = f->getFirst();
	Vertex* vert1= he1->getVert();
	glm::vec3 v1(0,0,0);
	v1[0]=vert1->getX();
	v1[1]=vert1->getY();
	v1[2]=vert1->getZ();
	//second vert
	HalfEdge* he2 = he1->getNext();
	Vertex* vert2= he2->getVert();
	glm::vec3 v2(0,0,0);
	v2[0]=vert2->getX();
	v2[1]=vert2->getY();
	v2[2]=vert2->getZ();
	//third vert
	HalfEdge* he3 = he2->getNext();
	Vertex* vert3= he3->getVert();
	glm::vec3 v3(0,0,0);
	v3[0]=vert3->getX();
	v3[1]=vert3->getY();
	v3[2]=vert3->getZ();

	//calc normal 
	glm::vec3 from2to1 = v2-v1;
	glm::vec3 from3to1 = v3-v1;
	glm::vec3 normal = glm::cross(from2to1,from3to1);

	//FIX THIS PART
	glm::vec3 P1 (P->x,P->y,P->z);
	//cout << "P1!="; cout << P1[0]; cout << " , "; cout << P1[1]; cout << " , "; cout << P1[2]<< endl;
	//glm::vec3 P2 (P1[0]*3,P1[1]*3,P1[2]*3);
	glm::vec3 P2 (mycam->eyex,mycam->eyey,mycam->eyez);

	//calculating t
	float numerator = glm::dot(normal,(v1-P1));
	//cout << "numerator="; cout << numerator<< endl;
	float denominator = glm::dot(normal,(P2-P1));
	//cout << "denominator="; cout << denominator<< endl;
	float t = numerator/denominator;

	cout << "T="; cout << t<< endl;

	if(t<0) return false;

	//now checks if inside triangle
	glm::vec3 rayvector(P2[0]-P1[0],P2[1]-P1[1],P2[2]-P1[2]);
	pointofintersection = new glm::vec3(rayvector[0]*t+P1[0],rayvector[1]*t+P1[1],rayvector[2]*t+P1[2]);
	//float areaoftriangle = glm:: 
	//cout << "INTERSECTION POINT="; cout << pointofintersection->x; cout << " , "; cout << pointofintersection->y; cout << " , "; cout << pointofintersection->z << endl;

	v1heap = new glm::vec3 (v1[0],v1[1],v1[2]);
	v2heap= new glm::vec3 (v2[0],v2[1],v2[2]);
	v3heap= new glm::vec3 (v3[0],v3[1],v3[2]);
	float s = areaoft(v1heap,v2heap,v3heap);
	//cout << "HEAP="; cout << v1heap->x; cout << " , "; cout << v1heap->y; cout << " , "; cout << v1heap->z<< endl;
	//cout << "AREA="; cout << s << endl;

	float s1 = (areaoft(pointofintersection,v2heap,v3heap))/s;
	float s2 = (areaoft(pointofintersection,v3heap,v1heap))/s;
	float s3 = (areaoft(pointofintersection,v2heap,v1heap))/s;

	float sum = s1+s2+s3;
	//cout << "SUM="; cout << sum << endl;
	//cout << "s1="; cout << s1 << endl;
	//cout << "s2="; cout << s2 << endl;
	//cout << "s3="; cout << s3 << endl;
	if(s1<0 || s1>1) return false;
	if(s2<0 || s2>1) return false;
	if(s3<0 || s3>1) return false;
	//cout << "before";
	if(sum>1.001 || sum<.990) {
		//cout << "FAILURE";
		return false;
	}

	return true;

}


float MyGLWidget::areaoft(glm::vec3* v0, glm::vec3* v1, glm::vec3* v2){
	//cout << "v0="; cout << v0->x; cout << " , "; cout << v0->y; cout << " , "; cout << v0->z<< endl;
	//cout << "v1="; cout << v1->x; cout << " , "; cout << v1->y; cout << " , "; cout << v1->z<< endl;
	//cout << "v2="; cout << v2->x; cout << " , "; cout << v2->y; cout << " , "; cout << v2->z<< endl;

	//formula from www.mathopenref.com/heronsformula.html
	glm::vec3 side1 ((v1->x)-(v0->x),(v1->y)-(v0->y),(v1->z)-(v0->z));
	//cout << "side1="; cout << side1[0]; cout << " , "; cout << side1[1]; cout << " , "; cout << side1[2]<< endl;
	glm::vec3 side2((v2->x)-(v0->x),(v2->y)-(v0->y),(v2->z)-(v0->z));
	glm::vec3 side3 ((v2->x)-(v1->x),(v2->y)-(v1->y),(v2->z)-(v1->z));
	float length1= sqrt(side1[0]*side1[0] + side1[1]*side1[1] +side1[2]*side1[2]);
	//cout << "l1="; cout << length1 << endl; 
	float length2= sqrt(side2[0]*side2[0] + side2[1]*side2[1] +side2[2]*side2[2]);
	//cout << "l2="; cout << length2 << endl; 
	float length3= sqrt(side3[0]*side3[0] + side3[1]*side3[1] +side3[2]*side3[2]);
	//cout << "l3="; cout << length3 << endl; 

	float perimeter= length1+length2+length3;
	float halfperim = perimeter/2;

	float areasquared = halfperim *(halfperim-length1)*(halfperim-length2)*(halfperim-length3);
	float area = sqrt(areasquared);

	return area;
}






void MyGLWidget::mouseMoveEvent(QMouseEvent *m) {
	//if (*selectionMode) {
	//	*P = getP(e->x(), e->y());
	//	*E = glm::vec3(cam->position->x, cam->position->y, cam->position->z);
	//	Face* shortest = cube->findIntersectedFace(*P, *E);
	//	//if (shortest == selectedFace) {
	//	//	updateGL();
	//	//	return;
	//	//}
	//	if (shortest != NULL) {
	//		*F = *P + (shortest->intersectsWith(*P, *E)) * *E;
	//		if (mouseOverFace != NULL) {
	//			*mouseOverFace->color = glm::vec3(mouseOverFace->color->x - 0.2, 
	//											  mouseOverFace->color->y - 0.2, 
	//											  mouseOverFace->color->z - 0.2);
	//		}
	//		mouseOverFace = shortest;
	//		*mouseOverFace->color = glm::vec3(shortest->color->x + 0.2, 
	//										  shortest->color->y + 0.2, 
	//										  shortest->color->z + 0.2);
	//		cube->generateColorArray();
	//	}
	//	updateGL();
	//	return;
	//}




	//MOUSEMOVEMENTS by RITVIK
	if(m->buttons()==0x00000001){
		// Jinesh's primitive vertex moving code
		if (selectionmode == 2 && currentvertex != NULL && currenthevert != NULL && movementMode) {
			glm::vec3 eyev = glm::vec3(mycam->eyex, mycam->eyey, mycam->eyez);
			glm::vec3 yaxis = -1.0f * glm::normalize(glm::vec3(mycam->upx, mycam->upy, mycam->upz));
			glm::vec3 xaxis = -1.0f * glm::normalize(glm::cross(yaxis, eyev));
			glm::vec3 newpos = *currentvertex->xyz + (yaxis * static_cast<float>(m->y() - prevy) * 0.01f) + (xaxis * static_cast<float>(m->x() - prevx) * 0.01f);
			cube->moveVertex(currentvertex, newpos.x, newpos.y, newpos.z); 
			cube->generateVertArray();
			cube->generateNormArray();
			cube->generateIndArray();
			cube->generateColorArray();
			updateGL();
			prevx = m->x();
			prevy = m->y();
			return;
		}
		if (selectionmode == 1 && currentedge != NULL && movementMode) {
			glm::vec3 eyev = glm::vec3(mycam->eyex, mycam->eyey, mycam->eyez);
			glm::vec3 yaxis = -1.0f * glm::normalize(glm::vec3(mycam->upx, mycam->upy, mycam->upz));
			glm::vec3 xaxis = -1.0f * glm::normalize(glm::cross(yaxis, eyev));
			glm::vec3 newpos1 = *currentedge->v->xyz + (yaxis * static_cast<float>(m->y() - prevy) * 0.01f) + (xaxis * static_cast<float>(m->x() - prevx) * 0.01f);
			glm::vec3 newpos2 = *currentedge->sym->v->xyz + (yaxis * static_cast<float>(m->y() - prevy) * 0.01f) + (xaxis * static_cast<float>(m->x() - prevx) * 0.01f);
			*currentedge->v->xyz = newpos1; 
			*currentedge->sym->v->xyz = newpos2;
			cube->fixPlanarity(currentedge->v);
			cube->fixPlanarity(currentedge->getPrev()->v);
			edge1x=newpos1[0];
            edge1y=newpos1[1];
            edge1z=newpos1[2];
            edge2x=newpos2[0];
            edge2y=newpos2[1]; 
            edge2z=newpos2[2];
			cube->generateVertArray();
			cube->generateNormArray();
			cube->generateIndArray();
			cube->generateColorArray();
			updateGL();
			prevx = m->x();
			prevy = m->y();
			return;
		}
		if (selectionmode == 0 && currentface != NULL && movementMode) {
			glm::vec3 eyev = glm::vec3(mycam->eyex, mycam->eyey, mycam->eyez);
			glm::vec3 yaxis = -1.0f * glm::normalize(glm::vec3(mycam->upx, mycam->upy, mycam->upz));
			glm::vec3 xaxis = -1.0f * glm::normalize(glm::cross(yaxis, eyev));
			HalfEdge* faceh = currentface->first;
			while (true) {
				*faceh->v->xyz = *faceh->v->xyz + (yaxis * static_cast<float>(m->y() - prevy) * 0.01f) + (xaxis * static_cast<float>(m->x() - prevx) * 0.01f);
				faceh = faceh->next;
				if (faceh == currentface->first) {
					break;
				}
			}
			/*while (true) {
				cube->fixPlanarity(faceh->v);
				faceh = faceh->next;
				if (faceh = currentface->first) {
					break;
				}
			}*/
			cube->generateVertArray();
			cube->generateNormArray();
			cube->generateIndArray();
			cube->generateColorArray();
			updateGL();
			prevx = m->x();
			prevy = m->y();
			return;
		}
		if (selectionmode == 3 && currentcontrolvert != NULL) {
			glm::vec3 eyev = glm::vec3(mycam->eyex, mycam->eyey, mycam->eyez);
			glm::vec3 yaxis = -1.0f * glm::normalize(glm::vec3(mycam->upx, mycam->upy, mycam->upz));
			glm::vec3 xaxis = -1.0f * glm::normalize(glm::cross(yaxis, eyev));
			*currentcontrolvert->xyz = *currentcontrolvert->xyz + (yaxis * static_cast<float>(m->y() - prevy) * 0.01f) + (xaxis * static_cast<float>(m->x() - prevx) * 0.01f);
			FFD();
			updateGL();
			prevx = m->x();
			prevy = m->y();
			return;
		}
		if(m->x()>prevx){
			glm::mat4 ModelViewProjectionMatrix= glm::mat4(1.0f);
			horizangle=horizangle+6;
			ModelViewProjectionMatrix= mycam->rotateright(horizangle,vertangle);
			if(m->y()>prevy){
				vertangle=vertangle+6;
				ModelViewProjectionMatrix= mycam->rotateup(horizangle,vertangle);
				prevy=m->y();
			}
			else if(m->y()<prevy){
				vertangle=vertangle-6;
				ModelViewProjectionMatrix= mycam->rotateup(horizangle,vertangle);
				prevy=m->y();
			}
			glUniformMatrix4fv(u_projMatrixLocation, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
			updateGL();
			prevx=m->x();
			
		}
		else if(m->x()<prevx){
			glm::mat4 ModelViewProjectionMatrix= glm::mat4(1.0f);
			horizangle=horizangle-6;
			ModelViewProjectionMatrix= mycam->rotateright(horizangle,vertangle);
			if(m->y()>prevy){
				vertangle=vertangle+6;
				ModelViewProjectionMatrix= mycam->rotateup(horizangle,vertangle);
				prevy=m->y();
			}
			else if(m->y()<prevy){
				vertangle=vertangle-6;
				ModelViewProjectionMatrix= mycam->rotateup(horizangle,vertangle);
				prevy=m->y();
			}

			glUniformMatrix4fv(u_projMatrixLocation, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
			updateGL();
			prevx=m->x();

		}

		else if(m->y()>prevy){
			glm::mat4 ModelViewProjectionMatrix= glm::mat4(1.0f);
			vertangle=vertangle+6;
			ModelViewProjectionMatrix= mycam->rotateup(horizangle,vertangle);
			glUniformMatrix4fv(u_projMatrixLocation, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
			updateGL();
			prevy=m->y();

		}
		else if(m->y()<prevy){
			glm::mat4 ModelViewProjectionMatrix= glm::mat4(1.0f);
			vertangle=vertangle-6;
			ModelViewProjectionMatrix= mycam->rotateup(horizangle,vertangle);
			glUniformMatrix4fv(u_projMatrixLocation, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
			updateGL();
			prevy=m->y();

		}

	}
	//zoom in and out
	else if(m->buttons()==0x00000002){
		if(m->y()>prevy){
			glm::mat4 ModelViewProjectionMatrix= glm::mat4(1.0f);
			if(radius>0){
				radius=radius+1;
			}
			ModelViewProjectionMatrix= mycam->setradius(radius);
			glUniformMatrix4fv(u_projMatrixLocation, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
			updateGL();
			////cout << "right";
		}
		if(m->y()<prevy){
			glm::mat4 ModelViewProjectionMatrix= glm::mat4(1.0f);
			if(radius>0){
				if(radius>3)
					radius=radius-1;
			}
			ModelViewProjectionMatrix= mycam->setradius(radius);
			glUniformMatrix4fv(u_projMatrixLocation, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
			updateGL();
			////cout << "right";
		}
		prevy=m->y();
	}
	updateGL();

}

//from swiftless.com
char* MyGLWidget::textFileRead(const char* fileName) {
	char* text;

	if (fileName != NULL) {
		FILE *file = fopen(fileName, "rt");

		if (file != NULL) {
			fseek(file, 0, SEEK_END);
			int count = ftell(file);
			rewind(file);

			if (count > 0) {
				text = (char*)malloc(sizeof(char) * (count + 1));
				count = fread(text, sizeof(char), count, file);
				text[count] = '\0';	//cap off the string with a terminal symbol, fixed by Cory
			}
			fclose(file);
		}
	}
	return text;
}

float MyGLWidget::distbetween(glm::vec3* a,glm::vec3* b){
   
	float distance;    
    glm::vec3 ray(a->x-b->x,a->y-b->y,a->z-b->z);
    distance = pow(ray[0],2)+pow(ray[1],2)+pow(ray[2],2);
    distance = sqrt(distance);
    return distance;

}

void MyGLWidget::printLinkInfoLog(int prog) 
{
	int infoLogLen = 0;
	int charsWritten = 0;
	GLchar *infoLog;

	glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &infoLogLen);

	// should additionally check for OpenGL errors here

	if (infoLogLen > 0)
	{
		infoLog = new GLchar[infoLogLen];
		// error check for fail to allocate memory omitted
		glGetProgramInfoLog(prog,infoLogLen, &charsWritten, infoLog);
		cerr << "InfoLog:" << endl << infoLog << endl;
		delete [] infoLog;
	}
}

void MyGLWidget::printShaderInfoLog(int shader)
{
	int infoLogLen = 0;
	int charsWritten = 0;
	GLchar *infoLog;

	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);

	// should additionally check for OpenGL errors here

	if (infoLogLen > 0)
	{
		infoLog = new GLchar[infoLogLen];
		// error check for fail to allocate memory omitted
		glGetShaderInfoLog(shader,infoLogLen, &charsWritten, infoLog);
		cerr << "InfoLog:" << endl << infoLog << endl;
		delete [] infoLog;
	}

	// should additionally check for OpenGL errors here
}


//Ritvik's  methods
//for  reference changes
void MyGLWidget::setrefx(int i){
	refx = i;
mycam->changereferencepoint(refx,refy,refz);
	updateGL();}
void MyGLWidget::setrefy(int i){
	refy = i;
mycam->changereferencepoint(refx,refy,refz);
	updateGL();}
void MyGLWidget::setrefz(int i){
	refz = i;
mycam->changereferencepoint(refx,refy,refz);
	updateGL();}
void MyGLWidget::setnewref(){
	mycam->changereferencepoint(refx,refy,refz);
	updateGL();
}
void MyGLWidget::snaptoorigin(){
	mycam->changereferencepoint(0,0,0);
	mycam->setradius(20);
	mycam->eyex=20;
	mycam->eyey=0;
	mycam->eyez=0;
	mycam->upx=0;
	mycam->upy=0;
	mycam->upz=1;
	updateGL();
}

void MyGLWidget::setoverheadview(){
	mycam->overheadview();
	updateGL();
}
void MyGLWidget::setperspectiveview(){

	mycam->frontview();
	vertangle=vertangle+45;
	horizangle=horizangle+45;
	glm::mat4 ModelViewProjectionMatrix= glm::mat4(1.0f);
	ModelViewProjectionMatrix= mycam->rotateright(horizangle,vertangle);		ModelViewProjectionMatrix= mycam->rotateup(horizangle,vertangle);

	glUniformMatrix4fv(u_projMatrixLocation, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	updateGL();
}
void MyGLWidget::setfrontview(){
	mycam->frontview();
	updateGL();
}
void MyGLWidget::setbackview(){
	mycam->backview();
	updateGL();
}
void MyGLWidget::setrightview(){
	mycam->rightview();
	updateGL();
}
void MyGLWidget::setleftview(){
	mycam->leftview();
	updateGL();
}
void MyGLWidget::setcustomview(){
	mycam->customview();
	updateGL();
}

void MyGLWidget::savecustomview(){
	mycam->savecustomview();
	updateGL();
}

void MyGLWidget::setselectionmode(int x){
	
	if(currentface != NULL) currentface->deselect(); //need to add this in for other modes too 
	cube->generateColorArray();
	resetedgeline();
	currentface=NULL;
	currentvertex=NULL;
	currentedge=NULL;
	currentcontrolvert=NULL;
	selectionmode=x;
	updateGL();
}

void MyGLWidget::loadShaderFile() {
	string vert = QFileDialog::getOpenFileName(NULL, tr("Load Vertex Shader"), "", tr("Vertex Shader File (*.vert)")).toStdString(); 
	vertFile = vert.c_str();
	string frag = QFileDialog::getOpenFileName(NULL, tr("Load Fragment Shader"), "", tr("Fragment Shader File (*.frag)")).toStdString(); 
	fragFile = frag.c_str();
	initializeGL();
	updateGL();
}



void MyGLWidget::setlightx(int i) {
	//cout << "XXX";
	lightx=i;
	currentLightPos=new glm::vec3(lightx,lighty,lightz);
	updateGL();
}

void MyGLWidget::setlighty(int i) {
	//cout << "yyy";
	lighty=i;
	currentLightPos=new glm::vec3(lightx,lighty,lightz);
	updateGL();
}

void MyGLWidget::setlightz(int i) {
	//cout << "zzz";
	lightz=i;
	currentLightPos=new glm::vec3(lightx,lighty,lightz);
	updateGL();
}

void MyGLWidget::setlightsource() {
	currentLightPos=new glm::vec3(lightx,lighty,lightz);
	//glUniform3f(u_lightPosLocation, 10, 10, 10);
	//cout << "wHAT NOW?";
	//glUniform3f(u_lightPosLocation, currentLightPos->x, currentLightPos->y, currentLightPos->z);
	updateGL();
}

void MyGLWidget::snapViewSelected() {

	if(selectionmode==0){
		if(currentface==NULL) return;
		glm::vec3 centerofface = currentface->getCentroid();
		mycam->changereferencepoint(centerofface[0],centerofface[1],centerofface[2]);
		updateGL();
	
	}

	else if(selectionmode==1){
		if(currentedge==NULL) return;
		Vertex* first = currentedge->getVert();
		Vertex* second = (currentedge->getSym())->getVert();
		float newx= (first->getX()+second->getX())/2;
		float newy= (first->getY()+second->getY())/2;
		float newz= (first->getZ()+second->getZ())/2;
		resetedgeline();
		mycam->changereferencepoint(newx,newy,newz);
		updateGL();

	}

	else if(selectionmode==2){
		if(currentvertex==NULL) return;
		mycam->changereferencepoint(currentvertex->getX(),currentvertex->getY(),currentvertex->getZ());
		updateGL();
	}
	updateGL();
}



///MANSHAS SLOTS

void MyGLWidget::loadObjFile() {
	Mesh* m = new Mesh();
	string loadName = QFileDialog::getOpenFileName(NULL, tr("Load .OBJ Mesh"), "", tr("Object file (*.obj)")).toStdString();
	cube->filename = new string(loadName);
	cube = m->objFileStructure3(loadName.c_str());
	cube->generateVertArray();
	cube->generateNormArray();
	cube->generateColorArray();
	cube->generateIndArray();
	updateGL();
	//
}

void MyGLWidget::saveObjFile() {
	string savename = QFileDialog::getSaveFileName(NULL, tr("Save .OBJ Mesh"), "", tr("Object file (*.obj)")).toStdString();
	cube->filename = new string(savename);
	cube->saveObjFile(savename.c_str());
}

void MyGLWidget::subdivide() {

}

void MyGLWidget::subdivideMesh() {
	if(currentface!=NULL) currentface->deselect();
	resetedgeline();
	cube->subdivideMesh();
	updateGL();
}


	//MANSHA
void MyGLWidget::setdeformi(int i){
	boundingBox_i = i;
	boundingBox = cube->createBoundingBox(boundingBox_i,boundingBox_j,boundingBox_k);
	updateGL();
}
void MyGLWidget::setdeformj(int j){
	boundingBox_j = j;
	boundingBox = cube->createBoundingBox(boundingBox_i,boundingBox_j,boundingBox_k);
	updateGL();
}
void MyGLWidget::setdeformk(int k){
	boundingBox_k = k;
	boundingBox = cube->createBoundingBox(boundingBox_i,boundingBox_j,boundingBox_k);
	updateGL();
}
void MyGLWidget::setdeformdimensions(){
	boundingBox = cube->createBoundingBox(boundingBox_i,boundingBox_j,boundingBox_k);
	updateGL();
	
}
void MyGLWidget::twist(){

}

void MyGLWidget::saveObjOver() {
	if (cube->filename == NULL) {
		saveObjFile();	
	}
	string savename = *cube->filename;
	cube->saveObjFile(savename.c_str());
}

void MyGLWidget::resetedgeline(){
	edge1x=0;
	edge1y=0; 
	edge1z=0;
	edge2x=0;
	edge2y=0; 
	edge2z=0;
}

void MyGLWidget::selectcontrolx(int i){
	controlx=i;
	setcurrentcontrolvertex();
	// cout << "xxxxxxx";
	updateGL();
}
void MyGLWidget::selectcontroly(int i){
	controly=i;
	setcurrentcontrolvertex();
	 //cout << "yyyyyy";
	updateGL();
}
void MyGLWidget::selectcontrolz(int i){
	controlz=i;
	setcurrentcontrolvertex();
	// cout << "zzzzzzzz";
	updateGL();
}

void MyGLWidget::setcurrentcontrolvertex(){
  if(controlx>(boundingBox_i))controlx=boundingBox_i;
 if(controly>(boundingBox_j))controly=boundingBox_j;
 if(controlz>(boundingBox_k))controlz=boundingBox_k;

 int x = ((boundingBox_k+1)*(boundingBox_j+1))*(controlx);
 int y = (boundingBox_k+1)*(controly);
 int z = (controlz)%(boundingBox_k+1);
 int sum=x+y+z;

    currentcontrolvert=boundingBox.at(x+y+z);
}


void MyGLWidget::twistX(int x) {

		float deg = (float)x/10.0f;
	for(int i = 0; i<boundingBox.size(); i++){
		float z = boundingBox.at(i)->xyz->z;
		float y1 = boundingBox.at(i)->xyz->y;
		float x = boundingBox.at(i)->xyz->x*cos(y1*(float)deg) - boundingBox.at(i)->xyz->y*sin(float(y1*(float)deg));
		float y =  boundingBox.at(i)->xyz->x*sin(y1*(float)deg) + boundingBox.at(i)->xyz->x*cos(y1*(float)deg);
		
		boundingBox.at(i)->xyz->x = x;
		boundingBox.at(i)->xyz->y = y;
	}

		FFD();
}



void MyGLWidget::twistY(int y) {
		float deg = (float)y/10.0f;

	
	for(int i = 0; i<boundingBox.size(); i++){
		float z = boundingBox.at(i)->xyz->z;
		float x1 = boundingBox.at(i)->xyz->x;
		float x = boundingBox.at(i)->xyz->x*cos(x1*(float)deg) - boundingBox.at(i)->xyz->y*sin(float(x1*(float)deg));
		float y =  boundingBox.at(i)->xyz->x*sin(x1*(float)deg) + boundingBox.at(i)->xyz->x*cos(x1*(float)deg);
		
		boundingBox.at(i)->xyz->x = x;
		boundingBox.at(i)->xyz->y = y;
	}


		FFD();
}

void MyGLWidget::twistZ(int z) {
	float deg = (float)z/10.0f;
	for(int i = 0; i<boundingBox.size(); i++){
		float z = boundingBox.at(i)->xyz->z;
		float x = boundingBox.at(i)->xyz->x*cos(z*(float)deg) - boundingBox.at(i)->xyz->y*sin(float(z*(float)deg));
		float y =  boundingBox.at(i)->xyz->x*sin(z*(float)deg) + boundingBox.at(i)->xyz->x*cos(z*(float)deg);
		boundingBox.at(i)->xyz->x = x;
		boundingBox.at(i)->xyz->y = y;
	}

		FFD();
}

int MyGLWidget::getCombination(int n, int r){
	if(r==0){
		return 1;
	}
	int nfactorial = 1;
	for(int i = 1; i<=n; i++){
		nfactorial = nfactorial*i;
	}

	int rfactorial = 1;
	for(int i = 1; i<=r; i++){
		rfactorial = rfactorial*i;
	}
	int value = n-r;
	int n_rfactorial = 1;
	for(int i = 1; i<=value; i++){
		n_rfactorial = n_rfactorial*i;
	}

	int result = n_rfactorial*rfactorial;


	result = nfactorial/result;

	return result;

}


Vertex* MyGLWidget::getPointFromIJK(int i, int j, int k){

	int x = ((boundingBox_k+1)*(boundingBox_j+1))*(i);
	int y = (boundingBox_k+1)*(j);
	int z = (k)%(boundingBox_k+1);
	
	return boundingBox.at(x+y+z);

}

void MyGLWidget::FFD(){
	
	int l = boundingBox_i;
	int m = boundingBox_j;
	int n = boundingBox_k;

	//vector<Vertex*>* meshBeforeDeform = cube->verts;
	
	float kvx = 0;
	float jvx = 0;
	float ivx = 0;
	float kvy = 0;
	float jvy = 0;
	float ivy = 0;
	float kvz = 0;
	float jvz = 0;
	float ivz = 0;
	int i = 0;
	int j = 0;
	int k = 0;
	for(int v = 0; v<cube->verts->size(); v++){
		float s = (cube->verts->at(v)->xyz->x - cube->minXValue)/(cube->maxXValue-cube->minXValue);
		float t = (cube->verts->at(v)->xyz->y - cube->minYValue)/(cube->maxYValue-cube->minYValue);
		float u = (cube->verts->at(v)->xyz->z - cube->minZValue)/(cube->maxZValue-cube->minZValue);
		

		for( i = 0; i<l; i++){
			for( j = 0; j<m; j++){
				for( k = 0; k<n; k++){
					float kfactorialterm = (float)getCombination(n,k);
					float kpower1 = pow((1.0f-u), (float)(n-k));
					float kpower2 = pow(u,(float)k);
					kvx+= kfactorialterm*kpower1*kpower2*(float)getPointFromIJK(i, j, k)->xyz->x;
					kvy+= kfactorialterm*kpower1*kpower2*(float)getPointFromIJK(i, j, k)->xyz->y;
					kvz+= kfactorialterm*kpower1*kpower2*(float)getPointFromIJK(i, j, k)->xyz->z;

				}
				float jfactorialterm = (float)getCombination(m,j);
				float jpower1 = pow((1.0f-t), (float)(m-j));
				float jpower2 = pow(t,(float)j);
				jvx+= jfactorialterm*jpower1*jpower2*(float)getPointFromIJK(i, j, k)->xyz->x*kvx;
				jvy+= jfactorialterm*jpower1*jpower2*(float)getPointFromIJK(i, j, k)->xyz->y*kvy;
				jvz+= jfactorialterm*jpower1*jpower2*(float)getPointFromIJK(i, j, k)->xyz->z*kvz;
				kvx=0;
				kvy=0;
				kvz=0;
			}
			float ifactorialterm = (float)getCombination(l,i);
			float ipower1 = pow((1.0f-s), (float)(l-i));
			float ipower2 = pow(s,(float)i);
			ivx+= ifactorialterm*ipower1*ipower2*(float)getPointFromIJK(i, j, k)->xyz->x*jvx;
			ivy+= ifactorialterm*ipower1*ipower2*(float)getPointFromIJK(i, j, k)->xyz->y*jvy;
			ivz+= ifactorialterm*ipower1*ipower2*(float)getPointFromIJK(i, j, k)->xyz->z*jvz;
					jvx =0 ;
					jvy =0 ;
					jvz =0 ;		

		}

		
		float xs = cube->verts->at(v)->xyz->x;
		float ys = cube->verts->at(v)->xyz->y;
		float zs = cube->verts->at(v)->xyz->z;
		cube->verts->at(v)->xyz = new glm::vec3(ivx, ivy, ivz);
		
		
		ivx=0;
		ivy = 0;
		ivz = 0;

	}

	for(int a =0; a<cube->verts->size(); a++){
		/*cout<<"vertex "<<a<<endl;
		cout<<"x: "<<cube->verts->at(a)->xyz->x<<endl;
		cout<<"y: "<<cube->verts->at(a)->xyz->y<<endl;
		cout<<"z: "<<cube->verts->at(a)->xyz->z<<endl;*/
	}
		
		cube->generateVertArray();
		cube->generateNormArray();
		cube->generateColorArray();
		cube->generateIndArray();
	updateGL();
	

}