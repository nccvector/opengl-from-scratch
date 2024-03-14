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

  glm::mat4 GetTransform() {
    return mTransform;
  }

  glm::mat4 GetProjection() {
    return mProjection;
  }

  glm::mat4 GetViewMatrix() {
    return mViewMatrixCache;
  }

  void SetTransform( glm::mat4 transform ) {
    mTransform = transform;
    UpdateViewMatrixCache();
  }

  void SetProjection( glm::mat4 projection ) {
    mProjection = projection;
    UpdateViewMatrixCache();
  }

  void UpdateViewMatrixCache() {
    mViewMatrixCache = mProjection * glm::inverse(mTransform);
  }

  float GetVerticalFOV(){
    return mVerticalFOV;
  }

  float GetNearDistance(){
    return mNearDistance;
  }

  float GetFarDistacne(){
    return mFarDistance;
  }

private:
  float mVerticalFOV;
  float mNearDistance;
  float mFarDistance;
  glm::mat4 mProjection;
  glm::mat4 mTransform;
  glm::mat4 mViewMatrixCache;
};


#endif // INC_06_MODEL_LOADING_CAMERA_H
