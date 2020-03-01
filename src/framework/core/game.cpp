#include <sstream>
#include "game.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
#include "pac_object.h"
#include "util.cpp"
#include "text_renderer.h"

SpriteRenderer *Renderer;
PacObject *Player;
TextRenderer *Text;

Game::Game(GLuint width, GLuint height)
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height) {}

Game::~Game()
{
  delete Renderer;
  delete Player;
  delete Text;
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
  ResourceManager::LoadTexture("resources/textures/pacman.png", GL_TRUE, "pacman");

  GameLevel one;
  one.Load("resources/levels/level0", this->Width, this->Height);
  this->Levels.push_back(one);
  this->Level = 0;
  this->Score = 0;
  // Set render-specific controls
  Shader myShader = ResourceManager::GetShader("sprite");
  Renderer = new SpriteRenderer(myShader);

  Text = new TextRenderer(this->Width, this->Height);
  Text->Load("resources/fonts/ocraext.TTF", 24);

  // Configure player
  this->ResetPlayer();
}

void Game::Update(GLfloat dt)
{
  if (this->State == GAME_ACTIVE && this->Levels[this->Level].IsCompleted())
  {
    this->State = GAME_WIN;
  }
}

void Game::ProcessInput(GLfloat dt)
{
  if (this->State == GAME_ACTIVE)
  {
    bool collision = false;
    GLfloat velocity = this->Levels[this->Level].PLAYER_VELOCITY * dt;
    // Move playerboard
    if (this->Keys[GLFW_KEY_A])
    {
      for (GameObject &tile : this->Levels[this->Level].Tiles)
      {
        if (tile.IsVisible)
        {
          if (CheckCollision(*Player, tile, LEFT, velocity))
          {
            collision = true;
            if (!tile.IsSolid)
            {
              tile.IsVisible = GL_FALSE;
              this->Score++;

              if (Player->Position.x >= 0 - Player->Size.x)
                Player->Position.x -= velocity;
              else
                Player->Position.x = this->Width;
            }
          }
        }
      }

      if (!collision)
      {
        if (Player->Position.x >= 0 - Player->Size.x)
          Player->Position.x -= velocity;
        else
          Player->Position.x = this->Width;
      }
    }
    if (this->Keys[GLFW_KEY_D])
    {
      for (GameObject &tile : this->Levels[this->Level].Tiles)
      {
        if (tile.IsVisible)
        {
          if (CheckCollision(*Player, tile, RIGHT, velocity))
          {
            collision = true;
            if (!tile.IsSolid)
            {
              tile.IsVisible = GL_FALSE;
              this->Score++;

              if (Player->Position.x <= this->Width)
                Player->Position.x += velocity;
              else
                Player->Position.x = 0 - Player->Size.x;
            }
          }
        }
      }

      if (!collision)
      {
        if (Player->Position.x <= this->Width)
          Player->Position.x += velocity;
        else
          Player->Position.x = 0 - Player->Size.x;
      }
    }
    if (this->Keys[GLFW_KEY_W])
    {
      for (GameObject &tile : this->Levels[this->Level].Tiles)
      {
        if (tile.IsVisible)
        {
          if (CheckCollision(*Player, tile, UP, velocity))
          {
            collision = true;
            if (!tile.IsSolid)
            {
              tile.IsVisible = GL_FALSE;
              this->Score++;

              if (Player->Position.y >= 0)
                Player->Position.y -= velocity;
            }
          }
        }
      }

      if (!collision)
      {

        if (Player->Position.y >= 0)
          Player->Position.y -= velocity;
      }
    }
    if (this->Keys[GLFW_KEY_S])
    {
      for (GameObject &tile : this->Levels[this->Level].Tiles)
      {
        if (tile.IsVisible)
        {
          if (CheckCollision(*Player, tile, DOWN, velocity))
          {
            collision = true;
            if (!tile.IsSolid)
            {
              tile.IsVisible = GL_FALSE;
              this->Score++;

              if (Player->Position.y <= this->Height - Player->Size.y)
                Player->Position.y += velocity;
            }
          }
        }
      }

      if (!collision)
      {

        if (Player->Position.y <= this->Height - Player->Size.y)
          Player->Position.y += velocity;
      }
    }
  }
}

void Game::Render()
{
  std::stringstream ss;
  ss << this->Score;

  if (this->State == GAME_ACTIVE)
  {
    // Draw level
    this->Levels[this->Level].Draw(*Renderer);
    Player->Draw(*Renderer);
    Text->RenderText("Score:" + ss.str(), 5.0f, 5.0f, 1.0f);
  } else if (this->State == GAME_WIN) {
    Text->RenderText("You win!", 320.0, Height / 2 - 20.0, 1.0);
    Text->RenderText("Score:" + ss.str(), 325.0, Height / 2, 1.0);
  }
}

void Game::ResetLevel() {
  this->Levels[0].Load("resources/levels/level0", this->Width, this->Height);
  this->Score = 0;
}

void Game::ResetPlayer() {
  glm::vec2 playerPos = glm::vec2(this->Levels[this->Level].PLAYER_POSITION.x, this->Levels[this->Level].PLAYER_POSITION.y);
  GLfloat playerRadius = this->Levels[this->Level].PLAYER_RADIUS;
  GLfloat playerVelocity = this->Levels[this->Level].PLAYER_VELOCITY;
  Player = new PacObject(playerPos, playerRadius, playerVelocity, ResourceManager::GetTexture("pacman"));
}