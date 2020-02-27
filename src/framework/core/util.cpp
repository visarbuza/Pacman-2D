#include <glad/glad.h>
#include <game_object.h>

enum Direction
{
  UP,
  DOWN,
  LEFT,
  RIGHT
};

GLboolean CheckCollision(GameObject &one, GameObject &two, Direction direction, GLfloat velocity) // AABB - AABB collision
{
  if (direction == DOWN || direction == LEFT)
    velocity = -velocity;

  // Collision x-axis?
  bool collisionX = one.Position.x + one.Size.x + velocity >= two.Position.x && two.Position.x + two.Size.x >= one.Position.x + velocity;
  // Collision y-axis?
  bool collisionY = one.Position.y + one.Size.y - velocity >= two.Position.y && two.Position.y + two.Size.y >= one.Position.y - velocity;
  // Collision only if on both axes
  return collisionX && collisionY;
}