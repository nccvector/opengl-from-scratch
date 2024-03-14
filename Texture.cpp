//
// Created by vector on 24/03/08.
//

#include <iostream>

// GL
#include "glad/glad.h"

#include "Texture.h"

// Image read write headers
#include "stb_image.h"
#include "stb_image_write.h"

Texture::Texture( const char* name, unsigned char* data, int width, int height, int channels ) {
  CreateFromData( name, data, width, height, channels );
}

Texture::Texture( const char* name, const char* filePath ) {
  int width, height, channels;
  unsigned char* data = stbi_load( filePath, &( width ), &( height ), &( channels ), 4 );

  if ( !data ) {
    std::cerr << "Failed to load texture from disk: " << filePath << "\n";
    return;
  }

  std::cout << "Successfully loaded texture from disk: " << filePath << "\n";

  CreateFromData( name, data, width, height, channels );

  // Free image data from host
  stbi_image_free( data );
}

void Texture::CreateFromData( const char* name, unsigned char* data, int width, int height, int channels ) {
#ifdef DEBUG_DUMP_TEXTURES
  // Write this texture to disk
  std::string buf( name );
  buf.append( ".png" );
  std::cout << "Writing texture: " << buf << "\n";
  stbi_write_png( buf.c_str(), width, height, channels, data, channels * sizeof( unsigned char ) * width );
#endif

  mName     = name;
  mWidth    = width;
  mHeight   = height;
  mChannels = channels;

  // Texture settings
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

  // Texture creation
  glGenTextures( 1, &( mHandle ) );
  glBindTexture( GL_TEXTURE_2D, mHandle );
  glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data );
  glGenerateMipmap( GL_TEXTURE_2D );

  mCreatedOnDevice = true;
}
