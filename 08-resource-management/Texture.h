//
// Created by vector on 24/03/08.
//

#ifndef DEV_TEXTURE_H
#define DEV_TEXTURE_H

enum TextureType { DIFFUSE = 0, NORMAL, TEXTURETYPE_NUM_ITEMS };

class Texture {
public:
  Texture( const char* name, unsigned char* data, int width, int height, int channels );

  Texture( const char* name, const char* filePath );

  void CreateFromData( const char* name, unsigned char* data, int width, int height, int channels );

  inline const char* GetName() {
    return mName;
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
