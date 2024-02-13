//
// Created by vector on 24/02/13.
//

#include "Camera.h"

Camera::Camera( float verticalFov, float aspectRatio, float nearPlaneDistance, float farPlaneDistance ) {
  pProjection = glm::perspective( verticalFov, aspectRatio, nearPlaneDistance, farPlaneDistance );
  pTransform  = glm::mat4(1.0f);
  UpdateTransformProjectionCache();
}