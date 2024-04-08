#include <iostream>
#include "glm/gtc/type_ptr.hpp"

#include "Common.h"
#include "Shader.h"
#include "ResourceManager.h"

void Shader::loadAndCompile( const char* path, unsigned int shader ) {
  std::string shaderSource;
  ResourceManager::loadFile( path, shaderSource );

  const char* ccShaderSource = shaderSource.c_str();

  glShaderSource( shader, 1, &ccShaderSource, nullptr );
  glCompileShader( shader );

  // check for shader compile errors
  int success;
  char infoLog[512];
  glGetShaderiv( shader, GL_COMPILE_STATUS, &success );
  if ( !success ) {
    glGetShaderInfoLog( shader, 512, nullptr, infoLog );
    throw std::runtime_error( "SHADER COMPILATION FAILED" );
  }
}

PhongShader::PhongShader() {
  // Create, load and compile vertex shader
  unsigned int vertexShader = glCreateShader( GL_VERTEX_SHADER );
  PhongShader::loadAndCompile( "shaders/shader.vert", vertexShader );

  // Create, load and compile fragment shader
  unsigned int fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
  PhongShader::loadAndCompile( "shaders/shader.frag", fragmentShader );

  // Create program
  program = glCreateProgram();

  // Link shaders
  glAttachShader( program, vertexShader );
  glAttachShader( program, fragmentShader );
  glLinkProgram( program );

  // Check linking errors
  int success;
  char infoLog[512];
  glGetProgramiv( program, GL_LINK_STATUS, &success );
  if ( !success ) {
    glGetProgramInfoLog( program, 512, nullptr, infoLog );
    throw std::runtime_error( "SHADER PROGRAM LINKING FAILED" );
  }

  glDeleteShader( vertexShader );
  glDeleteShader( fragmentShader );
}
