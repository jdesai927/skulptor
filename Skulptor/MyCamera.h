#pragma once

	
#include "../glm/gtx/rotate_vector.hpp"
#include  <math.h>
//#include "../glm/core/type_mat4.hpp"
//#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
class MyCamera
{

private: 




	glm::vec3* eye;
	glm::vec3* center;
	glm::vec3* up;

	int screenwidth;
	int screenheight;
	//using above get view matrix
	float radius;
	float znear; //distance to front and back plane
	float zfar;

	//angles

	int hangle;
	int vangle;

	glm::mat4 ProjectionMatrix;
	glm::mat4 ModelViewProjectionMatrix;
	glm::mat4 ViewMatrix;

public:
		glm::vec3* truecenter;
	float eyex;
	float eyey;
	float eyez;
	float centerx;
	float centery;
	float centerz;
	float upx;
	float upy;
	float upz;

	float customeyex;
	float customeyey;
	float customeyez;
	float customcenterx;
	float customcentery;
	float customcenterz;
	float customupx;
	float customupy;
	float customupz;
	float customradius;

	void changereferencepoint(int,int,int);
	void savecustomview();

	MyCamera(int sw,int sh);
	~MyCamera(void);

	
	
glm::mat4 getViewMatrix(void);
glm::mat4 getProjectionMatrix(void);
glm::mat4 getFinalMatrix(void);


//TRANSFORMATION METHODS

glm::mat4 rotateright(int x,int y);
glm::mat4 rotateup(int x, int y);
glm::mat4 setradius(int x);

//views
void overheadview();
void perspectiveview();
void frontview();
void rightview();
void leftview();
void backview();
void customview();	
	
};

