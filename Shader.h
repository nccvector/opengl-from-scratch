#ifndef SHADER_ABSTRACTION_SHADER_H
#define SHADER_ABSTRACTION_SHADER_H

// GL
#include "glad/glad.h"

#include "Camera.h"

class Shader {
public:
  Shader()  = default;
  ~Shader() = default;

  void destroy() const {
    glDeleteProgram( program );
  }

  [[nodiscard]] inline unsigned int getProgram() const{
    return program;
  }

  [[nodiscard]] inline bool isActive() const{
    return mActive;
  }

  inline void activate(){
    glUseProgram( program );
    mActive = true;
  }

  inline void deactivate(){
    glUseProgram(0);
    mActive = false;
  }

protected:
  static void loadAndCompile( const char* path, unsigned int shader );
  unsigned int program;
  bool mActive = false;
};

class PhongShader : public Shader {
public:
  PhongShader();
};

#endif // SHADER_ABSTRACTION_SHADER_H
