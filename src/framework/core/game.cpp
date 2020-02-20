#include "game.h"
#include "resource_manager.h"
#include "sprite_renderer.h"

SpriteRenderer* Renderer;


Game::Game(GLuint width, GLuint height) 
	: State(GAME_ACTIVE), Keys(), Width(width), Height(height) { }

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
    ResourceManager::LoadTexture("resources/textures/food.png", GL_TRUE, "food");
    ResourceManager::LoadTexture("resources/textures/wall.png", GL_TRUE, "wall");

    GameLevel one;
    one.Load("../../../levels/level0", this->Width, this->Height);
    this->Levels.push_back(one);
    this->Level = 0;
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
	//Texture2D myTexture = ResourceManager::GetTexture("food");
    //Renderer->DrawSprite(myTexture, glm::vec2(200, 200), glm::vec2(64, 64), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));


    if (this->State == GAME_ACTIVE)
    {
        // Draw level
        this->Levels[this->Level].Draw(*Renderer);
    }
}