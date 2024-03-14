//
// Created by vector on 24/02/13.
//

#include "Camera.h"

Camera::Camera( float verticalFov, float aspectRatio, float nearPlaneDistance, float farPlaneDistance ) {
  mVerticalFOV = verticalFov;
  mNearDistance = nearPlaneDistance;
  mFarDistance = farPlaneDistance;
  mProjection = glm::perspective( verticalFov, aspectRatio, nearPlaneDistance, farPlaneDistance );
  mTransform  = glm::mat4(1.0f);
  UpdateViewMatrixCache();
}