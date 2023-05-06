//
// Author: Faizan Ali (github.com/nccvector)
// Date: 23/05/06
//

#ifndef OPENGL_FROM_SCRATCH_LIGHT_H
#define OPENGL_FROM_SCRATCH_LIGHT_H

#include "Types.h"

struct PointLight {
  float Intensity;
  Vec3 Color;
  Vec3 Position;
};

struct DirectionalLight
{
  float Intensity;
  Vec3 Color;
  Vec3 Direction;
};

struct SpotLight
{
  float Intensity;
  Vec3 Color;
  Vec3 Position;
  Vec3 Direction;
  Vec3 Angle;
};

struct SimplexAreaLight
{
  float Intensity;
  Vec3 Vertices[3];
};



#endif // OPENGL_FROM_SCRATCH_LIGHT_H
