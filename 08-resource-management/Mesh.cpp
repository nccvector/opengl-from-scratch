//
// Created by vector on 24/03/08.
//

#include "Material.h"
#include "Mesh.h"

#include "ResourceManager.h"

void Mesh::Draw() {
  // Activate shader
  ResourceManager::EnsureShaderActiveState( mMaterial->GetShader() );

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

Mesh::Mesh( const char* name, const std::shared_ptr<Material>& material ) {
  mName     = name;
  mMaterial = material;
}

void Mesh::CreateOnHost( const std::vector<Vertex>& vertices ) {
  mVertices    = vertices;
  mNumVertices = mVertices.size();
}

void Mesh::CreateOnDevice() {
  glGenVertexArrays( 1, &( mVAO ) );
  glGenBuffers( 1, &( mVBO ) );

  // bind the Vertex Array Object first, then bind and set vertex buffer(s), and
  // then configure vertex attributes(s).
  glBindVertexArray( mVAO );

  glBindBuffer( GL_ARRAY_BUFFER, mVBO );
  glBufferData( GL_ARRAY_BUFFER, sizeof( Vertex ) * mVertices.size(), mVertices.data(), GL_STATIC_DRAW );

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
      GL_FALSE,         // normalized (clamp between -1 to 1)
      sizeof( Vertex ), // stride in bytes to the next same vertex attribute
      (void*) offsetof( Vertex,
          normal ) // byte size offset for the first component
  );

  // TexCoord
  glEnableVertexAttribArray( texCoordAttribIndex );
  glVertexAttribPointer( texCoordAttribIndex, // shader location index
      3,                                      // number of components per attribute
      GL_FLOAT,
      GL_FALSE,         // normalized (clamp between -1 to 1)
      sizeof( Vertex ), // stride in bytes to the next same vertex attribute
      (void*) offsetof( Vertex,
          texCoord ) // byte size offset for the first component
  );

  // Release buffer
  glBindBuffer( GL_ARRAY_BUFFER, 0 );

  // Release buffer array
  glBindVertexArray( 0 );
}

void Mesh::CreateFromFbxMesh( FbxMesh* mesh ) {
  std::vector<Vertex> vertices = GetVerticesFromFbxMesh( mesh );

  CreateOnHost( vertices );
  CreateOnDevice();
  // ReleaseFromHost();    // Free vertices memory
}

std::vector<Vertex> Mesh::GetVerticesFromFbxMesh( FbxMesh* mesh ) {
  // ================================================================================
  // Load mesh
  // ================================================================================

  if ( !mesh ) {
    throw "Mesh is NULL";
  }

  // Declare memory for mesh
  std::vector<Vertex> vertices;

  int vertexCount = mesh->GetControlPointsCount();
  int indexCount  = mesh->GetPolygonVertexCount();

  // Copy indices to model
  int* indices = mesh->GetPolygonVertices();

  // Resize vertices to be of the size of elements
  vertices.resize( indexCount ); // Declare memory

  // Copy vertices to model
  FbxVector4* pVertices = mesh->GetControlPoints();

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
    float scale          = 1.0f;
    vertices[i].position = { scale * static_cast<float>( pVertex[0] ), scale * static_cast<float>( pVertex[1] ),
        scale * static_cast<float>( pVertex[2] ) };

    // Add normal
    vertices[i].normal = {
        static_cast<float>( pNormal[0] ), static_cast<float>( pNormal[1] ), static_cast<float>( pNormal[2] ) };

    vertices[i].texCoord = { static_cast<float>( pTexCoord[0] ), static_cast<float>( pTexCoord[1] ) };

    //    std::cout << "Vertex: " << model.meshes[0].vertices[i][0] << ", " <<
    //    model.meshes[0].vertices[i][1] << ", "
    //              << model.meshes[0].vertices[i][2] << "\n";
  }

  //  std::cout << "Loaded vertex count: " << vertices.size() << "\n";

  // ================================================================================

  return vertices;
}
