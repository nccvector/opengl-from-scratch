//
// Created by vector on 23/04/24.
//

#ifndef OPENGL_FROM_SCRATCH_CAMERA_H
#define OPENGL_FROM_SCRATCH_CAMERA_H

#include "Node.h"


class Camera : Node {
public:
  explicit Camera( unsigned int width = 800, unsigned int height = 600, float near = 0.0001f, float far = 100000.0f );
  ~Camera();

  glm::vec3 getPosition();
  void setPosition( const glm::vec3& position );

  void lookAt( const glm::vec3& position, const glm::vec3& upVector );

private:
  unsigned int mWidth;
  unsigned int mHeight;
  float mNear;
  float mFar;
  glm::mat4 mProjectionMatrix;
};


#endif // OPENGL_FROM_SCRATCH_CAMERA_H
