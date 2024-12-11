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
#include <shapeGenerator.h>
#include <camera.h>
#include <shader.h>

//##### - GENERAL VARIABLES - #####//
GLuint programID;
Camera camera;
const float cameraSpeed = 2.5f; // adjust accordingly

glm::mat4 clip = glm::mat4(1.0f);
glm::mat4 proj = glm::mat4(1.0f);
glm::mat4 view = glm::mat4(1.0f);
glm::mat4 model = glm::mat4(1.0f);

int winWidth = 0, winHeight = 0;

float deltaTime = 0.0f;
float lastFrame = 0.0f;



//##### - FUNCTIONS - #####//
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	camera.UpdateMouse(glm::vec2(xpos, ypos));
}

static void error_callback(int error, const char* description)
{
	std::cout << "[OpenGL Error]:" << description << "\n";
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

	GLFWwindow* window = glfwCreateWindow(640, 480, "Instancing", NULL, NULL);
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
		//##### - DATA - #####//
		ShapeData cube = ShapeGenerator::MakeCube();

		VAO vao;
		VBO vbo(cube.GetVertexBufferSize(), cube.vertices);
		VBOLayout vboLpc;
		vboLpc.Push<GLfloat>(3);	//Set position attrib
		vboLpc.Push<GLfloat>(3);	//Set color attrib

		vao.AddBuffer(vbo, vboLpc, 0);

		EBO ebo(cube.GetIndexBufferSize(), cube.indices);

		glm::mat4 modelMatrices[] =
		{
			glm::translate(glm::mat4(1.0f), glm::vec3(3.0f, 0.0f, 0.0f)),
			glm::translate(glm::mat4(1.0f), glm::vec3(-3.0f, 0.0f, 0.0f))
		};

		VBO modelMatrixBuffer(sizeof(modelMatrices), modelMatrices);
		VBOLayout vboLm;
		vboLm.Push<GLfloat>(4);	//Set first vec4 of model matrix attrib
		vboLm.Push<GLfloat>(4);	//Set second vec4 of model matrix attrib
		vboLm.Push<GLfloat>(4);	//Set third vec4 of model matrix attrib
		vboLm.Push<GLfloat>(4);	//Set fourth vec4 of model matrix attrib
		glVertexAttribDivisor(2, 1);
		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);

		vao.AddBuffer(modelMatrixBuffer, vboLm, 2);



		//##### - SHADERS - #####//
		Shader shader(RESOURCES_PATH "shaders/instanced-geometry.vert", RESOURCES_PATH "shaders/default.frag");

		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetCursorPosCallback(window, mouse_callback);


		//##### - CLEANING - #####//
		cube.CleanUp();
		vao.Unbind();
		vbo.Unbind();
		ebo.Unbind();
		shader.Unbind();



		//##### - LOCAL CONSTANTS - #####//
		camera.position = glm::vec3(0.0f, 0.0f, 5.0f);



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
			proj = glm::perspective(90.0f, (float)winWidth / (float)winHeight, 0.1f, 10.0f);
			view = camera.GetWorldToViewMatrix();
			glm::mat4 worldToClip = proj * view;


			//##### - Rendering
			vao.Bind();
			shader.Bind();
			shader.SetUniformMatrix4fv("worldToClip", &worldToClip[0][0]);

			glDrawElementsInstanced(GL_TRIANGLES, ebo.GetIndicesCount(), GL_UNSIGNED_SHORT, 0, 2);


			//Loop end
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}