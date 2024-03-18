//
// Created by vector on 24/03/17. //

#ifndef DEV_VIEWPORT_H
#define DEV_VIEWPORT_H

#include "Common.h"
#include "Framebuffer.h"
#include "Camera.h"

class Viewport {
public:
  Viewport( const char* name, float width, float height ) {
    mName        = name;
    mWidth       = width;
    mHeight      = height;
    mCropWidth   = width;
    mCropHeight  = height;
    mFramebuffer = std::make_shared<Framebuffer>( "Render Framebuffer", mWidth, mHeight );
    mCamera      = nullptr;
  }

  void SetAsDrawTarget() {
    // Tell OpenGL to draw with the same size as this viewport
    // Otherwise OpenGL will draw on bigger or smaller bounds
    // Hence, parts of the drawn image will go outside the viewport
    glViewport( 0, 0, mWidth, mHeight );

    mFramebuffer->Bind();
  }

  unsigned int GetRenderTextureHandle() {
    return mFramebuffer->GetRenderTextureHandle();
  }

  void DrawGui() {
    ImGui::Begin( mName );
    ImGui::Image(
        (void*) ( GetRenderTextureHandle() ), { (float) mCropWidth, (float) mCropHeight }, { 0, 1 }, { 1, 0 } );

    // Update widht and height to handle resizing of the draw region (mHeight and mWidth will be used
    // by SetAsDrawTarget in the next frame.
    ImVec2 minRegion = ImGui::GetWindowContentRegionMin();
    ImVec2 maxRegion = ImGui::GetWindowContentRegionMax();
    ImVec2 size      = { maxRegion.x - minRegion.x, maxRegion.y - minRegion.y };

    if ( size.x != mCropWidth || size.y != mCropHeight ) {
      mCropWidth  = size.x;
      mCropHeight = size.y;

      if ( mCropWidth > mFramebuffer->GetWidth() || mCropWidth > mFramebuffer->GetHeight() ) {
        ERROR( "Viewport frame buffer maximum size reached! Framebuffer resize is not implemented. You might experience pixelated rendering." );
      }

      // Re-adjust the camera if it's not NULL
      if ( mCamera ) {
        DEBUG( "RESIZING CAMERA" );
        mCamera->Resize( mCropWidth, mCropHeight );
      }
    }

    ImGui::End();
  }

  void SetCamera( const std::shared_ptr<Camera>& camera ) {
    mCamera = camera;
  }

  float GetWidth() {
    return mCropWidth;
  }

  float GetHeight() {
    return mCropHeight;
  }

protected:
  const char* mName;
  float mWidth;
  float mHeight;
  float mCropWidth;
  float mCropHeight;
  std::shared_ptr<Framebuffer> mFramebuffer;
  std::shared_ptr<Camera> mCamera;
};


#endif // DEV_VIEWPORT_H
