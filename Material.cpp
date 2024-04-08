//
// Created by vector on 24/03/08.
//

#include <iostream>

#include "Common.h"
#include "Material.h"
#include "Texture.h"
#include "ResourceManager.h"

Material::Material( const char* name, glm::vec3 color ) {
  mName = name;

  // Assign default shader on creation
  mShader = ResourceManager::getDefaultShader();

  // Initialize textures
  DEBUG("Creating 1x1 texture for material: ", mName);
  std::vector<unsigned char> pixel = {
      (unsigned char) ( color[0] * 255 ),
      (unsigned char) ( color[1] * 255 ),
      (unsigned char) ( color[2] * 255 ),
      (unsigned char) ( 1 * 255 ),
  };

  // Register a new texture
  std::shared_ptr<Texture> newTexture =
      std::make_shared<Texture>( name, pixel.data(), 1, 1, 4 );

  if ( newTexture != nullptr ) {
    // Add to global list
    ResourceManager::addResource( newTexture );
    // Assign to model mesh
    mTextures[DIFFUSE] = newTexture;
  }

  mTextures[NORMAL]  = nullptr;
}

void Material::createFromFbxSurfaceMaterial( FbxSurfaceMaterial* material ) {
  for ( int texTypeInt = DIFFUSE; texTypeInt < TEXTURETYPE_NUM_ITEMS; texTypeInt++ ) {
    static const char* fbxPropType;
    static const char* fbxFactorType;

    switch ( texTypeInt ) {
    case DIFFUSE:
      fbxPropType   = FbxSurfaceMaterial::sDiffuse;
      fbxFactorType = FbxSurfaceMaterial::sDiffuseFactor;
      break;
    case NORMAL:
      fbxPropType   = FbxSurfaceMaterial::sBump;
      fbxFactorType = FbxSurfaceMaterial::sBumpFactor;
      break;
    }

    const FbxProperty prop       = material->FindProperty( fbxPropType );
    const FbxProperty propFactor = material->FindProperty( fbxFactorType );

    FbxDouble3 lResult( 0, 0, 0 );
    FbxFileTexture* fileTexture = nullptr;

    if ( prop.IsValid() ) {
      lResult = prop.Get<FbxDouble3>();

      // Multiply by factor if applicable
      if ( propFactor.IsValid() ) {
        double lFactor = propFactor.Get<FbxDouble>();
        if ( lFactor != 1 ) {
          lResult[0] *= lFactor;
          lResult[1] *= lFactor;
          lResult[2] *= lFactor;
        }
      }

      // Assign texture if available
      const int lFileTextureCount = prop.GetSrcObjectCount<FbxFileTexture>();
      if ( lFileTextureCount ) {
        fileTexture = prop.GetSrcObject<FbxFileTexture>();

        for ( std::shared_ptr<Texture> texture : ResourceManager::getResourceList<Texture>() ) {
          if ( strcmp( fileTexture->GetName(), texture->getName() ) == 0 ) {
            DEBUG("Assigning texture {} to material {}", fileTexture->GetName(), mName);
            mTextures[texTypeInt] = texture;
          }
        }
      }

      // Create a new texture from property if texture is still empty
      if ( !mTextures[texTypeInt] ) {
        std::string newTextureName( mName );
        newTextureName.append( "_" );
        newTextureName.append( prop.GetName() );

        WARN("No texture for material {}, so creating 1x1", mName);

        std::vector<unsigned char> pixel = {
            (unsigned char) ( lResult[0] * 255 ),
            (unsigned char) ( lResult[1] * 255 ),
            (unsigned char) ( lResult[2] * 255 ),
            (unsigned char) ( 1 * 255 ),
        };

        // Register a new texture
        std::shared_ptr<Texture> newTexture =
            std::make_shared<Texture>( newTextureName.c_str(), pixel.data(), 1, 1, 4 );

        if ( newTexture != nullptr ) {
          // Add to global list
          ResourceManager::addResource( newTexture );
          // Assign to model mesh
          mTextures[texTypeInt] = newTexture;
        }
      }
    }
  }
}
