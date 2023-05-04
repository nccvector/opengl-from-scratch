//
// Created by vector on 23/04/21.
//

// opengl
#include <glad/glad.h>

// stb_image
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Texture.h"

// Static var to keep track of assigned texture units
int Texture::mCurrentTextureUnit     = 0;
const int Texture::MAX_TEXTURE_UNITS = 10;

void Texture::bind() const {
  if (!alreadyBound() && Texture::mCurrentTextureUnit < Texture::MAX_TEXTURE_UNITS)
  {
    glActiveTexture(GL_TEXTURE0 + mTextureUnitId );
    glBindTexture(GL_TEXTURE_2D, mGLTexture);
  }
  else
  {
    std::cerr << "Max texture units exceeded" << std::endl;
  }
}

void Texture::release() const {
  // Activate the texture unit and unbind this texture from that unit
  glActiveTexture(GL_TEXTURE0 + mTextureUnitId );
  glBindTexture(GL_TEXTURE_2D, 0);  // Unbind
}

[[maybe_unused]] Texture::Texture( const std::string& texturePath ) {

  if (!textureUnitsAvailable())
  {
    std::cerr << "No texture units available" << std::endl;
    return;
  }

  // Assign a fixed texture unit id
  mTextureUnitId = Texture::mCurrentTextureUnit; // Starts with zero for the first texture

  // Create GL texture object
  glGenTextures(1, &mGLTexture);
  glActiveTexture(GL_TEXTURE0 + mTextureUnitId);
  glBindTexture(GL_TEXTURE_2D, mGLTexture);

  // set the texture wrapping/filtering options (on the currently bound texture object)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // load and generate the texture
  stbi_set_flip_vertically_on_load(true);
  unsigned char *data = stbi_load(texturePath.c_str(), &mWidth, &mHeight, &mNumChannels, 0);
  if (data)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
    std::cerr << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);

  // unbind
  glBindTexture(GL_TEXTURE_2D, 0);

  // Increment the global texture unit iterator
  Texture::mCurrentTextureUnit += 1;
}
unsigned int Texture::getId() const {
  return mGLTexture;
}
