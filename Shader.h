//
// Created by vector on 23/04/20.
//

#ifndef OPENGL_FROM_SCRATCH_SHADER_H
#define OPENGL_FROM_SCRATCH_SHADER_H

// iostream
#include <iostream>

// Loading shaders
#include <string>
#include <sstream>
#include <fstream>

// opengl
#include <glad/glad.h>

// GLM
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:
  Shader( const std::string& vertexPath, const std::string& fragmentPath ) {
    mGLVertexShader = createVertexShader( vertexPath );
    validateShader( mGLVertexShader );

    mGLFragmentShader = createFragmentShader( fragmentPath );
    validateShader( mGLFragmentShader );

    mGLProgram = createProgram( mGLVertexShader, mGLFragmentShader );
    validateProgram( mGLProgram );
  }

  ~Shader() {
    glDeleteShader( mGLVertexShader );
    glDeleteShader( mGLFragmentShader );
    glDeleteProgram( mGLProgram );
  }

  const unsigned int getId() {
    return mGLProgram;
  }

  static void use( const unsigned int program ) {
    glUseProgram( program );
  }

  void use() {
    use( mGLProgram );
  }

  void setBool( const std::string& name, bool value ) {
    glUniform1i( glGetUniformLocation( mGLProgram, name.c_str() ), (int) value );
  }

  void setInt( const std::string& name, int value ) {
    glUniform1i( glGetUniformLocation( mGLProgram, name.c_str() ), value );
  }
  void setFloat( const std::string& name, float value ) {
    glUniform1f( glGetUniformLocation( mGLProgram, name.c_str() ), value );
  }

  void setTexture( const std::string& name, int value ) {
    glUniform1i( glGetUniformLocation( mGLProgram, name.c_str() ), value );
  }

  void setModelMatrix( const glm::mat4& modelMatrix ) {
    glUniformMatrix4fv( glGetUniformLocation( mGLProgram, "Model" ), 1, GL_FALSE, glm::value_ptr( modelMatrix ) );
  }

  void setViewMatrix( const glm::mat4& viewMatrix ) {
    glUniformMatrix4fv( glGetUniformLocation( mGLProgram, "View" ), 1, GL_FALSE, glm::value_ptr( viewMatrix ) );
  }

  void setProjectionMatrix( const glm::mat4& projectionMatrix ) {
    glUniformMatrix4fv(
        glGetUniformLocation( mGLProgram, "Projection" ), 1, GL_FALSE, glm::value_ptr( projectionMatrix ) );
  }

  void setModelViewProjectionMatrix(
      const glm::mat4& modelMatrix, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix ) {
    setModelMatrix( modelMatrix );
    setViewMatrix( viewMatrix );
    setProjectionMatrix( projectionMatrix );
  }

  // Statics
  // static const Texture loadTexture() {}

  static std::string loadShaderString( const std::string& filepath ) {
    std::ostringstream stringStream;
    std::ifstream inputFileStream( filepath );
    stringStream << inputFileStream.rdbuf();

    std::string shaderString( stringStream.str() );
    return shaderString;
  }

  static const unsigned int createVertexShader( const std::string& vertexPath ) {
    const std::string& vertexShaderString = loadShaderString( vertexPath );
    const char* vertexShaderSource        = vertexShaderString.c_str();

    unsigned int vertexShader = glCreateShader( GL_VERTEX_SHADER );
    glShaderSource( vertexShader, 1, &vertexShaderSource, nullptr );
    glCompileShader( vertexShader );
    return vertexShader;
  }

  static const unsigned int createFragmentShader( const std::string& fragmentPath ) {
    const std::string& fragmentShaderString = loadShaderString( fragmentPath );
    const char* fragmentShaderSource        = fragmentShaderString.c_str();

    unsigned int fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
    glShaderSource( fragmentShader, 1, &fragmentShaderSource, nullptr );
    glCompileShader( fragmentShader );
    return fragmentShader;
  }

  static const unsigned int createProgram( const unsigned int vertexShader, const unsigned int fragmentShader ) {
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

  static void validateShader( unsigned int shader ) {
    int success;
    char infoLog[512];
    glGetShaderiv( shader, GL_COMPILE_STATUS, &success );
    if ( !success ) {
      glGetShaderInfoLog( shader, 512, nullptr, infoLog );
      std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
  }

  static void validateProgram( const unsigned int program ) {
    int success;
    char infoLog[512];
    glGetProgramiv( program, GL_LINK_STATUS, &success );
    if ( !success ) {
      glGetProgramInfoLog( program, 512, nullptr, infoLog );
      std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
  }

private:
  unsigned int mGLVertexShader;
  unsigned int mGLFragmentShader;
  unsigned int mGLProgram;
};


#endif // OPENGL_FROM_SCRATCH_SHADER_H
