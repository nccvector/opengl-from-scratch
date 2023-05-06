//
// Created by vector on 23/04/20.
//

#ifndef OPENGL_FROM_SCRATCH_SHADER_H
#define OPENGL_FROM_SCRATCH_SHADER_H

// iostream
#include <iostream>

// GLM
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Model.h"

class Model;

class Shader {
public:
  Shader( const std::string& vertexPath, const std::string& fragmentPath );
  ~Shader();

  [[maybe_unused]] [[nodiscard]] unsigned int getId() const;
  static void use( unsigned int program );
  void use() const;

  void setBool( const std::string& name, bool value ) const;
  void setInt( const std::string& name, int value ) const;
  void setFloat( const std::string& name, float value ) const;

  // Set material props
  void setTexture( const std::string& name, unsigned int textureId ) const;
  void setColor( const std::string& colorName, Color colorValue ) const;

  // Set model props
  void setModelMatrix( const glm::mat4& modelMatrix ) const;

  // Set camera view props
  void setViewMatrix( const glm::mat4& viewMatrix ) const;
  void setProjectionMatrix( const glm::mat4& projectionMatrix ) const;
  void setModelViewProjectionMatrix(
      const glm::mat4& modelMatrix, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix ) const;

  // Draw
  void draw( Model& model ) const;

  // Statics
  // static const Texture loadTexture() {}
  static std::string loadShaderString( const std::string& filepath );
  static unsigned int createVertexShader( const std::string& vertexPath );
  static unsigned int createFragmentShader( const std::string& fragmentPath );
  static unsigned int createProgram( unsigned int vertexShader, unsigned int fragmentShader );
  static void validateShader( unsigned int shader );
  static void validateProgram( unsigned int program );

private:
  unsigned int mGLVertexShader;
  unsigned int mGLFragmentShader;
  unsigned int mGLProgram;
};


#endif // OPENGL_FROM_SCRATCH_SHADER_H
