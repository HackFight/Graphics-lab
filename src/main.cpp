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
#include <stb/stb_img.h>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "imguiThemes.h"

#include <primitives/vbo.h>
#include <primitives/ebo.h>
#include <primitives/vao.h>
#include <primitives/shapeGenerator.h>
#include <primitives/shader.h>
#include <primitives/texture.h>
#include <primitives/materials.h>
#include <renderer.h>
#include <camera.h>
#include <lights.h>

//##### - GENERAL VARIABLES - #####//
Camera camera;
const float cameraSpeed = 2.5f; // adjust accordingly

glm::vec3 cubePos = glm::vec3(0.0f, 0.5f, 0.0f);

glm::vec3 planeColor = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 planePos = glm::vec3(0.0f, 0.0f, 0.0f);

glm::mat4 model1 = glm::mat4(1.0f);
glm::mat4 lightModel = glm::mat4(1.0f);
glm::mat4 model3 = glm::mat4(1.0f);

int current_shader = 3;
int current_texture = 0;

int winWidth = 640, winHeight = 480;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool wireframe = false;
bool cursorReleased = false;
bool escReleased = true;

const unsigned int POINT_LIGHTS_NUMBER = 1;



//##### - FUNCTIONS - #####//
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if(!cursorReleased)
		camera.UpdateMouse(glm::vec2(xpos, ypos));

	ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);
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
			if (cursorReleased)
			{
				cursorReleased = false;
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			}
			else
			{
				cursorReleased = true;
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
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
	imguiThemes::green();
	//imguiThemes::red();
	//imguiThemes::embraceTheDarkness();

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;

	io.FontGlobalScale = 1.0f; //make text bigger please!

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



		//##### - RENDERER - #####//
		Renderer renderer;



		//##### - TEXTURES - #####//
		Texture jinxCubeTextures[] =
		{
			Texture(GL_TEXTURE0, "diffuse", RESOURCES_PATH "textures/container.png", GL_RGBA, false),
			Texture(GL_TEXTURE1, "specular", RESOURCES_PATH "textures/container_specular.png", GL_RGBA, false),
			Texture(GL_TEXTURE2, "emission", RESOURCES_PATH "textures/container_emission.png", GL_RGB, false)
		};

		Texture groundTilesTextures[] =
		{
			Texture(GL_TEXTURE0, "diffuse", RESOURCES_PATH "textures/tiles.png", GL_RGBA, true),
			Texture(GL_TEXTURE1, "specular", RESOURCES_PATH "textures/tiles_specular.png", GL_RGBA, true),
			Texture(GL_TEXTURE2, "emission", RESOURCES_PATH "textures/black.png", GL_RGB, true)
		};



		//##### - MESHES - #####//
		ShapeData cube = ShapeGenerator::MakeCube();
		std::vector<Vertex> verts(cube.vertices, cube.vertices + sizeof(*cube.vertices) / sizeof(Vertex));
		std::vector<GLuint> ind(cube.indices, cube.indices + sizeof(*cube.indices) / sizeof(GLuint));
		std::vector<Texture> tex(jinxCubeTextures, jinxCubeTextures + sizeof(jinxCubeTextures) / sizeof(Texture));
		Mesh jinxCube(verts, ind, tex);

		Mesh lightCube(verts, ind, tex);	//Same vertices and indices for the light, the textures are a placeholder

		ShapeData plane = ShapeGenerator::MakePlane(100, 100.0f);
		verts = std::vector<Vertex>(plane.vertices, plane.vertices + sizeof(*plane.vertices) / sizeof(Vertex));
		ind = std::vector<GLuint>(plane.indices, plane.indices + sizeof(*plane.indices) / sizeof(GLuint));
		tex = std::vector<Texture>(groundTilesTextures, groundTilesTextures + sizeof(groundTilesTextures) / sizeof(Texture));
		Mesh groundTiles(verts, ind, tex);



		//##### - SHADERS - #####//
		Shader debugColorShader(RESOURCES_PATH "shaders/default.vert", RESOURCES_PATH "shaders/default.frag");
		Shader UVShader(RESOURCES_PATH "shaders/default.vert", RESOURCES_PATH "shaders/UV.frag");
		Shader unlitShader(RESOURCES_PATH "shaders/default.vert", RESOURCES_PATH "shaders/color.frag");
		Shader unlitTextureShader(RESOURCES_PATH "shaders/default.vert", RESOURCES_PATH "shaders/texture.frag");
		Shader phongShader(RESOURCES_PATH "shaders/default.vert", RESOURCES_PATH "shaders/phong.frag");
		Shader texturePhongShader(RESOURCES_PATH "shaders/default.vert", RESOURCES_PATH "shaders/texturePhong.frag");



		//##### - MATERIALS - #####//
		Material jinxCubeMat;
		jinxCubeMat.shader = &texturePhongShader;

		Material tilesMat;
		tilesMat.shader = &texturePhongShader;

		UnlitMaterial defaultMat;
		defaultMat.shader = &unlitShader;
		defaultMat.color = glm::vec3(1.0f, 0.0f, 1.0f);

		Material debugMat;
		debugMat.shader = &debugColorShader;



		//##### - Cleanup
		debugColorShader.Unbind(); //Unbinding one shader unbinds them all

		for each(Texture texture in jinxCubeTextures) //Not for textures though... this is still very useless.
		{
			texture.Unbind();
		}
		for each(Texture texture in groundTilesTextures)
		{
			texture.Unbind();
		}



		//##### - LIGHTS - #####//
		DirLight dirLight;
		dirLight.enabled = true;
		dirLight.direction = glm::vec3(0.2f, -1.0f, -0.5f);
		dirLight.color = glm::vec3(1.0f, 1.0f, 1.0f);

		PointLight pointLights[POINT_LIGHTS_NUMBER];
		for (int i = 0; i < POINT_LIGHTS_NUMBER; i++)
		{
			pointLights[i].position = glm::vec3(i - (float)POINT_LIGHTS_NUMBER / 2.0f, 3.0f, 0.0f);
		}



		//##### - LOCAL CONSTANTS - #####//
		camera.position = glm::vec3(0.0f, 2.0f, 5.0f);

		// ### - Main loop - ### //
		while (!glfwWindowShouldClose(window))
		{
			//Loop start
			glfwGetFramebufferSize(window, &winWidth, &winHeight);
			glViewport(0, 0, winWidth, winHeight);
			renderer.Clear();
			renderer.Update(winWidth, winHeight, camera);

			if (wireframe)
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
			model1 = glm::mat4(1.0f);
			model3 = glm::mat4(1.0f);

			model1 = glm::translate(model1, cubePos);
			model3 = glm::translate(model3, planePos);


			//##### - Rendering
			phongShader.Bind();
			phongShader.SetUniform3fv("viewPos", camera.position);

			phongShader.SetUniform1b("dirLight.enabled", dirLight.enabled);
			phongShader.SetUniform3fv("dirLight.direction", dirLight.direction);
			phongShader.SetUniform3fv("dirLight.ambient", dirLight.color * 0.2f);
			phongShader.SetUniform3fv("dirLight.diffuse", dirLight.color * 0.5f);
			phongShader.SetUniform3fv("dirLight.specular", dirLight.color * 1.0f);

			for (int i = 0; i < POINT_LIGHTS_NUMBER; i++)
			{
				phongShader.SetUniform1b("pointLights[" + std::to_string(i) + "].enabled", pointLights[i].enabled);
				phongShader.SetUniform3fv("pointLights[" + std::to_string(i) + "].position", pointLights[i].position);
				phongShader.SetUniform3fv("pointLights[" + std::to_string(i) + "].ambient", pointLights[i].color * 0.2f);
				phongShader.SetUniform3fv("pointLights[" + std::to_string(i) + "].diffuse", pointLights[i].color * 0.5f);
				phongShader.SetUniform3fv("pointLights[" + std::to_string(i) + "].specular", pointLights[i].color * 1.0f);
				phongShader.SetUniform1f("pointLights[" + std::to_string(i) + "].constant", 1.0f);
				phongShader.SetUniform1f("pointLights[" + std::to_string(i) + "].linear", 0.09f);
				phongShader.SetUniform1f("pointLights[" + std::to_string(i) + "].quadratic", 0.032f);
			}

			texturePhongShader.Bind();
			texturePhongShader.SetUniform3fv("viewPos", camera.position);

			texturePhongShader.SetUniform1b("dirLight.enabled", dirLight.enabled);
			texturePhongShader.SetUniform3fv("dirLight.direction", dirLight.direction);
			texturePhongShader.SetUniform3fv("dirLight.ambient", dirLight.color * 0.2f);
			texturePhongShader.SetUniform3fv("dirLight.diffuse", dirLight.color * 0.5f);
			texturePhongShader.SetUniform3fv("dirLight.specular", dirLight.color * 1.0f);

			for (int i = 0; i < POINT_LIGHTS_NUMBER; i++)
			{
				texturePhongShader.SetUniform1b("pointLights[" + std::to_string(i) + "].enabled", pointLights[i].enabled);
				texturePhongShader.SetUniform3fv("pointLights[" + std::to_string(i) + "].position", pointLights[i].position);
				texturePhongShader.SetUniform3fv("pointLights[" + std::to_string(i) + "].ambient", pointLights[i].color * 0.2f);
				texturePhongShader.SetUniform3fv("pointLights[" + std::to_string(i) + "].diffuse", pointLights[i].color * 0.5f);
				texturePhongShader.SetUniform3fv("pointLights[" + std::to_string(i) + "].specular", pointLights[i].color * 1.0f);
				texturePhongShader.SetUniform1f("pointLights[" + std::to_string(i) + "].constant", 1.0f);
				texturePhongShader.SetUniform1f("pointLights[" + std::to_string(i) + "].linear", 0.09f);
				texturePhongShader.SetUniform1f("pointLights[" + std::to_string(i) + "].quadratic", 0.032f);
			}


			//Render jinx cube
			if (current_shader == 0)
			{
				debugMat.Bind();
				debugMat.shader->SetUniformMatrix4fv("model", model1);
				renderer.DrawMesh(jinxCube, debugMat);
			}
			else if (current_shader == 1)
			{
			}
			else if (current_shader == 2)
			{
			}
			else if (current_shader == 3)
			{
				jinxCubeMat.Bind();
				jinxCubeMat.shader->SetUniformMatrix4fv("model", model1);
				renderer.DrawMesh(jinxCube, jinxCubeMat);
			}


			/*//Render lights
			for (int i = 0; i < POINT_LIGHTS_NUMBER; i++)
			{
				if (pointLights[i].enabled)
				{
					lightModel = glm::mat4(1.0f);
					lightModel = glm::translate(lightModel, pointLights[i].position);
					lightModel = glm::scale(lightModel, glm::vec3(0.1f, 0.1f, 0.1f));

					defaultMat.Bind();
					unlitShader.SetUniformMatrix4fv("model", lightModel);
					renderer.DrawMesh(lightCube, defaultMat);
				}
			}*/


			/*//Render plane
			groundMat.Bind();
			texturePhongShader.SetUniformMatrix4fv("model", model3);
			renderer.DrawMesh(groundTiles, groundMat);*/


			//##### - Debug
			ImGui::Begin("Rendering");
			if (ImGui::CollapsingHeader("Shaders"))
			{
				const char* shaders[] = { "Debug", "UV", "Texture", "Render" };
				ImGui::Combo("Shader mode", &current_shader, shaders, sizeof(shaders) / sizeof(*shaders));

				if (current_shader == 2)
				{
					const char* textures[] = { "Color", "Specular", "Emission" };
					ImGui::Combo("Texture", &current_texture, textures, sizeof(textures) / sizeof(*textures));
				}
			}

			if (ImGui::CollapsingHeader("Lights"))
			{
				if (ImGui::TreeNode("Directional light"))
				{
					ImGui::Checkbox("Enable", &dirLight.enabled);
					ImGui::SliderFloat3("Direction", &dirLight.direction[0], -5.0f, 5.0f);
					ImGui::ColorEdit3("Color", &dirLight.color[0]);
					ImGui::TreePop();
				}
				if (ImGui::TreeNode("Point lights"))
				{
					for (int i = 0; i < POINT_LIGHTS_NUMBER; i++)
					{
						ImGui::PushID(i);
						if (ImGui::TreeNode(("Light " + std::to_string(i)).c_str()))
						{
							ImGui::Checkbox("Enable", &pointLights[i].enabled);
							ImGui::SliderFloat3("Position", &pointLights[i].position[0], -5.0f, 5.0f);
							ImGui::ColorEdit3("Color", &pointLights[i].color[0]);
							ImGui::TreePop();
						}
						ImGui::PopID();
					}
					ImGui::TreePop();
				}
			}

			if (ImGui::CollapsingHeader("Miscellaneous"))
			{
				ImGui::Checkbox("Wireframe", &wireframe);
			}
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

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}