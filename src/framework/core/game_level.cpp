#include <game_level.h>

#include <fstream>
#include <sstream>
#include <iostream>

void GameLevel::Load(const GLchar *file, GLuint levelWidth, GLuint levelHeight)
{
  // Clear old data
  this->Tiles.clear();
  // Load from file
  GLuint tileCode;
  GameLevel level;
  std::string line;
  std::fstream fstream;
  fstream.open(file);
  std::vector<std::vector<GLuint>> tileData;
  if (fstream)
  {
    // Skip first line
    std::getline(fstream, line);
    while (std::getline(fstream, line)) // Read each line from level file
    {
      std::istringstream sstream(line);
      std::vector<GLuint> row;
      while (sstream >> tileCode) // Read each word seperated by spaces
        row.push_back(tileCode);
      tileData.push_back(row);
    }
    if (tileData.size() > 0)
      this->init(tileData, levelWidth, levelHeight);
  }
}

void GameLevel::Draw(SpriteRenderer &renderer)
{
  for (GameObject &tile : this->Tiles)
    if (tile.IsVisible)
      tile.Draw(renderer);
}

GLboolean GameLevel::IsCompleted()
{
  for (GameObject &tile : this->Tiles)
    if (!tile.IsSolid && tile.IsVisible)
      return GL_FALSE;
  return GL_TRUE;
}

void GameLevel::init(std::vector<std::vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight)
{
  // Calculate dimensions
  GLuint height = tileData.size();
  GLuint width = tileData[0].size(); // Note we can index vector at [0] since this function is only called if height > 0
  GLfloat unit_width = levelWidth / static_cast<GLfloat>(width), unit_height = levelHeight / height;
  this->PLAYER_SIZE = glm::vec2(unit_width, unit_height);
  this->PLAYER_VELOCITY = (unit_width + unit_height) * 2;
  this->PLAYER_RADIUS = glm::min(unit_height, unit_width) / 2.3;
  // Initialize level tiles based on tileData
  for (GLuint y = 0; y < height; ++y)
  {
    for (GLuint x = 0; x < width; ++x)
    {
      // Check block type from level data
      if (tileData[y][x] == 1) // Wall
      {
        glm::vec2 pos(unit_width * x, unit_height * y);
        glm::vec2 size(unit_width, unit_height);
        GameObject obj(pos, size, ResourceManager::GetTexture("wall"));
        obj.IsSolid = GL_TRUE;
        this->Tiles.push_back(obj);
      }
      else if (tileData[y][x] == 0) // Food
      {
        glm::vec2 pos(unit_width * x, unit_height * y);
        glm::vec2 size(unit_width, unit_height);
        GameObject obj(pos, size, ResourceManager::GetTexture("food"));
        obj.IsSolid = GL_FALSE;
        this->Tiles.push_back(obj);
      }
      else if (tileData[y][x] == 2) // Pacman initial position
      {
        this->PLAYER_POSITION = glm::vec2(unit_width * x, unit_height * y);
      }
    }
  }
}