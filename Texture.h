//
// Created by vector on 23/04/21.
//

#ifndef OPENGL_FROM_SCRATCH_TEXTURE_H
#define OPENGL_FROM_SCRATCH_TEXTURE_H

#include <iostream>

class Texture {

public:
  [[maybe_unused]] explicit Texture( const std::string& texturePath );

  [[nodiscard]] unsigned int getId() const;

  void bind() const;
  void release() const;

  inline bool alreadyBound() const {
    // if -1 then not bound
    // else already bound
    return mTextureUnitId != -1;
  }

  inline static bool textureUnitsAvailable() {
    return Texture::mCurrentTextureUnit < ( Texture::MAX_TEXTURE_UNITS - 1 );
  }

  inline static void resetTextureUnits()
  {
    // It is material's responsibility to reset texture units when released
    Texture::mCurrentTextureUnit = 0;
  }

private:
  int mTextureUnitId = -1;
  int mWidth {};
  int mHeight {};
  int mNumChannels {};
  unsigned int mGLTexture {};

  static int mCurrentTextureUnit;
  static const int MAX_TEXTURE_UNITS;
};

#endif // OPENGL_FROM_SCRATCH_TEXTURE_H
