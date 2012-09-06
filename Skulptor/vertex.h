#ifndef _VERTEX_
#define _VERTEX_

#include "../glm/glm.hpp"
#include <qlistwidget.h>
#include <vector>

//#pragma once
//#include "face.h"
//#include "halfedge.h"

//class Face;
//class HalfEdge;

class Vertex : public QListWidgetItem {

private:
	

public:
	int* id;
	//vector<HalfEdge*>* adjEdges;
	//vector<Face*>* adjFaces;
	Vertex(float X, float Y, float Z, QString* ID);
	~Vertex();
	glm::vec3* xyz;
	static glm::vec3 averageVertex(Vertex* v1, Vertex* v2);
	float getX();
	float getY();
	float getZ();
	glm::vec3* getVec3();
	glm::vec4 getVec4();
	int getID();

};

#endif