//
// Created by vector on 23/04/24.
//

#include "Camera.h"

Camera::Camera( unsigned int width, unsigned height, float near, float far ) : Node() {
  mWidth            = width;
  mHeight           = height;
  mNear             = near;
  mFar              = far;
  mProjectionMatrix = glm::perspective( glm::radians( 45.0f ), (float) mWidth / (float) mHeight, mNear, mFar );
}

Camera::~Camera() {}

glm::vec3 Camera::getPosition() {
  return glm::vec3 { getTransform()[3] };
}

void Camera::lookAt( const glm::vec3& position, const glm::vec3& upVector ) {
  glm::mat4& transform = getTransform();

  transform = glm::lookAt( getPosition(), position, upVector );
}

void Camera::setPosition( const glm::vec3& position ) {
  glm::mat4& transform = getTransform();

  transform = glm::translate( transform, position );
}
