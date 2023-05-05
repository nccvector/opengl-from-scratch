//
// Created by vector on 23/04/21.
//

#ifndef OPENGL_FROM_SCRATCH_TEXTURE_H
#define OPENGL_FROM_SCRATCH_TEXTURE_H

#include <iostream>

class Texture {

public:
  explicit Texture( const std::string& texturePath );
  ~Texture();

  [[nodiscard]] unsigned int getId() const;

private:
  int mWidth {};
  int mHeight {};
  int mNumChannels {};
  unsigned int mGLTexture {};
  unsigned char* mData;
  bool mBound = false;

  static int mCurrentTextureUnit;
  static const int MAX_TEXTURE_UNITS;
};

#endif // OPENGL_FROM_SCRATCH_TEXTURE_H
