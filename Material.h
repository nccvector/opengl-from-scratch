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
  PhongMaterial( const std::vector<Texture>& textures, Color colorAmbient = Color( 1.0f ),
      Color colorDiffuse = Color( 0.5f ), Color colorSpecular = Color( 1.0f ) )
      : mTextures( textures ), mColorAmbient( colorAmbient ), mColorDiffuse( colorDiffuse ),
        mColorSpecular( colorSpecular ) {};

  inline const std::vector<Texture> getTextures() const {
    return mTextures;
  }

private:
  Color mColorAmbient;
  Color mColorDiffuse;
  Color mColorSpecular;

  std::vector<Texture> mTextures;
};


#endif // OPENGL_FROM_SCRATCH_MATERIAL_H
