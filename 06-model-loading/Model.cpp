//
// Created by vector on 24/02/10.
//

#include <iostream>
#include "Model.h"

void ModelTools::CreateModelFromFbxNode( FbxNode* node, Model& model ) {
  FbxMesh* mesh = node->GetMesh();

  // Declare memory for mesh
  model.meshes.resize( 1 );
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
