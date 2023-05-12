//
// Author: Faizan Ali (github.com/nccvector)
// Date: 23/05/12
//

#ifndef OPENGL_FROM_SCRATCH_PHONGSHADER_H
#define OPENGL_FROM_SCRATCH_PHONGSHADER_H

#include "Shader.h"

class PhongShader : public Shader {
public:
  PhongShader() {
    load( "./shaders/PhongVertex.glsl", "./shaders/PhongFragment.glsl" );
  }

  void load( const std::string& vertexPath, const std::string& fragmentPath ) override;
  void bindMaterial( const Material& material ) const override;
};

#endif // OPENGL_FROM_SCRATCH_PHONGSHADER_H
