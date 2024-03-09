#ifndef SHADER_ABSTRACTION_SHADER_H
#define SHADER_ABSTRACTION_SHADER_H

// GL
#include "glad/glad.h"

#include "Camera.h"

class Shader {
public:
  Shader()  = default;
  ~Shader() = default;

  void destroy() {
    glDeleteProgram( program );
  }

  inline unsigned int GetProgram(){
    return program;
  }

protected:
  void loadAndCompile( const char* path, unsigned int shader );
  unsigned int program;
};

class PhongShader : public Shader {
public:
  PhongShader();
};

#endif // SHADER_ABSTRACTION_SHADER_H
