//
// Created by vector on 24/03/15.
//

#ifndef DEV_FRAMEBUFFER_H
#define DEV_FRAMEBUFFER_H

#include "Common.h"
#include "Texture.h"

class Framebuffer {
public:
  Framebuffer( const char* name, int width, int height ) {
    mName = name;
    mWidth  = width;
    mHeight = height;
    mHandle = 0;

    // Initialize framebuffer and render textures
    glGenFramebuffers( 1, &( mHandle ) );
    glBindFramebuffer( GL_FRAMEBUFFER, mHandle );
    mRenderTexture = std::make_shared<Texture>( "GL Render texture", nullptr, mWidth, mHeight, 4 );

    // Initialize the depth texture
    // The depth buffer
    glGenRenderbuffers(1, &mDepthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, mDepthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, mWidth, mHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mDepthBuffer);

    // Attach render texture
    glFramebufferTexture( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, mRenderTexture->getHandle(), 0 );

    // Set the list of draw buffers.
    GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers( 1, DrawBuffers );

    // Always check that our framebuffer is ok
    if ( glCheckFramebufferStatus( GL_FRAMEBUFFER ) != GL_FRAMEBUFFER_COMPLETE ) {
      ERROR( "Framebuffer incomplete" );
      throw "FAILED FRAMEBUFFER";
    }
  }

  void resize(int width, int height){
    mWidth = width;
    mHeight = height;

    // Delete
    glDeleteFramebuffers(1, &mHandle);

    // Recreate
    glGenFramebuffers( 1, &( mHandle ) );
    glBindFramebuffer( GL_FRAMEBUFFER, mHandle );

    // Resize texture
    mRenderTexture->Resize(width, height);

    // Delete old depth buffer
    glDeleteRenderbuffers(1, &mDepthBuffer);

    // Recreate the depth texture
    // The depth buffer
    glGenRenderbuffers(1, &mDepthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, mDepthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, mWidth, mHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mDepthBuffer);

    // Reattach render texture
    glFramebufferTexture( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, mRenderTexture->getHandle(), 0 );

    // Set the list of draw buffers.
    GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers( 1, DrawBuffers );

    // Always check that our framebuffer is ok
    if ( glCheckFramebufferStatus( GL_FRAMEBUFFER ) != GL_FRAMEBUFFER_COMPLETE ) {
      ERROR( "Framebuffer resize failed, Framebuffer incomplete" );
      throw "FAILED FRAMEBUFFER";
    }

    DEBUG("Frame buffer resized");
  }

  void bind() const {
    glBindFramebuffer( GL_FRAMEBUFFER, mHandle );
  }

  [[maybe_unused]] std::shared_ptr<Texture> getRenderTexture(){
    return mRenderTexture;
  }

  inline unsigned int getRenderTextureHandle(){
    return mRenderTexture->getHandle();
  }

  [[nodiscard]] int getWidth() const{
    return mWidth;
  }

  [[nodiscard]] int getHeight() const{
    return mHeight;
  }

protected:
  const char* mName;
  int mWidth;
  int mHeight;

  unsigned int mHandle;
  unsigned int mDepthBuffer;
  std::shared_ptr<Texture> mRenderTexture;
};


#endif // DEV_FRAMEBUFFER_H
