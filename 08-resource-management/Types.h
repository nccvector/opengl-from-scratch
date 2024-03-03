//
// Created by vector on 24/03/03.
//

#ifndef INC_08_RESOURCE_MANAGEMENT_TYPES_H
#define INC_08_RESOURCE_MANAGEMENT_TYPES_H

enum TextureType { DIFFUSE = 0, NORMAL, TEXTURETYPE_NUM_ITEMS };

// Application Level Data structures and Resources
struct Texture {
  const char* name;
  int width;
  int height;
  int channels;
  unsigned int handle;
};

struct Material {
  const char* name;
  std::shared_ptr<Texture> textures[TEXTURETYPE_NUM_ITEMS];
};

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texCoord;
};

struct Mesh {
  const char* name;

  // GL data
  unsigned int VBO, VAO;

  unsigned int numVertices;
  std::vector<Vertex> vertices;
  std::shared_ptr<Material> material;
};

struct Model {
  const char* name;
  std::vector<std::shared_ptr<Mesh>> meshes;
  glm::mat4 transform = glm::mat4( 1.0f );
  bool loadedOnDevice = false;
};


#endif // INC_08_RESOURCE_MANAGEMENT_TYPES_H
