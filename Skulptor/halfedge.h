#pragma once
#include "face.h"

class Face;

#include "vertex.h"

class HalfEdge {

private:

public:
	Face* f;
	Vertex* v;
	HalfEdge* next;
	HalfEdge* sym;
	HalfEdge();
	HalfEdge(Face* face, Vertex* vert, HalfEdge* nex, HalfEdge* sy);
	~HalfEdge();
	Face* getFace();
	Vertex* getVert();
	glm::vec3 getNormAtVert();
	HalfEdge* getNext();
	HalfEdge* getSym();
	HalfEdge* getPrev();
};