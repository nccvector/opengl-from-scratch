//
// Author: Faizan Ali (github.com/nccvector)
// Date: 23/05/02
//

#ifndef OPENGL_FROM_SCRATCH_VERTEX_H
#define OPENGL_FROM_SCRATCH_VERTEX_H

#include <glm/gtc/matrix_transform.hpp>

struct Vertex {
  glm::vec3 Position;
  glm::vec3 Normal;
  glm::vec2 TexCoord;

  static unsigned int getSize() {
    return sizeof( Vertex );
  }
  static unsigned int getPositionNumEntries() {
    return sizeof( Position ) / sizeof( float );
  }
  static unsigned int getNormalNumEntries() {
    return sizeof( Normal ) / sizeof( float );
  }
  static unsigned int getTexCoordNumEntries() {
    return sizeof( TexCoord ) / sizeof( float );
  }
  static unsigned int getPositionOffset() {
    return 0;
  }
  static unsigned int getNormalOffset() {
    return sizeof( Position );
  }
  static unsigned int getTexCoordOffset() {
    return sizeof( Position ) + sizeof( Normal );
  }
};


#endif // OPENGL_FROM_SCRATCH_VERTEX_H
