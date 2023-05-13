//
// Created by vector on 23/04/21.
//

// opengl
#include <glad/glad.h>

// stb_image
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Texture.h"

namespace TextureTools {
void LoadOnHost( const char* texturePath, Texture& texture ) {
  // load and generate the texture
  stbi_set_flip_vertically_on_load( true );
  texture._data = stbi_load( texturePath, &texture.Width, &texture.Height, &texture.Channels, 0 );
}

void FreeOnHost( unsigned char* data ) {
  stbi_image_free( data );
}

void GenTextureOnDevice( Texture& texture ) {
  // Create GL texture object
  glGenTextures( 1, &( texture.GLID ) );

  glActiveTexture( GL_TEXTURE0 );
  glBindTexture( GL_TEXTURE_2D, texture.GLID );

  // set the texture wrapping/filtering options (on the currently bound texture object)
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
}

void UpdateTextureData( Texture& texture ) {
  // Bind the texture to default texture unit
  glActiveTexture( GL_TEXTURE0 );
  glBindTexture( GL_TEXTURE_2D, texture.GLID );

  // Copy the data to texture
  glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, texture.Width, texture.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture._data );
  glGenerateMipmap( GL_TEXTURE_2D );
}

void LoadOnDevice( Texture& texture ) {
  GenTextureOnDevice( texture );
  UpdateTextureData( texture );

  // Copying texture from host to device
  if ( texture._data ) {
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGB, texture.Width, texture.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture._data );
    glGenerateMipmap( GL_TEXTURE_2D );
  } else {
    std::cerr << "Could not bind texture!" << std::endl;
  }
}
} // namespace TextureTools
