//
// Created by vector on 24/03/08.
//

#ifndef DEV_TEXTURE_H
#define DEV_TEXTURE_H

// GL
#include <glad/glad.h>

enum TextureType { DIFFUSE = 0, NORMAL, TEXTURETYPE_NUM_ITEMS };

class Texture {
public:
  Texture( const char* name, unsigned char* data, int width, int height, int channels );

  Texture( const char* name, const char* filePath );

  ~Texture();

  void createFromData( const char* name, unsigned char* data, int width, int height, int channels );

  inline void Resize(int width, int height){
    // Delete the current GL texture and create a new handle
    glDeleteTextures(1, &mHandle);

    createFromData( mName, nullptr, width, height, mChannels );
  }

  inline const char* getName() {
    return mName;
  }

  [[nodiscard]] inline int getWidth() const{
    return mWidth;
  }

  [[nodiscard]] inline int getHeight() const {
    return mHeight;
  }

  [[maybe_unused]] [[nodiscard]] inline int getNumChannels() const {
    return mChannels;
  }

  [[nodiscard]] inline unsigned int getHandle() const {
    return mHandle;
  }

  [[nodiscard]] inline bool isCreatedOnDevice() const {
    return mCreatedOnDevice;
  }

protected:
  const char* mName;
  int mWidth;
  int mHeight;
  int mChannels;
  unsigned int mHandle;
  bool mCreatedOnDevice = false;
};


#endif // DEV_TEXTURE_H
