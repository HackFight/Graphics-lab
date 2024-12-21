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

//##### - STRUCTS - #####//
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



		//##### - DATA - #####//
		//##### - Square
		ShapeData square = ShapeGenerator::MakeSquare();

		VAO squareVao;
		VBO squareVbo(square.GetVertexBufferSize(), square.vertices);
		VBOLayout squareVboL;
		squareVboL.Push<GLfloat>(3);	//Set position attrib
		squareVboL.Push<GLfloat>(2);	//Set UV attrib
		squareVboL.Push<GLfloat>(3);	//Set normals attrib
		squareVboL.Push<GLfloat>(3);	//Set color attrib

		squareVao.AddBuffer(squareVbo, squareVboL, 0);

		EBO squareEbo(square.GetIndexBufferCount(), square.indices);

		//##### - Cleanup
		square.CleanUp();

		squareVao.Unbind();
		squareVbo.Unbind();
		squareEbo.Unbind();

		//##### - Plane
		ShapeData plane = ShapeGenerator::MakePlane(100, 100.0f);

		VAO planeVao;
		VBO planeVbo(plane.GetVertexBufferSize(), plane.vertices);
		VBOLayout planeVboL;
		planeVboL.Push<GLfloat>(3);	//Set position attrib
		planeVboL.Push<GLfloat>(2);	//Set UV attrib
		planeVboL.Push<GLfloat>(3);	//Set normals attrib
		planeVboL.Push<GLfloat>(3);	//Set color attrib

		planeVao.AddBuffer(planeVbo, planeVboL, 0);

		EBO planeEbo(plane.GetIndexBufferCount(), plane.indices);

		//##### - Cleanup
		plane.CleanUp();

		planeVao.Unbind();
		planeVbo.Unbind();
		planeEbo.Unbind();

		//##### - Cube
		ShapeData cube = ShapeGenerator::MakeCube();

		VAO cubeVao;
		VBO cubeVbo(cube.GetVertexBufferSize(), cube.vertices);
		VBOLayout cubeVboL;
		cubeVboL.Push<GLfloat>(3);	//Set position attrib
		cubeVboL.Push<GLfloat>(2);	//Set UV attrib
		cubeVboL.Push<GLfloat>(3);	//Set normals attrib
		cubeVboL.Push<GLfloat>(3);	//Set color attrib

		cubeVao.AddBuffer(cubeVbo, cubeVboL, 0);

		EBO cubeEbo(cube.GetIndexBufferCount(), cube.indices);

		//##### - Cleanup
		cube.CleanUp();

		cubeVao.Unbind();
		cubeVbo.Unbind();
		cubeEbo.Unbind();



		//##### - RENDERER - #####//
		Renderer renderer;



		//##### - SHADERS - #####//
		Shader sDebugColor(RESOURCES_PATH "shaders/default.vert", RESOURCES_PATH "shaders/default.frag");
		Shader sUV(RESOURCES_PATH "shaders/default.vert", RESOURCES_PATH "shaders/UV.frag");
		Shader sLight(RESOURCES_PATH "shaders/default.vert", RESOURCES_PATH "shaders/color.frag");
		Shader sTexture(RESOURCES_PATH "shaders/default.vert", RESOURCES_PATH "shaders/texture.frag");
		Shader sPhong(RESOURCES_PATH "shaders/default.vert", RESOURCES_PATH "shaders/phong.frag");
		Shader sTexturePhong(RESOURCES_PATH "shaders/default.vert", RESOURCES_PATH "shaders/texturePhong.frag");



		//##### - TEXTURES - #####//
		Texture tBlack(GL_TEXTURE2, RESOURCES_PATH "textures/black.png", GL_RGBA, true);

		Texture tColor(GL_TEXTURE0, RESOURCES_PATH "textures/container.png", GL_RGBA, false);
		Texture tSpecular(GL_TEXTURE1, RESOURCES_PATH "textures/container_specular.png", GL_RGBA, false);
		Texture tEmission(GL_TEXTURE2, RESOURCES_PATH "textures/container_emission.png", GL_RGB, false);

		Texture tilesColor(GL_TEXTURE0, RESOURCES_PATH "textures/tiles.png", GL_RGBA, true);
		Texture tilesSpecular(GL_TEXTURE1, RESOURCES_PATH "textures/tiles_specular.png", GL_RGBA, true);
		
		
		
		//##### - MATERIALS - #####//
		PhongTextureMaterial jinxCubeMat;
		jinxCubeMat.shader = &sTexturePhong;
		jinxCubeMat.colorTexID = tColor.GetID();
		jinxCubeMat.specularTexID = tSpecular.GetID();
		jinxCubeMat.emissionTexID = tEmission.GetID();

		PhongTextureMaterial tilesMat;
		tilesMat.shader = &sTexturePhong;
		tilesMat.colorTexID = tilesColor.GetID();
		tilesMat.specularTexID = tilesSpecular.GetID();
		tilesMat.emissionTexID = tBlack.GetID();

		PhongMaterial groundMaterial;
		groundMaterial.shader = &sPhong;
		groundMaterial.diffuse = planeColor;

		TextureMaterial containerTextureMat;
		containerTextureMat.shader = &sTexture;

		TextureMaterial tilesTextureMat;
		tilesTextureMat.shader = &sTexture;

		UnlitMaterial defaultMat;
		defaultMat.shader = &sLight;
		defaultMat.color = glm::vec3(1.0f, 0.0f, 1.0f);



		//##### - Cleanup
		sLight.Unbind();
		sUV.Unbind();
		sPhong.Unbind();
		sDebugColor.Unbind();

		tColor.Unbind();
		tSpecular.Unbind();
		tEmission.Unbind();



		//##### - LIGHTS - #####//
		DirLight dirLight;
		dirLight.enabled = false;
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
			model1 = glm::mat4(1.0f);
			model3 = glm::mat4(1.0f);

			model1 = glm::translate(model1, cubePos);
			model3 = glm::translate(model3, planePos);

			//##### - Rendering
			if (current_shader == 0)
			{
				sDebugColor.Bind();

				//Render cube
				sDebugColor.SetUniformMatrix4fv("model", model1);
				renderer.Draw(cubeVao, cubeEbo, sDebugColor);

				//Render lights
				for (int i = 0; i < POINT_LIGHTS_NUMBER; i++)
				{
					if (pointLights[i].enabled)
					{
						lightModel = glm::mat4(1.0f);
						lightModel = glm::translate(lightModel, pointLights[i].position);
						lightModel = glm::scale(lightModel, glm::vec3(0.1f, 0.1f, 0.1f));
						sDebugColor.SetUniformMatrix4fv("model", lightModel);
						renderer.Draw(cubeVao, cubeEbo, sDebugColor);
					}
				}

				//Render plane
				sDebugColor.SetUniformMatrix4fv("model", model3);
				renderer.Draw(planeVao, planeEbo, sDebugColor);
			}
			else if (current_shader == 1)
			{
				sUV.Bind();

				//Render cube
				sUV.SetUniformMatrix4fv("model", model1);
				renderer.Draw(cubeVao, cubeEbo, sUV);

				//Render lights
				for (int i = 0; i < POINT_LIGHTS_NUMBER; i++)
				{
					if (pointLights[i].enabled)
					{
						lightModel = glm::mat4(1.0f);
						lightModel = glm::translate(lightModel, pointLights[i].position);
						lightModel = glm::scale(lightModel, glm::vec3(0.1f, 0.1f, 0.1f));
						sUV.SetUniformMatrix4fv("model", lightModel);
						renderer.Draw(cubeVao, cubeEbo, sUV);
					}
				}

				//Render plane
				sUV.SetUniformMatrix4fv("model", model3);
				renderer.Draw(planeVao, planeEbo, sUV);
			}
			else if (current_shader == 2)
			{
				//Set cube texture
				if (current_texture == 0)
				{
					containerTextureMat.textureID = tColor.GetID();
					tilesTextureMat.textureID = tilesColor.GetID();
				}
				else if (current_texture == 1)
				{
					containerTextureMat.textureID = tSpecular.GetID();
					tilesTextureMat.textureID = tilesSpecular.GetID();
				}
				else if (current_texture == 2)
				{
					containerTextureMat.textureID = tEmission.GetID();
					tilesTextureMat.textureID = tBlack.GetID();
				}

				//Render cube
				containerTextureMat.Bind();
				sTexture.SetUniformMatrix4fv("model", model1);
				renderer.DrawMaterial(cubeVao, cubeEbo, containerTextureMat);

				//Render lights
				defaultMat.Bind();
				for (int i = 0; i < POINT_LIGHTS_NUMBER; i++)
				{
					if (pointLights[i].enabled)
					{
						lightModel = glm::mat4(1.0f);
						lightModel = glm::translate(lightModel, pointLights[i].position);
						lightModel = glm::scale(lightModel, glm::vec3(0.1f, 0.1f, 0.1f));
						sLight.SetUniformMatrix4fv("model", lightModel);
						renderer.DrawMaterial(cubeVao, cubeEbo, defaultMat);
					}
				}

				//Render plane
				tilesTextureMat.Bind();
				sTexture.SetUniformMatrix4fv("model", model3);
				renderer.DrawMaterial(planeVao, planeEbo, tilesTextureMat);
			}
			else if (current_shader == 3)
			{
				sPhong.Bind();
				sPhong.SetUniform3fv("viewPos", camera.position);

				sPhong.SetUniform1b("dirLight.enabled", dirLight.enabled);
				sPhong.SetUniform3fv("dirLight.direction", dirLight.direction);
				sPhong.SetUniform3fv("dirLight.ambient", dirLight.color * 0.2f);
				sPhong.SetUniform3fv("dirLight.diffuse", dirLight.color * 0.5f);
				sPhong.SetUniform3fv("dirLight.specular", dirLight.color * 1.0f);

				for (int i = 0; i < POINT_LIGHTS_NUMBER; i++)
				{
					sPhong.SetUniform1b("pointLights[" + std::to_string(i) + "].enabled", pointLights[i].enabled);
					sPhong.SetUniform3fv("pointLights[" + std::to_string(i) + "].position", pointLights[i].position);
					sPhong.SetUniform3fv("pointLights[" + std::to_string(i) + "].ambient", pointLights[i].color * 0.2f);
					sPhong.SetUniform3fv("pointLights[" + std::to_string(i) + "].diffuse", pointLights[i].color * 0.5f);
					sPhong.SetUniform3fv("pointLights[" + std::to_string(i) + "].specular", pointLights[i].color * 1.0f);
					sPhong.SetUniform1f("pointLights[" + std::to_string(i) + "].constant", 1.0f);
					sPhong.SetUniform1f("pointLights[" + std::to_string(i) + "].linear", 0.09f);
					sPhong.SetUniform1f("pointLights[" + std::to_string(i) + "].quadratic", 0.032f);
				}

				sTexturePhong.Bind();
				sTexturePhong.SetUniform3fv("viewPos", camera.position);

				sTexturePhong.SetUniform1b("dirLight.enabled", dirLight.enabled);
				sTexturePhong.SetUniform3fv("dirLight.direction", dirLight.direction);
				sTexturePhong.SetUniform3fv("dirLight.ambient", dirLight.color * 0.2f);
				sTexturePhong.SetUniform3fv("dirLight.diffuse", dirLight.color * 0.5f);
				sTexturePhong.SetUniform3fv("dirLight.specular", dirLight.color * 1.0f);

				for (int i = 0; i < POINT_LIGHTS_NUMBER; i++)
				{
					sTexturePhong.SetUniform1b("pointLights[" + std::to_string(i) + "].enabled", pointLights[i].enabled);
					sTexturePhong.SetUniform3fv("pointLights[" + std::to_string(i) + "].position", pointLights[i].position);
					sTexturePhong.SetUniform3fv("pointLights[" + std::to_string(i) + "].ambient", pointLights[i].color * 0.2f);
					sTexturePhong.SetUniform3fv("pointLights[" + std::to_string(i) + "].diffuse", pointLights[i].color * 0.5f);
					sTexturePhong.SetUniform3fv("pointLights[" + std::to_string(i) + "].specular", pointLights[i].color * 1.0f);
					sTexturePhong.SetUniform1f("pointLights[" + std::to_string(i) + "].constant", 1.0f);
					sTexturePhong.SetUniform1f("pointLights[" + std::to_string(i) + "].linear", 0.09f);
					sTexturePhong.SetUniform1f("pointLights[" + std::to_string(i) + "].quadratic", 0.032f);
				}


				//Render jinx cube
				jinxCubeMat.Bind();
				sTexturePhong.SetUniformMatrix4fv("model", model1);
				renderer.DrawMaterial(cubeVao, cubeEbo, jinxCubeMat);


				//Render lights
				sLight.Bind();
				for (int i = 0; i < POINT_LIGHTS_NUMBER; i++)
				{
					if (pointLights[i].enabled)
					{
						lightModel = glm::mat4(1.0f);
						lightModel = glm::translate(lightModel, pointLights[i].position);
						lightModel = glm::scale(lightModel, glm::vec3(0.1f, 0.1f, 0.1f));
						sLight.SetUniformMatrix4fv("model", lightModel);
						sLight.SetUniform3fv("color", pointLights[i].color);
						renderer.Draw(cubeVao, cubeEbo, sLight);
					}
				}


				//Render plane
				tilesMat.Bind();
				sTexturePhong.SetUniformMatrix4fv("model", model3);
				renderer.DrawMaterial(planeVao, planeEbo, tilesMat);
			}


			//##### - Debug
			ImGui::Begin("Rendering");
			if (ImGui::CollapsingHeader("Shaders"))
			{
				const char* shaders[] = { "Debug", "UV", "Texture", "Render" };
				ImGui::Combo("Shader mode", &current_shader, shaders, sizeof(shaders) / sizeof(*shaders));

				if (current_shader == 2)
				{
					const char* textures[] = { "Color", "Specular", "Emission"};
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