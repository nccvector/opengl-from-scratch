//
// Created by vector on 24/02/10.
//

#ifndef INC_06_MODEL_LOADING_MODEL_H
#define INC_06_MODEL_LOADING_MODEL_H

#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <fbxsdk.h>
#include "glad/glad.h"

#include "common.h"

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texCoord;
};

struct Mesh {
  unsigned int VBO, VAO;
  unsigned int numVertices;
  Material material;
};

struct Model {
  const char* name;
  std::vector<Mesh> meshes;
  glm::mat4 transform = glm::mat4( 1.0f );
  bool loadedOnDevice = false;
};


namespace ModelTools {

bool CreateTextureData( unsigned char* data, Texture* texture, const char* name, int width, int height, int channels );
bool CreateTextureFromPath( const char* filePath, Texture* texture, const char* name );
void CreateModelFromFbxNode( FbxNode* node, Model& model );
void LoadMeshOnDevice( Mesh& mesh, std::vector<Vertex> vertices );

} // namespace ModelTools


#endif // INC_06_MODEL_LOADING_MODEL_H
