//
// Author: Faizan Ali (github.com/nccvector)
// Date: 23/05/04
//

#ifndef OPENGL_FROM_SCRATCH_MATERIAL_H
#define OPENGL_FROM_SCRATCH_MATERIAL_H

#include "Types.h"
#include "Shader.h"
#include "Texture.h"

struct PhongMaterial{
  Color ColorAmbient {0.01f};
  Color ColorDiffuse {0.5f};
  Color ColorSpecular {1.0f};

  const std::vector<Texture>& Textures;
};

#endif // OPENGL_FROM_SCRATCH_MATERIAL_H
