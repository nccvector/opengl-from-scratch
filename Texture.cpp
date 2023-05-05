//
// Created by vector on 23/04/21.
//

// opengl
#include <glad/glad.h>

// stb_image
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Texture.h"

Texture::Texture( const std::string& texturePath ) {
  // Create GL texture object
  glGenTextures( 1, &mGLTexture );

  glActiveTexture( GL_TEXTURE0 );
  glBindTexture( GL_TEXTURE_2D, mGLTexture );

  // set the texture wrapping/filtering options (on the currently bound texture object)
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

  // load and generate the texture
  stbi_set_flip_vertically_on_load( true );
  mData = stbi_load( texturePath.c_str(), &mWidth, &mHeight, &mNumChannels, 0 );

  // Copying texture from host to device
  if ( mData ) {
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, mData );
    glGenerateMipmap( GL_TEXTURE_2D );
  }
  else
  {
    std::cerr << "Could not bind texture!" << std::endl;
  }

  stbi_image_free( mData );
}

unsigned int Texture::getId() const {
  return mGLTexture;
}
