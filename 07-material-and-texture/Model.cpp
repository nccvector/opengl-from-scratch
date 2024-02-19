//
// Created by vector on 24/02/10.
//

#include <iostream>
#include "Model.h"
#include "stb_image.h"

// #define DEBUG_DUMP_TEXTURES

bool ModelTools::CreateTextureData(
    unsigned char* data, Texture* texture, const char* name, int width, int height, int channels ) {

#ifdef DEBUG_DUMP_TEXTURES
  // Write this texture to disk
  std::string buf( name );
  buf.append( ".png" );
  std::cout << "Writing texture: " << buf << "\n";
  stbi_write_png( buf.c_str(), width, height, channels, data, channels * sizeof( unsigned char ) * width );
#endif

  texture->name     = name;
  texture->width    = width;
  texture->height   = height;
  texture->channels = channels;

  // Texture settings
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

  // Texture creation
  glGenTextures( 1, &( texture->handle ) );
  glBindTexture( GL_TEXTURE_2D, texture->handle );
  glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, texture->width, texture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data );
  glGenerateMipmap( GL_TEXTURE_2D );

  return true;
}

bool ModelTools::CreateTextureFromPath( const char* filePath, Texture* texture, const char* name ) {
  int width, height, channels;
  unsigned char* data = stbi_load( filePath, &( width ), &( height ), &( channels ), 4 );

  if ( !data ) {
    std::cerr << "Failed to load texture from disk: " << filePath << "\n";
    return false;
  }

  std::cout << "Successfully loaded texture from disk: " << filePath << "\n";

  bool sucess = ModelTools::CreateTextureData( data, texture, name, width, height, channels );

  // Free image data from host
  stbi_image_free( data );

  return sucess;
}

void ModelTools::CreateModelFromFbxNode( FbxNode* node, Model& model ) {
  model.name    = node->GetName();
  FbxMesh* mesh = node->GetMesh();

  // No need to proceed any further if no mesh
  if ( !mesh ) {
    return;
  }

  // Inialize model mesh
  model.meshes.resize( 1 );

  // Display attributes summary
  std::cout << "Num materials: " << node->GetMaterialCount() << "\n";
  node->GetMaterialCount();
  FbxSurfaceMaterial* material = node->GetMaterial( 0 ); // 0 is the submesh index
  material->GetName();
  std::cout << "Material name: " << material->GetName() << "\n";

  // ================================================================================
  // Apply material
  // ================================================================================

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

    std::cout << "CHECKING TEX TYPE: " << fbxPropType << "\n";

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
      std::cout << "Texture count: " << lFileTextureCount << "\n";
      if ( lFileTextureCount ) {
        fileTexture = prop.GetSrcObject<FbxFileTexture>();

        for ( Texture* texture : global::textures ) {
          if ( strcmp( fileTexture->GetName(), texture->name ) == 0 ) {
            std::cout << "Assiging texture `" << fileTexture->GetName() << "` to model `" << model.name << "`\n";
            model.meshes[0].material.textures[texTypeInt] = texture;
          }
        }
      }

      // Create a new texture from property if texture is still empty
      if ( !model.meshes[0].material.textures[texTypeInt] ) {
        std::string newTextureName( model.name );
        newTextureName.append( "_" );
        newTextureName.append( prop.GetName() );

        std::cout << "No texture, creating 1x1 texture for model: " << model.name << "\n";

        std::vector<unsigned char> pixel = {
            (unsigned char) ( lResult[0] * 255 ),
            (unsigned char) ( lResult[1] * 255 ),
            (unsigned char) ( lResult[2] * 255 ),
            (unsigned char) ( 1 * 255 ),
        };

        Texture* newTexture = new Texture;

        if ( ModelTools::CreateTextureData( pixel.data(), newTexture, newTextureName.c_str(), 1, 1, 4 ) ) {
          // Add to global textures
          global::textures.push_back( newTexture );

          // Assign to model mesh
          model.meshes[0].material.textures[texTypeInt] = global::textures[global::textures.size() - 1];
        }
      }
    }
  }
  // ================================================================================


  // ================================================================================
  // Load mesh
  // ================================================================================

  // Declare memory for mesh
  std::vector<Vertex> vertices;

  int vertexCount             = mesh->GetControlPointsCount();
  int indexCount              = mesh->GetPolygonVertexCount();
  model.meshes[0].numVertices = indexCount;

  // Copy indices to model
  int* indices = mesh->GetPolygonVertices();

  // Resize vertices to be of the size of elements
  vertices.resize( indexCount ); // Declare memory

  // Copy vertices to model
  FbxVector4* pVertices = mesh->GetControlPoints();
  vertices.resize( indexCount ); // Declare memory

  // Copy normals to model
  FbxArray<FbxVector4> pNormals;
  mesh->GenerateNormals();
  mesh->GetPolygonVertexNormals( pNormals );

  // Copy texCoords to model
  FbxStringList names;
  mesh->GetUVSetNames( names );
  FbxArray<FbxVector2> pTexCoords;
  mesh->GetPolygonVertexUVs( names[0], pTexCoords ); // first name is "default"

  for ( int i = 0; i < indexCount; i++ ) {
    int vertexIndex    = indices[i];
    FbxVector4 pVertex = pVertices[vertexIndex];

    FbxVector4 pNormal;
    if ( pNormals.Size() > 0 ) {
      pNormal = pNormals[i];
    } else {
      pNormal = { 0, 0, 0, 0 };
    }

    FbxVector2 pTexCoord;
    if ( pTexCoords.Size() > 0 ) {
      pTexCoord = pTexCoords[i];
    } else {
      pTexCoord = { 0, 0 };
    }

    // Add position
    vertices[i].position = {
        static_cast<float>( pVertex[0] ), static_cast<float>( pVertex[1] ), static_cast<float>( pVertex[2] ) };

    // Add normal
    vertices[i].normal = {
        static_cast<float>( pNormal[0] ), static_cast<float>( pNormal[1] ), static_cast<float>( pNormal[2] ) };

    vertices[i].texCoord = { static_cast<float>( pTexCoord[0] ), static_cast<float>( pTexCoord[1] ) };

    //    std::cout << "Vertex: " << model.meshes[0].vertices[i][0] << ", " << model.meshes[0].vertices[i][1] << ", "
    //              << model.meshes[0].vertices[i][2] << "\n";
  }

  std::cout << "Loaded vertex count: " << vertices.size() << "\n";

  // Create mesh object on device
  LoadMeshOnDevice( model.meshes[0], vertices );

  // ================================================================================
}

void ModelTools::LoadMeshOnDevice( Mesh& mesh, std::vector<Vertex> vertices ) {
  glGenVertexArrays( 1, &( mesh.VAO ) );
  glGenBuffers( 1, &( mesh.VBO ) );

  // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
  glBindVertexArray( mesh.VAO );

  glBindBuffer( GL_ARRAY_BUFFER, mesh.VBO );
  glBufferData( GL_ARRAY_BUFFER, sizeof( Vertex ) * vertices.size(), vertices.data(), GL_STATIC_DRAW );

  // Index of different Vertex attributes
  const unsigned int positionAttribIndex = 0;
  const unsigned int normalAttribIndex   = 1;
  const unsigned int texCoordAttribIndex = 2;

  // Position
  glEnableVertexAttribArray( positionAttribIndex );
  glVertexAttribPointer( positionAttribIndex, // shader location index
      3,                                      // number of components per attribute
      GL_FLOAT,
      GL_FALSE,         // normalized (clamp between -1 to 1)
      sizeof( Vertex ), // stride in bytes to the next same vertex attribute
      (void*) 0         // byte size offset for the first component
  );

  // Normal
  glEnableVertexAttribArray( normalAttribIndex );
  glVertexAttribPointer( normalAttribIndex, // shader location index
      3,                                    // number of components per attribute
      GL_FLOAT,
      GL_FALSE,                          // normalized (clamp between -1 to 1)
      sizeof( Vertex ),                  // stride in bytes to the next same vertex attribute
      (void*) offsetof( Vertex, normal ) // byte size offset for the first component
  );

  // TexCoord
  glEnableVertexAttribArray( texCoordAttribIndex );
  glVertexAttribPointer( texCoordAttribIndex, // shader location index
      3,                                      // number of components per attribute
      GL_FLOAT,
      GL_FALSE,                            // normalized (clamp between -1 to 1)
      sizeof( Vertex ),                    // stride in bytes to the next same vertex attribute
      (void*) offsetof( Vertex, texCoord ) // byte size offset for the first component
  );

  // Release buffer
  glBindBuffer( GL_ARRAY_BUFFER, 0 );

  // Release buffer array
  glBindVertexArray( 0 );
}
