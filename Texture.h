//
// Created by vector on 23/04/21.
//

#ifndef OPENGL_FROM_SCRATCH_TEXTURE_H
#define OPENGL_FROM_SCRATCH_TEXTURE_H

#include <iostream>

class Texture {

public:
  [[maybe_unused]] explicit Texture(const std::string& texturePath);

  [[nodiscard]] unsigned int getId() const;

private:
  int mWidth{};
  int mHeight{};
  int mNumChannels{};
  unsigned int mGLTexture{};
};


#endif // OPENGL_FROM_SCRATCH_TEXTURE_H
