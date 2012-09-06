#include "MyCamera.h"
#include <iostream>
using namespace std; 
#define PI 3.14159265


MyCamera::MyCamera(int sw,int sh)
{	
	 eyex=20;
	 eyey=0;
	 eyez=0;
	 centerx=0;
	 centery=0;
	 centerz=0;
	 upx=0;
	 upy=1;
	 upz=0;
	 //radius=20;

	 customeyex=eyex;
	 customeyey=eyey;
	 customeyez=eyez;
	 customcenterx=centerx;
	 customcentery=centery;
	 customcenterz=centerz;
	 customupx=upx;
	 customupy=upy;
	 customupz=upz;
	 customradius=radius;

	eye= new glm::vec3(eyex,eyey,eyez);
	center = new glm::vec3(centerx,centery,centerz);
	up = new glm::vec3(upx,upy,upz);
	truecenter = new glm::vec3(0,0,0);
	
	hangle=0;
	vangle=0;
	
	screenwidth=sw;
	screenheight=sh;
	radius=20;
	 customradius=radius;

	ProjectionMatrix= glm::mat4(1.0f);
	//glm::mat4 ModelViewProjectionMatrix= glm::mat4(1.0f);
	ProjectionMatrix*= glm::perspective(45.0f, 1.0f, 0.1f, 200.0f);
	ViewMatrix = glm::lookAt(*eye,*center,*up);

}


MyCamera::~MyCamera(void)
{



}


glm::mat4 MyCamera::getViewMatrix(){
	//update
	eye= new glm::vec3(eyex,eyey,eyez);
	center = new glm::vec3(centerx,centery,centerz);
	up = new glm::vec3(upx,upy,upz);
	ViewMatrix = glm::lookAt(*eye,*truecenter,*up);
	return ViewMatrix;
}

glm::mat4 MyCamera::getProjectionMatrix(){
	//might change depending on radius!
	return ProjectionMatrix;
}

glm::mat4 MyCamera::getFinalMatrix(){
	glm::mat4 temp = getProjectionMatrix()*getViewMatrix();
	return temp;
}

glm::mat4 MyCamera::setradius(int x){
	if(x>0){
	radius = x;
	glm::vec3 neweye=glm::core::function::geometric::normalize(glm::vec3(eyex,eyey,eyez));
	eyex=neweye[0]*x;
	eyey=neweye[1]*x;
	eyez=neweye[2]*x;	
	}
	return getFinalMatrix();
	
}



glm::mat4 MyCamera::rotateright(int x, int y){
	
	float xdif = (hangle-x);
	//cout << xdif << endl;
	hangle=x;
	/*eyex=(radius*sin(x*PI/180)*cos(y*PI/180));
	eyey=(radius*cos(x*PI/180)*cos(y*PI/180));
	eyez= radius*sin(y*PI/180);*/


	glm::vec3 oldup(upx,upy,upz);
	glm::vec3 oldeye(eyex,eyey,eyez);
	glm::vec3 viewvec(eyex-centerx,eyey-centery,eyez-centerz);
	glm::vec3 axis = glm::cross(oldup,viewvec);

	//setting new up
	//glm::vec3 axis = glm::cross(oldup,viewvec);	
	//glm::vec3 newup = glm::gtx::rotate_vector::rotate(oldup,ydif,axis);

	//upx=newup[0];
	//upy=newup[1];
	//upz=newup[2];

	//setting eye
	glm::vec3 neweye = glm::gtx::rotate_vector::rotate(viewvec,xdif,oldup);
	neweye=glm::core::function::geometric::normalize(neweye)*radius;
	
	eyex=neweye[0];
	eyey=neweye[1];
	eyez=neweye[2];


	return getFinalMatrix();
}



glm::mat4 MyCamera::rotateup(int x, int y){
	//storing changed v angle
	float ydif = (vangle-y);
	//cout << ydif << endl;
	vangle=y;

	glm::vec3 oldup(upx,upy,upz);
	glm::vec3 oldeye(eyex,eyey,eyez);
	glm::vec3 viewvec(eyex-centerx,eyey-centery,eyez-centerz);

	glm::vec3 axis = glm::cross(oldup,viewvec);	
	glm::vec3 newup = glm::gtx::rotate_vector::rotate(oldup,ydif,axis);
	newup = glm::core::function::geometric::normalize(newup);
	
	upx=newup[0];
	upy=newup[1];
	upz=newup[2];

	//
	////cout << upx << endl;
	////cout << upy << endl;
	////cout << upz << endl;

	//eyex=(radius*sin(x*PI/180)*cos(y*PI/180));
	//eyey=(radius*cos(x*PI/180)*cos(y*PI/180));
	//eyez= radius*sin(y*PI/180);


	/*eyex=(radius*sin(x*PI/180)*cos(y*PI/180));
	eyey=(radius*cos(x*PI/180)*cos(y*PI/180));
	eyez= radius*sin(y*PI/180);*/

	glm::vec3 neweye = glm::gtx::rotate_vector::rotate(viewvec,ydif,axis);
	neweye=glm::core::function::geometric::normalize(neweye)*radius;

	eyex=neweye[0];
	eyey=neweye[1];
	eyez=neweye[2];
	//cout << eyex;
	//cout << eyey;
	//cout << eyez << endl;

	return getFinalMatrix();
}


void MyCamera::changereferencepoint(int x, int y, int z){
	truecenter = new glm::vec3(x,y,z);
}


//VIEW METHODS

void MyCamera::overheadview(){
	 eyex=0;
	 eyey=radius;
	 eyez=0;
	 centerx=0;
	 centery=0;
	 centerz=0;
	 upx=0;
	 upy=0;
	 upz=1;
	 //setradius(20);
	 
}

void MyCamera::frontview(){
	 eyex=0;
	 eyey=0;
	 eyez=radius;
	 centerx=0;
	 centery=0;
	 centerz=0;
	 upx=0;
	 upy=1;
	 upz=0;
	 //radius = 20;

}

void MyCamera::rightview(){
	
	 eyex=radius;
	 eyey=0;
	 eyez=0;
	 centerx=0;
	 centery=0;
	 centerz=0;
	 upx=0;
	 upy=1;
	 upz=0;
	// radius = 20;
}

void MyCamera::leftview(){
	
	eyex=-radius;
	 eyey=0;
	 eyez=0;
	 centerx=0;
	 centery=0;
	 centerz=0;
	 upx=0;
	 upy=1;
	 upz=0;
	// radius = 20;
}

void MyCamera::backview(){
	
	 eyex=0;
	 eyey=0;
	 eyez=-radius;
	 centerx=0;
	 centery=0;
	 centerz=0;
	 upx=0;
	 upy=1;
	 upz=0;
	// radius = 20;
}

	
void MyCamera::perspectiveview(){
	
	 eyex=radius;
	 eyey=-radius;
	 eyez=0;
	 centerx=0;
	 centery=0;
	 centerz=0;
	 upx=0;
	 upy=1;
	 upz=0;
	 //radius = 20;
}

void MyCamera::customview(){
	 eyex=customeyex;
	eyey= customeyey;
	 eyez=customeyez;
	centerx= customcenterx;
	 centery=customcentery;
	centerz= customcenterz;
	 upx=customupx;
	 upy=customupy;
	 upz=customupz;
	 radius = customradius;
}

void MyCamera::savecustomview(){
	 customeyex=eyex;
	 customeyey=eyey;
	 customeyez=eyez;
	 customcenterx=centerx;
	 customcentery=centery;
	 customcenterz=centerz;
	 customupx=upx;
	 customupy=upy;
	 customupz=upz;
	customradius  =radius ;
}
