#include "camera.h"

Camera::Camera() :
	position(0.0f, 0.0f, 5.0f), direction(0.0f, 0.0f, -1.0f), up(0.0f, 1.0f, 0.0f), sensitivity(0.1f), oldMousePos(0.0f) { }

void Camera::UpdateMouse(const glm::vec2& newMousePos)
{
	glm::vec2 mouseDelta = newMousePos - oldMousePos;

	if (glm::length(mouseDelta) < 100.0f) //If jump is too big --> give up (mouse left window and came back for example)
	{
		direction = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(-mouseDelta.x) * sensitivity, up)) * direction;

		glm::vec3 toRotateAround = glm::cross(direction, up);
		direction = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(-mouseDelta.y) * sensitivity, toRotateAround)) * direction;
	}

	oldMousePos = newMousePos;
}

glm::mat4 Camera::GetWorldToViewMatrix() const
{
	return glm::lookAt(position, position + direction, up);
}
