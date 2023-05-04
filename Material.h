//
// Author: Faizan Ali (github.com/nccvector)
// Date: 23/05/04
//

#ifndef OPENGL_FROM_SCRATCH_MATERIAL_H
#define OPENGL_FROM_SCRATCH_MATERIAL_H

#include "Types.h"
#include "Shader.h"
#include "Texture.h"

class Shader;

class PhongMaterial {
public:
  explicit PhongMaterial( Color colorAmbient = Color( 1.0f ), Color colorDiffuse = Color( 0.5f ),
      Color colorSpecular = Color( 1.0f ), Texture* textureAmbient = nullptr, Texture* textureDiffuse = nullptr,
      Texture* textureSpecular = nullptr, Shader* shader = nullptr )
      : mColorAmbient( colorAmbient ), mColorDiffuse( colorDiffuse ), mColorSpecular( colorSpecular ),
        mTextureAmbient( textureAmbient ), mTextureDiffuse( textureDiffuse ), mTextureSpecular( textureSpecular ),
        mShader( shader ) {};

  inline Color getColorAmbient() {
    return mColorAmbient;
  }

  inline Color getColorDiffuse() {
    return mColorAmbient;
  }

  inline Color getColorSpecular() {
    return mColorAmbient;
  }

  inline Texture* getTextureAmbient() {
    return mTextureAmbient;
  }

  inline Texture* getTextureDiffuse() {
    return mTextureDiffuse;
  }

  inline Texture* getTextureSpecular() {
    return mTextureSpecular;
  }

  inline void bind()
  {
    Texture::resetTextureUnits();
    
    if (mTextureAmbient)
      mTextureAmbient->bind();
    if (mTextureDiffuse)
      mTextureDiffuse->bind();
    if (mTextureSpecular)
      mTextureSpecular->bind();
  }

  inline void release()
  {
    if (mTextureAmbient)
      mTextureAmbient->release();
    if (mTextureDiffuse)
      mTextureDiffuse->release();
    if (mTextureSpecular)
      mTextureSpecular->release();

    Texture::resetTextureUnits();
  }

private:
  Color mColorAmbient;
  Color mColorDiffuse;
  Color mColorSpecular;

  // TODO: Use smart pointers here...
  Texture* mTextureAmbient;
  Texture* mTextureDiffuse;
  Texture* mTextureSpecular;

  Shader* mShader;
};


#endif // OPENGL_FROM_SCRATCH_MATERIAL_H
