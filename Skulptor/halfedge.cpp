#include "halfedge.h"
#include <iostream>

HalfEdge::HalfEdge(Face* face, Vertex* vert, HalfEdge* nex, HalfEdge* sy) {
	f = face;
	v = vert;
	next = nex;
	sym = sy;
}

HalfEdge::HalfEdge() {

}

HalfEdge::~HalfEdge() {
	
}

Face* HalfEdge::getFace() {
	return f;
}

Vertex* HalfEdge::getVert() {
	return v;
}

HalfEdge* HalfEdge::getNext() {
	return next;
}

HalfEdge* HalfEdge::getSym() {
	return sym;
}

HalfEdge* HalfEdge::getPrev() {
	HalfEdge* counter = this;
	while (true) {
		if (counter == NULL) {
			break;
		}
		if (counter->next == this) {
			break;
		}
		counter = counter->next;
	}
	return counter;
}

glm::vec3 HalfEdge::getNormAtVert() {
	glm::vec3 v1 = *v->xyz - *getPrev()->v->xyz;
	glm::vec3 v2 = *v->xyz - *next->v->xyz;
	return glm::cross(v1, v2);
}