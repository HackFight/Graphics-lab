#ifndef LIGHTS_STRUCT
#define LIGHTS_STRUCT

struct DirLight
{
	bool enabled = true;
	glm::vec3 direction = glm::vec3(0.0f, -1.0f, 1.0f);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
};

struct PointLight
{
	bool enabled = true;
	glm::vec3 position = glm::vec3(0.0f, 5.0f, 0.0f);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
};

#endif