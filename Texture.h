//
// Created by vector on 23/04/21.
//

#ifndef OPENGL_FROM_SCRATCH_TEXTURE_H
#define OPENGL_FROM_SCRATCH_TEXTURE_H

#include <iostream>

enum TextureType { Ambient = 0, Diffuse, Specular };

struct Texture {
  const char* Name = "My Texture";
  TextureType Type = TextureType::Diffuse;
  int Width {};
  int Height {};
  int Channels {};
  unsigned int GLID {};
  unsigned char* _data = NULL;  // data NULL by default
};

namespace TextureTools {
void LoadOnHost( const char* texturePath, Texture& texture );
void FreeOnHost( unsigned char* data );
void GenTextureOnDevice( Texture& texture );
void UpdateTextureData( Texture& texture );
void LoadOnDevice( Texture& texture );
} // namespace TextureTools
#endif // OPENGL_FROM_SCRATCH_TEXTURE_H
