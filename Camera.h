//
// Author: Faizan Ali (github.com/nccvector)
// Date: 23/05/14
//

#ifndef OPENGL_FROM_SCRATCH_CAMERA_H
#define OPENGL_FROM_SCRATCH_CAMERA_H

#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
  Camera(
      int width = 800, int height = 600, float verticalFOV = 45.0f, float zNear = 0.0001f, float zFar = 100000.0f ) {
    mWidth       = width;
    mHeight      = height;
    mVerticalFOV = verticalFOV;
    mZNear       = zNear;
    mZFar        = zFar;
    mTransform   = glm::mat4( 1.0 );
    mProjection  = glm::perspective( glm::radians( mVerticalFOV ), (float) mWidth / (float) mHeight, mZNear, mZFar );
  }

  void resize( int width, int height ) {
    mWidth  = width;
    mHeight = height;

    // Consistent projection on resize
    mProjection = glm::perspective( glm::radians( mVerticalFOV ), (float) mWidth / (float) mHeight, mZNear, mZFar );
  }

  void lookAt( glm::vec3 target ) {
    mTransform = glm::lookAt( glm::vec3( mTransform[3] ), target, glm::vec3( 0, 1, 0 ) );
  }

  void setPosition( glm::vec3 position ) {
    mTransform = glm::translate(mTransform, position - glm::vec3(mTransform[3]));
  }

  glm::mat4 getTransform() {
    return mTransform;
  }

  glm::mat4 getProjection() {
    return mProjection;
  }

private:
  int mWidth;
  int mHeight;
  float mVerticalFOV;
  float mZNear;
  float mZFar;
  glm::mat4 mTransform;
  glm::mat4 mProjection;
};


#endif // OPENGL_FROM_SCRATCH_CAMERA_H
