#include "face.h"
#include <iostream>

Face::Face() {
  
}

Face::Face(HalfEdge* half, float r, float g, float b, QString* ID) : QListWidgetItem(*ID) {
	first = half;
	color = new glm::vec3(r, g, b);
	oldcolor = new glm::vec3(r, g, b);
	id = new int(ID->toInt());
	isselected=false;
	centroid = glm::vec3(0.0f);
}

Face::~Face() {
	if (color != NULL) {
		delete color;
	}
	if (id != NULL) {
		delete id;
	}
	if (first != NULL) {
		/*
		HalfEdge* he = first->getPrev();
		while (he != first) {
			if (he != NULL && he->sym != NULL) {
				//he->sym->sym = NULL;
			}
			HalfEdge* del = he;
			if (del != NULL) {
				he = he->getPrev();
				delete del;
			} else {
				break;
			}
		}
		*/
	}
}

int Face::numEdges() {
	int count = 0;
	HalfEdge* countHE = first;
	while (true) {
		count++;
		countHE = countHE->next;
		if (countHE == first ) {
			return count;
		}
	}
}

HalfEdge* Face::getFirst() {
	return first;
}

glm::vec3* Face::getColor() {
	glm::vec3* red=new glm::vec3(255,255,255);
	if (isselected){
		
		return red;
	}
		return red;
}

int Face::getID() {
	return *id;
}

bool Face::containsVert(Vertex* v) {
	HalfEdge* he = first;
	if (he == NULL) {
		return false;
	}
	while (true) {
		if (he->v == v) {
			return true;
		}
		he = he->next;
		if (he == first || he == NULL) {
			break;
		}
	}
	return false;
}

bool Face::isPlanar() {
	float e = 0.01;
	glm::vec3 norm;
	float thisNorm;
	HalfEdge* he = first;
	while (true) {
		norm = he->getNormAtVert();
		thisNorm = glm::dot(*he->next->v->xyz, norm);
		if (thisNorm < -e || thisNorm > e) {
			return false;
		}
		he = he->next;
		if (he == first) {
			break;
		}
	}
	return true;
}

bool Face::isConvex() {
	return true;
}

float getTriArea(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3) {
	float side1 = glm::length(v1 - v2);
	float side2 = glm::length(v1 - v3);
	float side3 = glm::length(v2 - v3);
	float perim = (side1 + side2 + side3)/2;
	return sqrt(perim * (perim - side1) * (perim - side2) * (perim - side3));
}

float Face::intersectsWith(glm::vec3 point, glm::vec3 ray) {
	glm::vec3 v1 = *first->v->xyz;
	glm::vec3 v2 = *first->next->v->xyz;
	glm::vec3 v3 = *first->next->next->v->xyz;
	glm::vec3 side1 = (v1 - v2);
	glm::vec3 side2 = (v1 - v3);
	glm::vec3 n = glm::cross(side1, side2);
	float t = glm::dot(n, (v1 - point)) / glm::dot(n, (ray - point));
	glm::vec3 intPoint = point + (ray - point) * t;
	float s = getTriArea(v1, v2, v3);
	float s1 = getTriArea(intPoint, v2, v3)/s;
	float s2 = getTriArea(intPoint, v3, v1)/s;
	float s3 = getTriArea(intPoint, v1, v2)/s;
	if (s1 >= 0 && s1 <= 1 && 
		s2 >= 0 && s2 <= 1 && 
		s3 >= 0 && s3 <= 1 && 
		(s1 + s2 + s3 <= 1.01) && 
		(s1 + s2 + s3 >= 0.99)) {
			return t;
	} else {
		return -1;
	}
}

void Face::select(){
	oldcolor=color;
	color = new glm::vec3(255,0,0);
	isselected=true;
}

void Face::deselect(){
	color=oldcolor;
	isselected=false;
}

glm::vec3 Face::getCentroid() {
	float i = 0.0f;
	glm::vec3 sum = glm::vec3(0.0f);
	HalfEdge* he = first;
	while (true) {
		sum += (*he->v->xyz);
		i++;
		he = he->next;
		if (he == first) {
			break;
		}
	}
	sum /= i;
	return sum;
}