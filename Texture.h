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

  void CreateFromData( const char* name, unsigned char* data, int width, int height, int channels );

  inline void Resize(int width, int height){
    // Detete the current GL texture and create a new handle
    glDeleteTextures(1, &mHandle);

    CreateFromData(mName, nullptr, width, height, mChannels);
  }

  inline const char* GetName() {
    return mName;
  }

  inline const int GetWidth(){
    return mWidth;
  }

  inline const int GetHeight() {
    return mHeight;
  }

  inline const int GetNumChannels() {
    return mChannels;
  }

  inline const unsigned int GetHandle() {
    return mHandle;
  }

  inline bool IsCreatedOnDevice() {
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
