#include "glm/gtc/type_ptr.hpp"
#include "Shader.h"
#include "utils.h"


void Shader::Draw( Camera* camera, Model* model ) {
  glm::mat4 modelViewProjection = camera->GetModelViewMatrix() * model->transform;
  glUseProgram( program );
//  glUniformMatrix4fv(
//      glGetUniformLocation( program, "modelViewProjection" ), 1, GL_FALSE, glm::value_ptr( camera->GetTransform() ) );

  // Draw all the model meshes
  glBindVertexArray( model->VAO );
  glDrawElements( GL_TRIANGLES, model->meshes[0].indices.size(), GL_UNSIGNED_INT, 0 );
}

void Shader::loadAndCompile( const char* path, unsigned int shader ) {
  std::string shaderSource;
  utils::loadFile( path, shaderSource );

  const char* ccShaderSource = shaderSource.c_str();

  glShaderSource( shader, 1, &ccShaderSource, NULL );
  glCompileShader( shader );

  // check for shader compile errors
  int success;
  char infoLog[512];
  glGetShaderiv( shader, GL_COMPILE_STATUS, &success );
  if ( !success ) {
    glGetShaderInfoLog( shader, 512, NULL, infoLog );
    std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
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
    glGetProgramInfoLog( program, 512, NULL, infoLog );
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
  }

  glDeleteShader( vertexShader );
  glDeleteShader( fragmentShader );
}
