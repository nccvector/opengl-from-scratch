//
// Created by vector on 23/04/20.
//

// Loading shaders
#include <string>
#include <sstream>
#include <fstream>

// opengl
#include <glad/glad.h>

#include "Shader.h"


Shader::~Shader() {
  glDeleteShader( mGLVertexShader );
  glDeleteShader( mGLFragmentShader );
  glDeleteProgram( mGLProgram );
}

unsigned int Shader::getId() const {
  return mGLProgram;
}

void Shader::use( const unsigned int program ) {
  glUseProgram( program );
}

void Shader::setModelMatrix( const glm::mat4& modelMatrix ) const {
  glUniformMatrix4fv( glGetUniformLocation( mGLProgram, "Model" ), 1, GL_FALSE, glm::value_ptr( modelMatrix ) );
}

void Shader::setViewMatrix( const glm::mat4& viewMatrix ) const {
  glUniformMatrix4fv( glGetUniformLocation( mGLProgram, "View" ), 1, GL_FALSE, glm::value_ptr( viewMatrix ) );
}

void Shader::setProjectionMatrix( const glm::mat4& projectionMatrix ) const {
  glUniformMatrix4fv(
      glGetUniformLocation( mGLProgram, "Projection" ), 1, GL_FALSE, glm::value_ptr( projectionMatrix ) );
}

void Shader::setModelViewProjectionMatrix(
    const glm::mat4& modelMatrix, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix ) const {
  setModelMatrix( modelMatrix );
  setViewMatrix( viewMatrix );
  setProjectionMatrix( projectionMatrix );
}

std::string Shader::loadShaderString( const std::string& filepath ) {
  std::ostringstream stringStream;
  std::ifstream inputFileStream( filepath );
  stringStream << inputFileStream.rdbuf();

  std::string shaderString( stringStream.str() );
  return shaderString;
}

unsigned int Shader::createVertexShader( const std::string& vertexPath ) {
  const std::string& vertexShaderString = loadShaderString( vertexPath );
  const char* vertexShaderSource        = vertexShaderString.c_str();

  unsigned int vertexShader = glCreateShader( GL_VERTEX_SHADER );
  glShaderSource( vertexShader, 1, &vertexShaderSource, nullptr );
  glCompileShader( vertexShader );
  return vertexShader;
}

unsigned int Shader::createFragmentShader( const std::string& fragmentPath ) {
  const std::string& fragmentShaderString = loadShaderString( fragmentPath );
  const char* fragmentShaderSource        = fragmentShaderString.c_str();

  unsigned int fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
  glShaderSource( fragmentShader, 1, &fragmentShaderSource, nullptr );
  glCompileShader( fragmentShader );
  return fragmentShader;
}

unsigned int Shader::createProgram( const unsigned int vertexShader, const unsigned int fragmentShader ) {
  // Shader program:
  //    A shader program object is the final linked version of multiple shaders combined. To
  //    use the recently compiled shaders we have to link them to a shader program object and then activate this
  //    shader program when rendering objects. The activated shader program's shaders will be used when we issue
  //    render calls. When linking the shaders into a program it links the outputs of each shader to the inputs of the
  //    next shader. This is also where you'll get linking errors if your outputs and inputs do not match.
  unsigned int program = glCreateProgram();
  glAttachShader( program, vertexShader );
  glAttachShader( program, fragmentShader );
  glLinkProgram( program );
  return program;
}

void Shader::validateShader( unsigned int shader ) {
  int success;
  char infoLog[512];
  glGetShaderiv( shader, GL_COMPILE_STATUS, &success );
  if ( !success ) {
    glGetShaderInfoLog( shader, 512, nullptr, infoLog );
    std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
  }
}

void Shader::validateProgram( const unsigned int program ) {
  int success;
  char infoLog[512];
  glGetProgramiv( program, GL_LINK_STATUS, &success );
  if ( !success ) {
    glGetProgramInfoLog( program, 512, nullptr, infoLog );
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
  }
}

void Shader::use() const {
  use( mGLProgram );
}

void Shader::bindMaterial( const Material& material ) const {
  std::cerr << "bindMaterial invoked from base class" << std::endl;
}

void Shader::draw( const Model& model ) const {
  // Set all the shader attributes
  setModelMatrix( model.Transform );

  bindMaterial( model.crMaterial );

  // Draw model
  glBindVertexArray( model._VAO );
  glDrawElements( GL_TRIANGLES, model.Indices.size(), GL_UNSIGNED_INT, 0 );
  glBindVertexArray( 0 ); // unbind
}
