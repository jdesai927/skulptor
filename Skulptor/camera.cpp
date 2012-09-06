#include "camera.h"

Camera::Camera() {
	position = new glm::vec4(0.0f, 0.0f, 5.0f, 0.0f);
	up = new glm::vec4(0.0f, 5.0f, 0.0f, 0.0f);
}

void Camera::rotate(float angle, bool direction) {
	//true is x, false is y
	glm::vec3 axis = glm::vec3(up->x, up->y, up->z);
	if (!direction) {
		glm::vec3 pos = glm::vec3(position->x, position->y, position->z);
		axis = glm::cross(pos, axis);
	}
	glm::mat4 rotator = glm::rotate(glm::mat4(), angle, axis);
	*position = rotator * *position;
	*up = rotator * *up;
}

glm::vec3 Camera::getPos3() {
	return glm::vec3(position->x, position->y, position->z);
}

glm::vec3 Camera::getUp3() {
	return glm::vec3(position->x, position->y, position->z);
}

glm::vec4* Camera::getPos4() {
	return position;
}

glm::vec4* Camera::getUp4() {
	return up;
}