#include "game.h"
#include "resource_manager.h"
#include "sprite_renderer.h"

SpriteRenderer *Renderer;


Game::Game(GLuint width, GLuint height) 
	: State(GAME_ACTIVE), Keys(), Width(width), Height(height) 
{ 

}

Game::~Game()
{
	delete Renderer;
}

void Game::Init()
{
    // Load shaders
    ResourceManager::LoadShader("resources/shaders/sprite.vert", "resources/shaders/sprite.frag", "sprite");
    // Configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    // Load textures
    ResourceManager::LoadTexture("resources/textures/cat.png", GL_TRUE, "cat");
    // Set render-specific controls
	Shader myShader = ResourceManager::GetShader("sprite");
    Renderer = new SpriteRenderer(myShader);
}

void Game::Update(GLfloat dt)
{

}


void Game::ProcessInput(GLfloat dt)
{

}

void Game::Render()
{
	Texture2D myTexture = ResourceManager::GetTexture("cat");
    Renderer->DrawSprite(myTexture, glm::vec2(200, 200), glm::vec2(300, 400), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
}