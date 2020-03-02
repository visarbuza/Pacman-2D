#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <stb_image.h>
#include <gfx.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include "game.h"
#include "resource_manager.h"

// GLFW function declerations
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// The Width of the screen
const GLuint SCREEN_WIDTH = 768;
// The height of the screen
const GLuint SCREEN_HEIGHT = 900;

const auto glsl_version = "#version 430 core";

Game Pacman(SCREEN_WIDTH, SCREEN_HEIGHT);

int main(int argc, char* argv[])
{
	if (!glfwInit())
	{
		GFX_ERROR("Failed to initialize GLFW\n");
		return -1;
	}
	glfwWindowHint(GLFW_SAMPLES, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pacman", nullptr, nullptr);
	if (window == nullptr)
	{
		GFX_ERROR("Failed to open GLFW window.\n");
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGL())
	{
		GFX_ERROR("Failed to initialize GLAD\n");
	}

	glfwSetKeyCallback(window, key_callback);

	// OpenGL configuration
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);


	// Initialize game
	Pacman.Init();

	// DeltaTime variables
	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;

	// Start Game within Menu State
	Pacman.State = GAME_MENU;


	do
	{
		// Calculate delta time
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glfwPollEvents();

		//deltaTime = 0.001f;
		// Manage user input
		Pacman.ProcessInput(deltaTime);

		// Update Game state
		Pacman.Update(deltaTime);

		// Render
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		Pacman.Render();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		if (Pacman.State == GAME_PAUSED || Pacman.State == GAME_MENU)
		{
			ImGui::Begin("Pacman");
			if (Pacman.State == GAME_PAUSED) {
				if (ImGui::Button("Resume Game")) {
					Pacman.State = GAME_ACTIVE;
				}
			} else {
				if (ImGui::Button("New Game"))
				{
					Pacman.State = GAME_ACTIVE;
          Pacman.ResetLevel();
          Pacman.ResetPlayer();
				}
			}
			if (ImGui::Button("Exit Game"))
			{
				glfwSetWindowShouldClose(window, GL_TRUE);
			}
			ImGui::End();
		}



		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	} while (!glfwWindowShouldClose(window));

	//Delete all resources as loaded using the resource manager
	ResourceManager::Clear();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// When a user presses the escape key, we set the state to GAME_PAUSED which pulls up the menu.
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    if (Pacman.State == GAME_WIN || Pacman.State == GAME_LOSS) 
      Pacman.State = GAME_MENU;
		else
      Pacman.State = GAME_PAUSED;
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			Pacman.Keys[key] = GL_TRUE;
		else if (action == GLFW_RELEASE)
			Pacman.Keys[key] = GL_FALSE;
	}
}


