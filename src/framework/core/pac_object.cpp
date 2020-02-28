#include "pac_object.h"

PacObject::PacObject() 
    : GameObject(), Radius(12.5f) { }

PacObject::PacObject(glm::vec2 pos, GLfloat radius, GLfloat velocity, Texture2D sprite)
    :  GameObject(pos, glm::vec2(radius * 2, radius * 2), sprite, glm::vec3(1.0f), velocity), Radius(radius) { }