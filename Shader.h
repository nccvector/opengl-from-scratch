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
#include "Light.h"
#include "Texture.h"
#include "Material.h"

class Model;
struct PointLight;
struct Material;

class Shader {
public:
  ~Shader();

  [[maybe_unused]] [[nodiscard]] unsigned int getId() const;
  static void use( unsigned int program );
  void use() const;

  void setTextureSampler2D( TextureType textureIndex, unsigned int textureId ) const {
    std::string textureUniform = "TextureSampler2DArray[" + std::to_string( textureIndex ) + "]";

    glUniform1i( glGetUniformLocation( mGLProgram, textureUniform.c_str() ), (GLint) textureId );

    // Mark active
    std::string textureActiveStatusUniform = "TextureActiveStatusArray[" + std::to_string( textureIndex ) + "]";
    glUniform1i( glGetUniformLocation( mGLProgram, textureActiveStatusUniform.c_str() ), (int) true );
  }

  void setTextureVec3Float( TextureType textureIndex, Vec3 value ) const {
    std::string vec3Uniform = "TexutreVec3FloatArray[" + std::to_string( textureIndex ) + "]";
    glUniform3fv( glGetUniformLocation( mGLProgram, vec3Uniform.c_str() ), 1, glm::value_ptr( value ) );
  }

  void setTextureFloat( TextureType textureIndex, float value ) const {
    std::string floatUniform = "TextureFloatArray[" + std::to_string( textureIndex ) + "]";
    glUniform1f( glGetUniformLocation( mGLProgram, floatUniform.c_str() ), value );
  }

  void setPointLights( std::vector<PointLight> pointLights ) const {
    // TODO: check MAX_POINT_LIGHTS
    glUniform1i( glGetUniformLocation( mGLProgram, "NumActivePointLights" ), pointLights.size() );

    for ( int i = 0; i < pointLights.size(); i++ ) {
      std::string lightUniform          = "PointLights[" + std::to_string( i ) + "]";
      std::string lightIntensityUniform = lightUniform + ".Intensity";
      std::string lightColorUniform     = lightUniform + ".Color";
      std::string lightPositionUniform  = lightUniform + ".Position";
      glUniform1f( glGetUniformLocation( mGLProgram, lightIntensityUniform.c_str() ), pointLights[i].Intensity );
      glUniform3fv(
          glGetUniformLocation( mGLProgram, lightColorUniform.c_str() ), 1, glm::value_ptr( pointLights[i].Color ) );
      glUniform3fv( glGetUniformLocation( mGLProgram, lightPositionUniform.c_str() ), 1,
          glm::value_ptr( pointLights[i].Position ) );
    }
  }

  // Set model props
  void setModelMatrix( const glm::mat4& modelMatrix ) const;

  // Set camera view props
  void setViewMatrix( const glm::mat4& viewMatrix ) const;
  void setProjectionMatrix( const glm::mat4& projectionMatrix ) const;
  void setModelViewProjectionMatrix(
      const glm::mat4& modelMatrix, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix ) const;

  // Draw
  virtual void load( const std::string& vertexPath, const std::string& fragmentPath ) = 0;
  virtual void bindMaterial( const Material& material ) const                         = 0;
  void draw( const Model& model ) const;

  // Statics
  // static const Texture loadTexture() {}
  static std::string loadShaderString( const std::string& filepath );
  static unsigned int createVertexShader( const std::string& vertexPath );
  static unsigned int createFragmentShader( const std::string& fragmentPath );
  static unsigned int createProgram( unsigned int vertexShader, unsigned int fragmentShader );
  static void validateShader( unsigned int shader );
  static void validateProgram( unsigned int program );

protected:
  unsigned int mGLVertexShader;
  unsigned int mGLFragmentShader;
  unsigned int mGLProgram;
};



#endif // OPENGL_FROM_SCRATCH_SHADER_H
