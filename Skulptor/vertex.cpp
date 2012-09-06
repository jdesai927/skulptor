#include "vertex.h"

Vertex::Vertex(float X, float Y, float Z, QString* ID) : QListWidgetItem(*ID) {
	xyz = new glm::vec3(X, Y, Z);
	id = new int(ID->toInt());
	//adjEdges = new vector<HalfEdge*>();
}

Vertex::~Vertex() {
	if (xyz != NULL) {
		delete xyz;
	}
	if (id != NULL) {
		delete id;
	}
}

glm::vec3 Vertex::averageVertex(Vertex* v1, Vertex* v2) {
	float x = (v1->xyz->x + v2->xyz->x)/2;
	float y = (v1->xyz->y + v2->xyz->y)/2;
	float z = (v1->xyz->z + v2->xyz->z)/2;
	return glm::vec3(x, y, z);
}

float Vertex::getX() {
	return xyz->x;
}

float Vertex::getY() {
	return xyz->y;
}

float Vertex::getZ() {
	return xyz->z;
}

int Vertex::getID() {
	return *id;
}

glm::vec3* Vertex::getVec3() {
	return xyz;
}

glm::vec4 Vertex::getVec4() {
	return glm::vec4(xyz->x, xyz->y, xyz->z, 1.0f);
}