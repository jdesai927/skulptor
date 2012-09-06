#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"

class Camera {
private:
	
public:
	glm::vec4* position;
	glm::vec4* up;
	Camera();
	glm::vec3 getPos3();
	glm::vec3 getUp3();
	glm::vec4* getPos4();
	glm::vec4* getUp4();
	void rotate(float angle, bool direction);

};