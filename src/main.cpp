#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>
#include <GLFW/glfw3.h>
#include <openglErrorReporting.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <primitives/vbo.h>
#include <primitives/ebo.h>
#include <primitives/vao.h>
#include <primitives/shapeGenerator.h>
#include <primitives/shader.h>
#include <camera.h>

//##### - GENERAL VARIABLES - #####//
GLuint programID;
Camera camera;
const float cameraSpeed = 2.5f; // adjust accordingly

glm::vec3 objectColor = glm::vec3(1.0f, 0.5f, 0.31f);
glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 lightPos = glm::vec3(1.2f, 1.0f, 2.0f);

glm::mat4 clip = glm::mat4(1.0f);
glm::mat4 proj = glm::mat4(1.0f);
glm::mat4 view = glm::mat4(1.0f);
glm::mat4 modelToClip = glm::mat4(1.0f);
glm::mat4 model1 = glm::mat4(1.0f);
glm::mat4 model2 = glm::mat4(1.0f);

int winWidth = 640, winHeight = 480;

float deltaTime = 0.0f;
float lastFrame = 0.0f;



//##### - FUNCTIONS - #####//
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	camera.UpdateMouse(glm::vec2(xpos, ypos));
}

static void error_callback(int error, const char* description)
{
	std::cout << "[OpenGL Error]: " << description << "\n";
}

void processInput(GLFWwindow* window)
{
	float speed = cameraSpeed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.position += speed * camera.direction;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.position -= speed * camera.direction;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.position -= glm::normalize(glm::cross(camera.direction, camera.up)) * speed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.position += glm::normalize(glm::cross(camera.direction, camera.up)) * speed;
}



//##### - ENTRY POINT - #####//
int main(void)
{
#pragma region INIT
	//##### - INIT - #####//
	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
		exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#endif

	GLFWwindow* window = glfwCreateWindow(winWidth, winHeight, "Lighting", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	enableReportGlErrors();

	//glfwSwapInterval(1); //vsync
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe
	glEnable(GL_DEPTH_TEST);
#pragma endregion

	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetCursorPosCallback(window, mouse_callback);


		//##### - DATA - #####//
		ShapeData cube = ShapeGenerator::MakeCube();

		VAO vao;
		VBO vbo(cube.GetVertexBufferSize(), cube.vertices);
		VBOLayout vboL;
		vboL.Push<GLfloat>(3);	//Set position attrib
		vboL.Push<GLfloat>(3);	//Set normals attrib
		vboL.Push<GLfloat>(3);	//Set color attrib

		vao.AddBuffer(vbo, vboL, 0);

		EBO ebo(cube.GetIndexBufferSize(), cube.indices);



		//##### - SHADERS - #####//
		Shader light(RESOURCES_PATH "shaders/default.vert", RESOURCES_PATH "shaders/color.frag");
		Shader reciever(RESOURCES_PATH "shaders/default.vert", RESOURCES_PATH "shaders/phong.frag");



		//##### - CLEANING - #####//
		cube.CleanUp();

		vao.Unbind();
		vbo.Unbind();
		ebo.Unbind();

		light.Unbind();
		reciever.Unbind();



		//##### - LOCAL CONSTANTS - #####//
		camera.position = glm::vec3(0.0f, 0.0f, 5.0f);

		model1 = glm::mat4(1.0f);

		model2 = glm::translate(model2, lightPos);
		model2 = glm::scale(model2, glm::vec3(0.1f, 0.1f, 0.1f));

		//##### - CONSTANT UNIFORMS - #####//
		reciever.Bind();
		reciever.SetUniform3fv("lightPos", lightPos);
		reciever.SetUniform3fv("material.ambient", objectColor);
		reciever.SetUniform3fv("material.diffuse", objectColor);
		reciever.SetUniform3f("material.specular", 0.5f, 0.5f, 0.5f);
		reciever.SetUniform1f("material.shininess", 32.0f);
		reciever.SetUniform3fv("light.ambient", lightColor * 0.2f);
		reciever.SetUniform3fv("light.diffuse", lightColor * 0.5f);
		reciever.SetUniform3fv("light.specular", lightColor * 1.0f);

		light.Bind();
		light.SetUniform3fv("color", lightColor);



		// ### - Main loop - ### //
		while (!glfwWindowShouldClose(window))
		{
			//Loop start
			glfwGetFramebufferSize(window, &winWidth, &winHeight);
			glViewport(0, 0, winWidth, winHeight);
			glClear(GL_COLOR_BUFFER_BIT);
			glClear(GL_DEPTH_BUFFER_BIT);

			//##### - Inputs
			float currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;
			processInput(window);

			//##### - Matrices
			proj = glm::perspective(glm::radians(70.0f), (float)winWidth / (float)winHeight, 0.1f, 100.0f);
			view = camera.GetWorldToViewMatrix();

			//##### - Uniforms
			reciever.Bind();
			reciever.SetUniformMatrix4fv("proj", proj);
			reciever.SetUniformMatrix4fv("view", view);
			reciever.SetUniformMatrix4fv("model", model1);
			reciever.SetUniform3fv("viewPos", camera.position);

			light.Bind();
			light.SetUniformMatrix4fv("proj", proj);
			light.SetUniformMatrix4fv("view", view);
			light.SetUniformMatrix4fv("model", model2);
			light.SetUniform3fv("color", lightColor);

			//##### - Rendering
			vao.Bind();

			reciever.Bind();
			glDrawElements(GL_TRIANGLES, ebo.GetIndicesCount(), GL_UNSIGNED_SHORT, 0);

			light.Bind();
			glDrawElements(GL_TRIANGLES, ebo.GetIndicesCount(), GL_UNSIGNED_SHORT, 0);

			//##### - Debug


			//Loop end
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}