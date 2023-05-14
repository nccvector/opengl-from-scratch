//
// Author: Faizan Ali (github.com/nccvector)
// Date: 23/05/14
//

#ifndef OPENGL_FROM_SCRATCH_GLVIEWPORT_H
#define OPENGL_FROM_SCRATCH_GLVIEWPORT_H

#include <iostream>
#include <vector>
#include <memory>
#include <assert.h>

#include "Texture.h"
#include "FrameBuffer.h"
#include "Model.h"
#include "Material.h"
#include "Camera.h"

class GLViewport {
public:
  GLViewport( int width, int height, const std::shared_ptr<Camera>& camera ) {
    mWidth  = width;
    mHeight = height;
    mCamera = camera;

    // Initializing frame buffer and renderTexture
    mFramebuffer = std::make_unique<FrameBuffer>(width, height);
    assert( mFramebuffer->complete() );
  }

  void setCamera( const std::shared_ptr<Camera>& camera ) {
    mCamera = camera;
  }

  void resize( int width, int height ) {
    mFramebuffer.release();
    mFramebuffer = std::make_unique<FrameBuffer>(width, height);

    mWidth  = width;
    mHeight = height;

    // RESIZE CAMERA???
  }

  int getWidth() {
    return mWidth;
  }

  int getHeight() {
    return mHeight;
  }

  void draw(
      const std::vector<Model>& models, const std::vector<PointLight> pointlights, std::shared_ptr<Shader> shader ) {

    if ( mCamera == nullptr ) {
      std::cerr << "Camera is NULL" << std::endl;
      return;
    }

    // Bind render frame buffer before drawing scene
    // Render to our framebuffer
    mFramebuffer->bind();
    glViewport( 0, 0, mWidth, mHeight );

    // Clear background
    glClearColor( 0.1f, 0.1f, 0.1f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // Send view projection transforms to shader
    shader->setModelViewProjectionMatrix( glm::mat4( 1.0 ), mCamera->getTransform(), mCamera->getProjection() );

    // Enable shader
    shader->use();

    // Send light data to shader
    shader->setPointLights( pointlights );

    // Draw models
    for ( auto model : models ) {
      shader->draw( model );
    }
  }

  unsigned int getRenderTextureID() {
    return mFramebuffer->getRenderTextureID();
  }

private:
  int mWidth;
  int mHeight;
  std::unique_ptr<FrameBuffer> mFramebuffer;
  std::shared_ptr<Camera> mCamera;
};


#endif // OPENGL_FROM_SCRATCH_GLVIEWPORT_H
