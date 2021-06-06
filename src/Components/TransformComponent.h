#ifndef TRANSFORMCOMPONTENT_H
#define TRANSFORMCOMPONTENT_H

#include <glm/glm.hpp>
// POD ? in C++ are these really PODS?
struct TransformComponent {
  glm::vec2 position;
  glm::vec2 scale;
  double rotation;
};
#endif