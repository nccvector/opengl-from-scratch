//
// Created by vector on 24/03/08.
//

#ifndef DEV_MESH_H
#define DEV_MESH_H

#include <memory>
#include <vector>

#include "glad/glad.h"

#include "Types.h"
#include "Material.h"

class Mesh {
public:
  Mesh( const char* name, std::shared_ptr<Material> material );

  void CreateFromFbxMesh( FbxMesh* mesh );
  std::vector<Vertex> GetVerticesFromFbxMesh( FbxMesh* mesh );

  void CreateOnHost( const std::vector<Vertex>& vertices );
  void CreateOnDevice();

  inline const char* GetName() {
    return mName;
  }

  inline void Draw() {
    // Bind texture of this mesh
    if ( mMaterial->GetTextures()[DIFFUSE]->GetHandle() ) {
      glActiveTexture( GL_TEXTURE0 );
      glUniform1i( glGetUniformLocation( mMaterial->GetShader()->GetProgram(), "TextureDiffuse" ), DIFFUSE );
      glBindTexture( GL_TEXTURE_2D, mMaterial->GetTextures()[DIFFUSE]->GetHandle() );
    }

    if ( mMaterial->GetTextures()[NORMAL]->GetHandle() ) {
      glActiveTexture( GL_TEXTURE1 );
      glUniform1i( glGetUniformLocation( mMaterial->GetShader()->GetProgram(), "TextureNormal" ), NORMAL );
      glBindTexture( GL_TEXTURE_2D, mMaterial->GetTextures()[NORMAL]->GetHandle() );
    }

    glBindVertexArray( mVAO );
    //    glDrawElements(GL_TRIANGLES, model->meshes[0].numTriangles,
    //    GL_UNSIGNED_INT, 0);
    glDrawArrays( GL_TRIANGLES, 0, mNumVertices );
  }

  inline void SetMaterial(const std::shared_ptr<Material>& material){
    mMaterial = material;
  }

protected:
  const char* mName;

  // GL data
  unsigned int mVBO, mVAO;

  unsigned int mNumVertices;
  std::vector<Vertex> mVertices;
  std::shared_ptr<Material> mMaterial;
};


#endif // DEV_MESH_H
