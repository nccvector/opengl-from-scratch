//
// Created by vector on 24/02/10.
//

#include <iostream>
#include "Model.h"

void ModelTools::CreateModelFromFbxNode( FbxNode* node, Model& model ) {
  FbxMesh* mesh = node->GetMesh();

  // Declare memory for mesh
  model.meshes.resize( 1 );

  int vertexCount = mesh->GetControlPointsCount();
  int indexCount  = mesh->GetPolygonVertexCount();

  // Copy indices to model
  int* indices = mesh->GetPolygonVertices();
  model.meshes[0].indices.resize( indexCount ); // Declare memory
  memcpy( model.meshes[0].indices.data(), indices, indexCount * sizeof( int ) );

  // Copy vertices to model
  FbxVector4* pVertices = mesh->GetControlPoints();
  model.meshes[0].vertices.resize( 3 * vertexCount ); // Declare memory

  for ( int i = 0; i < vertexCount; i++ ) {
    model.meshes[0].vertices[i] = {
      static_cast<float>( pVertices[i][0] ),
      static_cast<float>( pVertices[i][1] ),
      static_cast<float>( pVertices[i][2] )
    };

    std::cout << "Vertex: " << model.meshes[0].vertices[i][0] << ", " << model.meshes[0].vertices[i][1] << ", "
              << model.meshes[0].vertices[i][2] << "\n";
  }
}

void ModelTools::LoadOnDevice( Model& model ) {
  glGenVertexArrays( 1, &( model.VAO ) );
  glGenBuffers( 1, &( model.VBO ) );
  glGenBuffers( 1, &( model.EBO ) );

  // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
  glBindVertexArray( model.VAO );

  glBindBuffer( GL_ARRAY_BUFFER, model.VBO );
  glBufferData( GL_ARRAY_BUFFER, sizeof( glm::vec3 ) * model.meshes[0].vertices.size(), model.meshes[0].vertices.data(),
      GL_STATIC_DRAW );

  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, model.EBO );
  glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( int ) * model.meshes[0].indices.size(), model.meshes[0].indices.data(),
      GL_STATIC_DRAW );

  glVertexAttribPointer( 0, // shader location index
      3,                    // number of components per attribute
      GL_FLOAT,
      GL_FALSE,            // normalized (clamp between -1 to 1)
      3 * sizeof( float ), // stride in bytes to the next same vertex attribute
      (void*) 0            // byte size offset for the first component
  );
  glEnableVertexAttribArray( 0 );

  // Release buffer
  glBindBuffer( GL_ARRAY_BUFFER, 0 );

  // Release buffer array
  glBindVertexArray( 0 );

  model.loadedOnDevice = true;
}
