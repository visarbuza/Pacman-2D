#include <sstream>
#include <cstdlib>
#include "game.h"
#include "resource_manager.h"
#include <iostream>
#include "sprite_renderer.h"
#include "pac_object.h"
#include "ghost_object.h"
#include "util.cpp"
#include "text_renderer.h"
#include "direction.h"
#include <vector>

SpriteRenderer  *Renderer;
PacObject       *Player;
TextRenderer    *Text;
GhostObject           *Blinky;
GhostObject           *Pinky;
GhostObject           *Inky;
GhostObject           *Clyde;
GLfloat animation = 0;
Direction blinkyDir = Direction::DOWN;
Direction pinkyDir = Direction::DOWN;
Direction inkyDir = Direction::DOWN;
Direction clydeDir = Direction::DOWN;

Game::Game(GLuint width, GLuint height)
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height) {}

Game::~Game()
{
  delete Renderer;
  delete Player;
  delete Text;
  delete Blinky;
  delete Pinky;
  delete Inky;
  delete Clyde;
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
  ResourceManager::LoadTexture("resources/textures/food.png", GL_TRUE, "food", false);
  ResourceManager::LoadTexture("resources/textures/wall.png", GL_TRUE, "wall", false);
  ResourceManager::LoadTexture("resources/textures/sprite.png", GL_TRUE, "pacman", true);
  ResourceManager::LoadTexture("resources/textures/ghost.png", GL_TRUE, "ghost", false);
  ResourceManager::LoadTexture("resources/textures/ghost2.png", GL_TRUE, "ghost2", false);
  ResourceManager::LoadTexture("resources/textures/ghost3.png", GL_TRUE, "ghost3", false);
  ResourceManager::LoadTexture("resources/textures/ghost4.png", GL_TRUE, "ghost4", false);

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
  this->ResetGhosts();
}

void Game::Update(GLfloat dt)
{
  animation += dt;

  ProcessInput(dt);
  if (this->State == GAME_ACTIVE && this->Levels[this->Level].IsCompleted())
  {
    this->State = GAME_WIN;
  }

  if (this->State == GAME_ACTIVE)
  {
    if(animation < 0.08){
      Player->Sprite = ResourceManager::GetTexture("pacman0");  
    } else if (animation < 0.16) {
      Player->Sprite = ResourceManager::GetTexture("pacman1");
    } else if (animation < 0.24) {
      Player->Sprite = ResourceManager::GetTexture("pacman2");
    } else if (animation < 0.32) {
      Player->Sprite = ResourceManager::GetTexture("pacman3");
    } else if (animation < 0.4) {
      Player->Sprite = ResourceManager::GetTexture("pacman2");
    } else if (animation < 0.48) {
      Player->Sprite = ResourceManager::GetTexture("pacman1");
    } else {
      animation = 0;
    }
    
    CheckForDeath();
    GLfloat velocity = this->Levels[this->Level].PLAYER_VELOCITY * dt;
    bool collisionBlinky = false, collisionInky = false, collisionPinky = false, collisionClyde = false;
    if (animation < 0.05) {
      blinkyDir = this->GenerateRandomDirection();
      pinkyDir = this->GenerateRandomDirection();
      inkyDir = this->GenerateRandomDirection();
      clydeDir = this->GenerateRandomDirection();
    }


    for (GameObject &tile : this->Levels[this->Level].Tiles)
    {
      if (tile.IsVisible && tile.IsSolid && CheckCollision(*Blinky, tile, blinkyDir, velocity))
      {
        collisionBlinky = true;
      }

      if (tile.IsVisible && tile.IsSolid && CheckCollision(*Inky, tile, inkyDir, velocity))
      {
        collisionInky = true;
      }

      if (tile.IsVisible && tile.IsSolid && CheckCollision(*Pinky, tile, pinkyDir, velocity))
      {
        collisionPinky = true;
      }

      if (tile.IsVisible && tile.IsSolid && CheckCollision(*Clyde, tile, clydeDir, velocity))
      {
        collisionClyde = true;
      }
    }

    if (!collisionBlinky) {
      Blinky->Move(blinkyDir, velocity);
    }

    if (!collisionInky) {
      Inky->Move(inkyDir, velocity);
    }

    if (!collisionPinky) {
      Pinky->Move(pinkyDir, velocity);
    }

    if (!collisionClyde) {
      Clyde->Move(clydeDir, velocity);
    }
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
      Player->Rotation = 3.14 / 2;
      for (GameObject &tile : this->Levels[this->Level].Tiles)
      {
        if (tile.IsVisible)
        {
          if (CheckCollision(*Player, tile, Direction::LEFT, velocity))
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
      Player->Rotation = -3.14 / 2;
      for (GameObject &tile : this->Levels[this->Level].Tiles)
      {
        if (tile.IsVisible)
        {
          if (CheckCollision(*Player, tile, Direction::RIGHT, velocity))
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
      Player->Rotation = 3.14;
      for (GameObject &tile : this->Levels[this->Level].Tiles)
      {
        if (tile.IsVisible)
        {
          if (CheckCollision(*Player, tile, Direction::UP, velocity))
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
      Player->Rotation = 0;
      for (GameObject &tile : this->Levels[this->Level].Tiles)
      {
        if (tile.IsVisible)
        {
          if (CheckCollision(*Player, tile, Direction::DOWN, velocity))
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
    Blinky->Draw(*Renderer);
    Pinky->Draw(*Renderer);
    Inky->Draw(*Renderer);
    Clyde->Draw(*Renderer);
    Text->RenderText("Score:" + ss.str(), 5.0f, 5.0f, 1.0f);
  } else if (this->State == GAME_WIN) {
    Text->RenderText("You win!", 320.0, Height / 2 - 20.0, 1.0);
    Text->RenderText("Score:" + ss.str(), 325.0, Height / 2, 1.0);
  } else if (this->State == GAME_LOSS) {
    Text->RenderText("You lose!", 320.0, Height / 2 - 20.0, 1.0);
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
  Player = new PacObject(playerPos, playerRadius, playerVelocity, ResourceManager::GetTexture("pacman0"));
  Player->Rotation = -3.14 / 2;
}

void Game::ResetGhosts() {
  glm::vec2 ghostPos = glm::vec2(this->Levels[this->Level].GHOST_POSITION.x, this->Levels[this->Level].GHOST_POSITION.y);
  GLfloat ghostVelocity = this->Levels[this->Level].PLAYER_VELOCITY;
  glm::vec2 ghostSize = this->Levels[this->Level].PLAYER_SIZE;
  Blinky = new GhostObject(ghostPos, ghostSize, ghostVelocity, ResourceManager::GetTexture("ghost"));
  Pinky = new GhostObject(ghostPos, ghostSize, ghostVelocity, ResourceManager::GetTexture("ghost2"));
  Inky = new GhostObject(ghostPos, ghostSize, ghostVelocity, ResourceManager::GetTexture("ghost3"));
  Clyde = new GhostObject(ghostPos, ghostSize, ghostVelocity, ResourceManager::GetTexture("ghost4"));
}

void Game::CheckForDeath() {
  std::vector<GhostObject> ghostVec = {*Blinky, *Pinky, *Inky, *Clyde};
  for (GhostObject ghost : ghostVec) {
    if (CheckCollision(*Player, ghost)) {
      this->State = GAME_LOSS;
    }
  }
}

Direction Game::GenerateRandomDirection() {
    // std::srand(std::time(0)); // use current time as seed for random generator
    float random_variable = std::rand() / double(RAND_MAX);

    if (random_variable < 0.25) {
      return Direction::UP;
    } else if (random_variable < 0.5) {
      return Direction::DOWN;
    } else if (random_variable < 0.75) {
      return Direction::RIGHT;
    } else {
      return Direction::LEFT;
    }
}