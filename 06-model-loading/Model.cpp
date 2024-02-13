//
// Created by vector on 24/02/10.
//

#include <iostream>
#include "Model.h"

void Model::LoadOnDevice() {
  FbxMesh* mesh        = GetMesh();
  FbxVector4 *pVertices = mesh->GetControlPoints();
  int *indices = mesh->GetPolygonVertices();
  int vertexCount      = mesh->GetControlPointsCount();
  int indexCount       = mesh->GetPolygonVertexCount();
  const int VERTEX_STRIDE = 4;

  // Create vertices and indices
  float vertices[vertexCount * 4];

  for (int i=0; i<vertexCount; i++)
  {
    vertices[i * VERTEX_STRIDE] = static_cast<float>(pVertices[i][0]) / 10;
    vertices[i * VERTEX_STRIDE + 1] = static_cast<float>(pVertices[i][1]) / 10;
    vertices[i * VERTEX_STRIDE + 2] = static_cast<float>(pVertices[i][2]) / 10;
    vertices[i * VERTEX_STRIDE + 3] = 1.0f;

    std::cout << "Vertex: " <<
    vertices[i * VERTEX_STRIDE] << ", " <<
    vertices[i * VERTEX_STRIDE + 1] << ", " <<
    vertices[i * VERTEX_STRIDE + 2] << ", " <<
    vertices[i * VERTEX_STRIDE + 3] << "\n";
  }

  if ( !mesh ) {
    return;
  }

  glGenVertexArrays( 1, &VAO );
  glGenBuffers( 1, &VBO );
  glGenBuffers( 1, &EBO );
  // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
  glBindVertexArray( VAO );

  glBindBuffer( GL_ARRAY_BUFFER, VBO );
  glBufferData( GL_ARRAY_BUFFER, VERTEX_STRIDE * sizeof( float ) * vertexCount, vertices, GL_STATIC_DRAW );

  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO );
  glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( int ) * indexCount, indices, GL_STATIC_DRAW );

  glVertexAttribPointer( 0, // shader location index
      4,                    // number of components per attribute
      GL_FLOAT,
      GL_FALSE,            // normalized (clamp between -1 to 1)
      4 * sizeof( float ), // stride in bytes to the next same vertex attribute
      (void*) 0            // byte size offset for the first component
  );
  glEnableVertexAttribArray( 0 );

  //    glVertexAttribPointer( 1, // shader location index
  //        2,                    // number of components per attribute
  //        GL_FLOAT,
  //        GL_FALSE,                       // normalized
  //        5 * sizeof( float ),            // stride in bytes to the next same vertex attribute
  //        (void*) ( 3 * sizeof( float ) ) // byte size offset for first component
  //    );
  //    glEnableVertexAttribArray( 1 );

  // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound
  // vertex buffer object so afterwards we can safely unbind
  glBindBuffer( GL_ARRAY_BUFFER, 0 );

  // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO;
  // keep the EBO bound. glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens.
  // Modifying other VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs)
  // when it's not directly necessary.
  glBindVertexArray( 0 );
}

unsigned int Model::GetVAO() {
  return VAO;
}

int Model::GetElementsCount() {
  return GetMesh()->GetPolygonVertexCount();
}
