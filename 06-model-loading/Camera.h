//
// Created by vector on 24/02/13.
//

#ifndef INC_06_MODEL_LOADING_CAMERA_H
#define INC_06_MODEL_LOADING_CAMERA_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Camera {
public:
  Camera( float verticalFov, float aspectRatio, float nearPlaneDistance, float farPlaneDistance );

  inline glm::mat4 GetTransform() {
    return pTransform;
  }

  inline glm::mat4 GetProjection() {
    return pProjection;
  }

  inline glm::mat4 GetViewMatrix() {
    return pViewMatrixCache;
  }

  inline void SetTransform( glm::mat4 transform ) {
    pTransform = transform;
    UpdateViewMatrixCache();
  }

  inline void SetProjection( glm::mat4 projection ) {
    pProjection = projection;
    UpdateViewMatrixCache();
  }

  inline void UpdateViewMatrixCache() {
    pViewMatrixCache = pProjection * glm::inverse(pTransform);
  }

private:
  glm::mat4 pProjection;
  glm::mat4 pTransform;
  glm::mat4 pViewMatrixCache;
};


#endif // INC_06_MODEL_LOADING_CAMERA_H
