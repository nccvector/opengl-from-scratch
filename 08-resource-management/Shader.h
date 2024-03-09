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

  inline bool IsActive(){
    return mActive;
  }

  inline void Activate(){
    glUseProgram( program );
    mActive = true;
  }

  inline void Deactivate(){
    glUseProgram(0);
    mActive = false;
  }

protected:
  void loadAndCompile( const char* path, unsigned int shader );
  unsigned int program;
  bool mActive = false;
};

class PhongShader : public Shader {
public:
  PhongShader();
};

#endif // SHADER_ABSTRACTION_SHADER_H
