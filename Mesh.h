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
  Mesh( const char* name, const std::shared_ptr<Material>& material );

  void createFromVertices(const std::vector<Vertex>& vertices);
  void createFromFbxMesh( FbxMesh* mesh );
  static std::vector<Vertex> getVerticesFromFbxMesh( FbxMesh* mesh );

  void createOnHost( const std::vector<Vertex>& vertices );
  void createOnDevice();

  [[maybe_unused]] inline const char* getName() {
    return mName;
  }

  void Draw();

  [[maybe_unused]] inline void setMaterial( const std::shared_ptr<Material>& material ) {
    mMaterial = material;
  }

protected:
  const char* mName;

  // GL data
  unsigned int mVBO, mVAO;

  // host data
  unsigned int mNumVertices;
  std::vector<Vertex> mVertices;
  std::shared_ptr<Material> mMaterial;
};


#endif // DEV_MESH_H
