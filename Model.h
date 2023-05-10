//
// Author: Faizan Ali (github.com/nccvector)
// Date: 23/05/02
//

#ifndef OPENGL_FROM_SCRATCH_MODEL_H
#define OPENGL_FROM_SCRATCH_MODEL_H

#include <vector>

#include <glad/glad.h>

#include "Types.h"
#include "Material.h"

class Material;

struct Model {
  VertexList Vertices;
  UIntList Indices;
  glm::mat4 Transform;
  const Material& crMaterial;
  unsigned int _VBO {};
  unsigned int _VAO {};
  unsigned int _EBO {};
};

namespace ModelTools {
void LoadOnDevice( Model& model );
}

#endif // OPENGL_FROM_SCRATCH_MODEL_H
