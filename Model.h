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
void LoadModelsFromFile(
    const std::string& filename, std::vector<Model>& models, std::vector<Material>& materials, float scale = 1.0f );
} // namespace ModelTools

#endif // OPENGL_FROM_SCRATCH_MODEL_H
