//
// Created by vector on 23/04/24.
//

#ifndef OPENGL_FROM_SCRATCH_NODE_H
#define OPENGL_FROM_SCRATCH_NODE_H

// GLM
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Node {
public:
  Node(const char* name = "Node");
  ~Node();

  glm::mat4& getTransform();

private:
  const char* mName;
  unsigned int mId;
  glm::mat4 mTransform;
  static unsigned int mIdCounter;
};

#endif // OPENGL_FROM_SCRATCH_NODE_H
