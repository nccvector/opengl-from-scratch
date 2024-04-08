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

  void resize( int width, int height ) {
    setProjection(
        glm::perspective( getVerticalFov(), (float) width / (float) height, getNearDistance(), getFarDistance() ) );
  }

  [[maybe_unused]] glm::mat4 getTransform() {
    return mTransform;
  }

  [[maybe_unused]] glm::mat4 getProjection() {
    return mProjection;
  }

  glm::mat4 getViewMatrix() {
    return mViewMatrixCache;
  }

  void setTransform( glm::mat4 transform ) {
    mTransform = transform;
    updateViewMatrixCache();
  }

  void setProjection( glm::mat4 projection ) {
    mProjection = projection;
    updateViewMatrixCache();
  }

  void updateViewMatrixCache() {
    mViewMatrixCache = mProjection * glm::inverse( mTransform );
  }

  [[nodiscard]] float getVerticalFov() const {
    return mVerticalFOV;
  }

  [[nodiscard]] float getNearDistance() const {
    return mNearDistance;
  }

  [[nodiscard]] float getFarDistance() const {
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
