//
// Created by vector on 24/03/08.
//

#ifndef DEV_MATERIAL_H
#define DEV_MATERIAL_H

#include <memory>
#include <fbxsdk.h>
#include "Texture.h"
#include "Shader.h"

class Material {
public:
  Material( const char* name, glm::vec3 color = glm::vec3( 1 ) );

  void createFromFbxSurfaceMaterial( FbxSurfaceMaterial* material );

  inline const char* getName() {
    return mName;
  }

  inline std::shared_ptr<Texture>* getTextures() {
    return mTextures;
  }

  inline std::shared_ptr<PhongShader> getShader() {
    return mShader;
  }

protected:
  const char* mName;
  std::shared_ptr<PhongShader> mShader;
  std::shared_ptr<Texture> mTextures[TEXTURETYPE_NUM_ITEMS];
};


#endif // DEV_MATERIAL_H
