#ifndef GAMELEVEL_H
#define GAMELEVEL_H
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "game_object.h"
#include "sprite_renderer.h"
#include "resource_manager.h"


/// GameLevel holds all Tiles as part of a Breakout level and 
/// hosts functionality to Load/render levels from the harddisk.
class GameLevel
{
public:
    // Initial player position
    glm::vec2 PLAYER_POSITION;
    // Player size relative to level
    glm::vec2 PLAYER_SIZE;
    // Player velocity relative to level
    GLfloat PLAYER_VELOCITY;
    // Player radius
    GLfloat PLAYER_RADIUS;
    // Initial ghost position
    glm::vec2 GHOST_POSITION;
    // Level state
    std::vector<GameObject> Tiles;
    // Constructor
    GameLevel() { }
    // Loads level from file
    void      Load(const GLchar* file, GLuint levelWidth, GLuint levelHeight);
    // Render level
    void      Draw(SpriteRenderer& renderer);
    // Check if the level is completed (all non-solid tiles are destroyed)
    GLboolean IsCompleted();
private:
    // Initialize level from tile data
    void      init(std::vector<std::vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight);
};

#endif