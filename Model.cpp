//
// Author: Faizan Ali (github.com/nccvector)
// Date: 23/05/02
//

#include "Model.h"

namespace ModelTools {
void LoadOnDevice(
    unsigned int& VAO, unsigned int& VBO, unsigned int& EBO, const VertexList& vertices, const UIntList& indices ) {
  // Create a vertex buffer
  glGenBuffers( 1, &VBO );

  // Create a vertex array object
  glGenVertexArrays( 1, &VAO );

  // Create an index buffer
  glGenBuffers( 1, &EBO );

  // 1. Bind VAO
  glBindVertexArray( VAO );
  // 2. Bind VBO and copy vertex data
  glBindBuffer( GL_ARRAY_BUFFER, VBO );
  glBufferData( GL_ARRAY_BUFFER, sizeof( Vertex ) * vertices.size(), &vertices[0], GL_STATIC_DRAW );
  // 3. Bind EBO and copy index data
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO );
  glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( unsigned int ) * indices.size(), &indices[0], GL_STATIC_DRAW );

  // 4. Linking vertex attributes (how the data is layed out in memory)
  // OpenGL does not know how the vertices are layed out in memory
  // To let the OpenGL know how to connect the vertices, we need to specify
  glVertexAttribPointer( 0,                    // Corresponds to the location = 0 in vertex shader
      (GLint) Vertex::getPositionNumEntries(), // Entries per vertex data (x y and z)
      GL_FLOAT,                                // Type of entry
      GL_FALSE,                                // Specifies whether the data should be normalized or not
      (GLint) Vertex::getSize(),               // Stride size in bytes
      (void*) Vertex::getPositionOffset() );   // offset: Where the position data begins in the buffer
  glEnableVertexAttribArray( 0 );              // Enable this attribute array
  // Create one more for color data layout
  glVertexAttribPointer( 1,                  // Corresponds to the location = 0 in vertex shader
      (GLint) Vertex::getNormalNumEntries(), // Entries per vertex data (r g and b)
      GL_FLOAT,                              // Type of entry
      GL_FALSE,                              // Specifies whether the data should be normalized or not
      (GLint) Vertex::getSize(),             // Stride size in bytes
      (void*) Vertex::getNormalOffset() );   // offset: Where the Normal data begins in the buffer
  glEnableVertexAttribArray( 1 );            // Enable this attribute array
  // Create one more for uv data layout
  glVertexAttribPointer( 2,                    // Corresponds to the location = 0 in vertex shader
      (GLint) Vertex::getTexCoordNumEntries(), // Entries per vertex data (u and v)
      GL_FLOAT,                                // Type of entry
      GL_FALSE,                                // Specifies whether the data should be normalized or not
      (GLint) Vertex::getSize(),               // Stride size in bytes
      (void*) Vertex::getTexCoordOffset() );   // offset: Where the position data begins in the buffer
  glEnableVertexAttribArray( 2 );              // Enable this attribute array

  // Unbind after creation
  glBindVertexArray( 0 );
}
} // namespace ModelTools