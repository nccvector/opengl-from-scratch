//
// Author: Faizan Ali (github.com/nccvector)
// Date: 23/05/02
//

#ifndef OPENGL_FROM_SCRATCH_MODEL_H
#define OPENGL_FROM_SCRATCH_MODEL_H

#include <vector>

#include <glad/glad.h>

#include "Types.h"
#include "Material.h"

class PhongMaterial;

class Model {
public:
  Model( VertexList& vertices, UIntList& indices, PhongMaterial& material )
      : mVertices( vertices ), mIndices( indices ), mMaterial( material ) {
    deviceLoad();
  }

  ~Model() {
    deviceUnload();
  }

  void deviceLoad();
  void deviceUnload() {}

  inline const glm::mat4& getTransform() const {
    return mTransform4x4;
  }

  inline void setTransform( const glm::mat4& transform ) {
    mTransform4x4 = transform;
  }

  inline const PhongMaterial& getMaterial() const {
    return mMaterial;
  }

  inline unsigned int getVAO() const {
    return mGlVAO;
  }

  inline size_t getSize() const {
    return mIndices.size();
  }

private:
  VertexList mVertices    = {};
  UIntList mIndices       = {};
  glm::mat4 mTransform4x4 = glm::mat4( 1.0f );
  PhongMaterial& mMaterial;

  // OpenGL vars
  unsigned int mGlVBO {};
  unsigned int mGlVAO {};
  unsigned int mGlEBO {};
};


#endif // OPENGL_FROM_SCRATCH_MODEL_H
