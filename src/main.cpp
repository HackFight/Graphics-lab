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

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "imguiThemes.h"

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
glm::vec3 objectPos = glm::vec3(0.0f, 0.5f, 0.0f);
glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 lightPos = glm::vec3(1.2f, 1.0f, 2.0f);

glm::mat4 clip = glm::mat4(1.0f);
glm::mat4 proj = glm::mat4(1.0f);
glm::mat4 view = glm::mat4(1.0f);
glm::mat4 modelToClip = glm::mat4(1.0f);
glm::mat4 model1 = glm::mat4(1.0f);
glm::mat4 model2 = glm::mat4(1.0f);
glm::mat4 model3 = glm::mat4(1.0f);

int winWidth = 640, winHeight = 480;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool wireframe = false;
bool menu = false;
bool escReleased = true;


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

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		if (escReleased)
		{
			escReleased = false;
			if (menu)
			{
				menu = false;
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				glfwSetCursorPosCallback(window, mouse_callback);
			}
			else
			{
				menu = true;
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				glfwSetCursorPosCallback(window, NULL);
			}
		}
	}
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE)
		escReleased = true;
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

#pragma region imgui
#if REMOVE_IMGUI == 0
	ImGui::CreateContext();
	//ImGui::StyleColorsDark();				//you can use whatever imgui theme you like!
	//imguiThemes::yellow();
	//imguiThemes::gray();
	//imguiThemes::green();
	imguiThemes::red();
	//imguiThemes::embraceTheDarkness();

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;

	io.FontGlobalScale = 2.0f; //make text bigger please!

	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		//style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 0.f;
		style.Colors[ImGuiCol_DockingEmptyBg].w = 0.f;
	}

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
#endif
#pragma endregion

	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetCursorPosCallback(window, mouse_callback);


		//##### - DATA - #####//
		//##### - Cube
		ShapeData cube = ShapeGenerator::MakeCube();

		VAO cubeVao;
		VBO cubeVbo(cube.GetVertexBufferSize(), cube.vertices);
		VBOLayout cubeVboL;
		cubeVboL.Push<GLfloat>(3);	//Set position attrib
		cubeVboL.Push<GLfloat>(3);	//Set normals attrib
		cubeVboL.Push<GLfloat>(3);	//Set color attrib

		cubeVao.AddBuffer(cubeVbo, cubeVboL, 0);

		EBO cubeEbo(cube.GetIndexBufferCount(), cube.indices);

		//##### - Cleanup
		cube.CleanUp();

		cubeVao.Unbind();
		cubeVbo.Unbind();
		cubeEbo.Unbind();

		//##### - Plane
		ShapeData plane = ShapeGenerator::MakePlane(3, 2.0f);

		VAO planeVao;
		VBO planeVbo(plane.GetVertexBufferSize(), plane.vertices);
		VBOLayout planeVboL;
		planeVboL.Push<GLfloat>(3);	//Set position attrib
		planeVboL.Push<GLfloat>(3);	//Set normals attrib
		planeVboL.Push<GLfloat>(3);	//Set color attrib

		planeVao.AddBuffer(planeVbo, planeVboL, 0);

		EBO planeEbo(plane.GetIndexBufferCount(), plane.indices);

		//##### - Cleanup
		plane.CleanUp();

		planeVao.Unbind();
		planeVbo.Unbind();
		planeEbo.Unbind();



		//##### - SHADERS - #####//
		Shader light(RESOURCES_PATH "shaders/default.vert", RESOURCES_PATH "shaders/color.frag");
		Shader reciever(RESOURCES_PATH "shaders/default.vert", RESOURCES_PATH "shaders/phong.frag");
		Shader color(RESOURCES_PATH "shaders/default.vert", RESOURCES_PATH "shaders/default.frag");

		//##### - Cleanup
		light.Unbind();
		reciever.Unbind();
		color.Unbind();



		//##### - LOCAL CONSTANTS - #####//
		camera.position = glm::vec3(0.0f, 0.0f, 5.0f);

		model1 = glm::translate(model1, objectPos);

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

			if(wireframe)
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			else
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

#pragma region imgui
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
#pragma endregion


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

			color.Bind();
			color.SetUniformMatrix4fv("proj", proj);
			color.SetUniformMatrix4fv("view", view);
			color.SetUniformMatrix4fv("model", model3);


			//##### - Rendering
			cubeVao.Bind();

			reciever.Bind();
			glDrawElements(GL_TRIANGLES, cubeEbo.GetIndicesCount(), GL_UNSIGNED_SHORT, 0);

			light.Bind();
			glDrawElements(GL_TRIANGLES, cubeEbo.GetIndicesCount(), GL_UNSIGNED_SHORT, 0);

			planeVao.Bind();

			color.Bind();
			glDrawElements(GL_TRIANGLES, planeEbo.GetIndicesCount(), GL_UNSIGNED_SHORT, 0);


			//##### - Debug
			ImGui::Begin("Rendering");
			ImGui::Checkbox("Wireframe", &wireframe);
			ImGui::End();


#pragma region imgui
			ImGui::Render();
			int display_w, display_h;
			glfwGetFramebufferSize(window, &display_w, &display_h);
			glViewport(0, 0, display_w, display_h);
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			// Update and Render additional Platform Windows
			// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
			//  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				GLFWwindow* backup_current_context = glfwGetCurrentContext();
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
				glfwMakeContextCurrent(backup_current_context);
			}
#pragma endregion


			//Loop end
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}