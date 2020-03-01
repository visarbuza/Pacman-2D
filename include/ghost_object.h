#ifndef GHOSTOBJECT_H
#define GHOSTOBJECT_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "texture.h"
#include "sprite_renderer.h"
#include "game_object.h"
#include "direction.h"

class GhostObject : public GameObject
{
private:

public:
  GhostObject();
  GhostObject(glm::vec2 pos, glm::vec2 size, GLfloat velocity, Texture2D sprite);
  void Move(Direction dir, GLfloat velocity);
};

#endif