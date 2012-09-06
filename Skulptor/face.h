#include "../glm/glm.hpp"
#include "vertex.h"
#include <vector>
#include <map>
#include <qlistwidget.h>

#pragma once
#include "halfedge.h"

class HalfEdge;

class Face : public QListWidgetItem {

private:
	

public:
	glm::vec3 centroid;
	int* id;
	bool isselected;
	Face();
	Face(HalfEdge* half, float r, float g, float b, QString* ID);
	~Face();
	HalfEdge* first;
	glm::vec3* color;
	glm::vec3* oldcolor;
	HalfEdge* getFirst();
	glm::vec3* getColor();
	bool isPlanar();
	bool isConvex();
	glm::vec3 getCentroid();
	float intersectsWith(glm::vec3 point, glm::vec3 ray);
	int numEdges();
	int getID();
	bool containsVert(Vertex* v);

	//RITVIKS
	void select();
	void deselect();

};