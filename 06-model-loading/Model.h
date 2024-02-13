//
// Created by vector on 24/02/10.
//

#ifndef INC_06_MODEL_LOADING_MODEL_H
#define INC_06_MODEL_LOADING_MODEL_H

#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <fbxsdk.h>
#include "glad/glad.h"


struct Mesh{
  std::vector<glm::vec3> vertices;
  std::vector<glm::vec3> normal;
  std::vector<glm::vec2> uvs;
  std::vector<int> indices;
};

struct Model{
  unsigned int VBO, VAO, EBO;
  std::vector<Mesh> meshes;
  glm::mat4 transform = glm::mat4(1.0f);
  bool loadedOnDevice = false;
};


namespace ModelTools {

  void CreateModelFromFbxNode(FbxNode* node, Model& model);
  void LoadOnDevice(Model& model);

};


#endif // INC_06_MODEL_LOADING_MODEL_H
