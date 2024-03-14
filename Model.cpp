//
// Created by vector on 24/03/08.
//

#include "Model.h"

Model::Model( const char* name, std::shared_ptr<Mesh> mesh ) {
  mName   = name;
  mMeshes = { mesh };
}