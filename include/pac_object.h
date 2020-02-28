#ifndef BALLOBJECT_H
#define BALLOBJECT_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "texture.h"
#include "sprite_renderer.h"
#include "game_object.h"

class PacObject : public GameObject
{
public:
    GLfloat   Radius;
    PacObject();
    PacObject(glm::vec2 pos, GLfloat radius, GLfloat velocity, Texture2D sprite);
};

#endif