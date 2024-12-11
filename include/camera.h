#ifndef CAMERA_CLASS
#define CAMERA_CLASS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
	glm::vec2 oldMousePos;

public:
	glm::vec3 position;
	glm::vec3 direction;
	const glm::vec3 up;

	float sensitivity;

	Camera();
	void UpdateMouse(const glm::vec2& newNousePos);
	glm::mat4 GetWorldToViewMatrix() const;
};

#endif