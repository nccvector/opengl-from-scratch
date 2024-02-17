//
// Created by vector on 24/02/10.
//

#ifndef INC_06_MODEL_LOADING_MODEL_H
#define INC_06_MODEL_LOADING_MODEL_H

#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <fbxsdk.h>
#include "glad/glad.h"

struct Vertex{
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texCoord;
};

struct Mesh{
  unsigned int VBO, VAO;
  unsigned int numVertices;
};

struct Model{
  std::vector<Mesh> meshes;
  glm::mat4 transform = glm::mat4(1.0f);
  bool loadedOnDevice = false;
};


namespace ModelTools {

  void CreateModelFromFbxNode(FbxNode* node, Model& model);
  void LoadMeshOnDevice(Mesh& mesh, std::vector<Vertex> vertices);

};


#endif // INC_06_MODEL_LOADING_MODEL_H
