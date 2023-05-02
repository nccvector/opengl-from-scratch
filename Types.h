//
// Author: Faizan Ali (github.com/nccvector)
// Date: 23/05/02
//

#ifndef OPENGL_FROM_SCRATCH_TYPES_H
#define OPENGL_FROM_SCRATCH_TYPES_H

#endif // OPENGL_FROM_SCRATCH_TYPES_H

#include <vector>
#include "Vertex.h"

// GLM
#include <glm/gtc/matrix_transform.hpp>

typedef glm::vec2 Vec2;
typedef glm::vec3 Vec3;
typedef glm::vec4 Vec4;

typedef std::vector<glm::vec3> Vec3List;
typedef std::vector<glm::vec2> Vec2List;
typedef std::vector<unsigned int> UIntList;

typedef std::vector<Vertex> VertexList;