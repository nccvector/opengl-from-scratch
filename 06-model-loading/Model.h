//
// Created by vector on 24/02/10.
//

#ifndef INC_06_MODEL_LOADING_MODEL_H
#define INC_06_MODEL_LOADING_MODEL_H

#include <fbxsdk.h>
#include "glad/glad.h"


class Model : public FbxNode {
public:
  void LoadOnDevice();

  unsigned int GetVAO();
  int GetElementsCount();

protected:
  unsigned int VBO, VAO, EBO;
};


#endif // INC_06_MODEL_LOADING_MODEL_H
