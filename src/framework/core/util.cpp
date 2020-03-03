#include <glad/glad.h>
#include <game_object.h>
#include <pac_object.h>
#include "ghost_object.h"
#include "direction.h"

GLboolean CheckCollision(GhostObject &one, GameObject &two, Direction direction, GLfloat velocity) // AABB - AABB collision
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

GLboolean CheckCollision(PacObject &one, GameObject &two, Direction direction, GLfloat velocity) // AABB - Circle collision
{
    if (direction == UP || direction == LEFT)
      velocity = -velocity;

    // Get center point circle first 
    glm::vec2 center(one.Position + velocity + one.Radius);
    // Calculate AABB info (center, half-extents)
    glm::vec2 aabb_half_extents(two.Size.x / 2, two.Size.y / 2);
    glm::vec2 aabb_center(
        two.Position.x + aabb_half_extents.x, 
        two.Position.y + aabb_half_extents.y
    );
    // Get difference vector between both centers
    glm::vec2 difference = center - aabb_center;
    glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
    // Add clamped value to AABB_center and we get the value of box closest to circle
    glm::vec2 closest = aabb_center + clamped;
    // Retrieve vector between center circle and closest point AABB and check if length <= radius
    difference = closest - center;
    return glm::length(difference) < one.Radius;
}  

GLboolean CheckCollision(PacObject &one, GameObject &two) // AABB - Circle collision
{
    // Get center point circle first 
    glm::vec2 center(one.Position + one.Radius);
    // Calculate AABB info (center, half-extents)
    glm::vec2 aabb_half_extents(two.Size.x / 2, two.Size.y / 2);
    glm::vec2 aabb_center(
        two.Position.x + aabb_half_extents.x, 
        two.Position.y + aabb_half_extents.y
    );
    // Get difference vector between both centers
    glm::vec2 difference = center - aabb_center;
    glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
    // Add clamped value to AABB_center and we get the value of box closest to circle
    glm::vec2 closest = aabb_center + clamped;
    // Retrieve vector between center circle and closest point AABB and check if length <= radius
    difference = closest - center;
    return glm::length(difference) < one.Radius;
}  

float clamp(float value, float min, float max) {
    return glm::max(min, glm::min(max, value));
} 