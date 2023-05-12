//
// Author: Faizan Ali (github.com/nccvector)
// Date: 23/05/12
//

#include "PhongShader.h"

void PhongShader::load( const std::string& vertexPath, const std::string& fragmentPath ) {
  mGLVertexShader = createVertexShader( vertexPath );
  validateShader( mGLVertexShader );

  mGLFragmentShader = createFragmentShader( fragmentPath );
  validateShader( mGLFragmentShader );

  mGLProgram = createProgram( mGLVertexShader, mGLFragmentShader );
  validateProgram( mGLProgram );
}

void PhongShader::bindMaterial( const Material& material ) const {
  // Bind Material TODO: check for compatibility with this shader before binding

  // Set colors
  setTextureVec3Float( TextureType::Ambient, material.ColorAmbient );
  setTextureVec3Float( TextureType::Diffuse, material.ColorDiffuse );
  setTextureVec3Float( TextureType::Specular, material.ColorSpecular );

  // Activate texture unit
  Texture textureDiffuse = material.Textures[0];
  glActiveTexture( GL_TEXTURE0 );
  setTextureSampler2D( TextureType::Diffuse, GL_TEXTURE0 );
  glBindTexture( GL_TEXTURE_2D, textureDiffuse.GLID ); // binding texture to appropriate texture unit

  // Reset to GL_TEXTURE0 in case after binding multiple textures
  glActiveTexture( GL_TEXTURE0 );
}
