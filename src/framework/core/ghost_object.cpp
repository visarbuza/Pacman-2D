#include "ghost_object.h"

GhostObject::GhostObject() 
    : GameObject() { }

GhostObject::GhostObject(glm::vec2 pos, glm::vec2 size, GLfloat velocity, Texture2D sprite)
    :  GameObject(pos, size, sprite, glm::vec3(1.0f), velocity) { }

void GhostObject::Move(Direction dir, GLfloat velocity) {
  switch(dir) {
    case UP: this->Position.y -= velocity;
      break;
    case DOWN: this->Position.y += velocity;
      break;
    case RIGHT: this->Position.x += velocity;
      break;
    case LEFT: this->Position.x -= velocity;
  }
}