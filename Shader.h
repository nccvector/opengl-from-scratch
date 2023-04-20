//
// Created by vector on 23/04/20.
//

#ifndef OPENGL_FROM_SCRATCH_SHADER_H
#define OPENGL_FROM_SCRATCH_SHADER_H

// Loading shaders
#include <string>
#include <sstream>
#include <fstream>

class Shader {
public:
  Shader( const char* vertexPath, const char* fragmentPath ) {}

private:
  std::string loadShaderString( const std::string& filepath ) {
    std::ostringstream stringStream;
    std::ifstream inputFileStream( filepath );
    stringStream << inputFileStream.rdbuf();

    std::string shaderString( stringStream.str() );
    return shaderString;
  }

private:
  unsigned int mId;
};


#endif // OPENGL_FROM_SCRATCH_SHADER_H
