//
// Created by vector on 24/03/08.
//

#ifndef DEV_MODEL_H
#define DEV_MODEL_H

#include <memory>
#include <vector>

// GL Mathematics
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"

#include "Mesh.h"


class Model {
public:
  Model( const char* name, std::shared_ptr<Mesh> mesh );

  inline glm::mat4& GetTransform() {
    return mTransform;
  }

  inline void Draw() {
    for ( std::shared_ptr<Mesh> mesh : mMeshes ) {
      mesh->Draw();
    }
  }

protected:
  const char* mName;
  std::vector<std::shared_ptr<Mesh>> mMeshes;
  glm::mat4 mTransform = glm::mat4( 1.0f );
};


#endif // DEV_MODEL_H
