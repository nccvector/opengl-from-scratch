//
// Created by vector on 24/02/10.
//

#include <iostream>
#include "Model.h"

void ModelTools::CreateModelFromFbxNode( FbxNode* node, Model& model ) {
  FbxMesh* mesh = node->GetMesh();

  // Declare memory for mesh
  model.meshes.resize( 1 );

  int vertexCount              = mesh->GetControlPointsCount();
  int indexCount               = mesh->GetPolygonVertexCount();
  model.meshes[0].numTriangles = indexCount;

  // Copy indices to model
  int* indices = mesh->GetPolygonVertices();

  // Resize vertices to be of the size of elements
  model.meshes[0].vertices.resize( indexCount ); // Declare memory

  // Copy vertices to model
  FbxVector4* pVertices = mesh->GetControlPoints();
  model.meshes[0].vertices.resize( indexCount ); // Declare memory

  // Copy normals to model
  FbxArray<FbxVector4> pNormals;
  mesh->GenerateNormals();
  mesh->GetPolygonVertexNormals( pNormals );

  for ( int i = 0; i < indexCount; i++ ) {
    int vertexIndex    = indices[i];
    FbxVector4 pVertex = pVertices[vertexIndex];

    FbxVector4 pNormal;
    if ( pNormals.Size() > 0 ) {
      pNormal = pNormals[i];
    } else {
      pNormal = { 0, 0, 0, 0 };
    }

    // Add position
    model.meshes[0].vertices[i].position = {
        static_cast<float>( pVertex[0] ), static_cast<float>( pVertex[1] ), static_cast<float>( pVertex[2] ) };

    // Add normal
    model.meshes[0].vertices[i].normal = {
        static_cast<float>( pNormal[0] ), static_cast<float>( pNormal[1] ), static_cast<float>( pNormal[2] ) };

    model.meshes[0].vertices[i].texCoord = { 0, 0 };

    //    std::cout << "Vertex: " << model.meshes[0].vertices[i][0] << ", " << model.meshes[0].vertices[i][1] << ", "
    //              << model.meshes[0].vertices[i][2] << "\n";
  }

  std::cout << "Loaded vertex count: " << model.meshes[0].vertices.size() << "\n";
}

void ModelTools::LoadOnDevice( Model& model ) {
  glGenVertexArrays( 1, &( model.meshes[0].VAO ) );
  glGenBuffers( 1, &( model.meshes[0].VBO ) );

  // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
  glBindVertexArray( model.meshes[0].VAO );

  glBindBuffer( GL_ARRAY_BUFFER, model.meshes[0].VBO );
  glBufferData( GL_ARRAY_BUFFER, sizeof( Vertex ) * model.meshes[0].vertices.size(), model.meshes[0].vertices.data(),
      GL_STATIC_DRAW );

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

  model.loadedOnDevice = true;
}
