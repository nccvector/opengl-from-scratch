//
// Author: Faizan Ali (github.com/nccvector)
// Date: 23/05/14
//

#ifndef OPENGL_FROM_SCRATCH_FRAMEBUFFER_H
#define OPENGL_FROM_SCRATCH_FRAMEBUFFER_H

#include <glad/glad.h>


class FrameBuffer {
public:
  FrameBuffer( int width, int height );

  ~FrameBuffer(){
    glDeleteFramebuffers(1, &mGLID);
    glDeleteTextures(1, &mColorAttachment);
    glDeleteTextures(1, &mDepthAttachment);
  }

  void bind();
  void unbind();
  bool complete();
  unsigned int& getID() {
    return mGLID;
  }

  unsigned int& getRenderTextureID() {
    return mColorAttachment;
  }

  unsigned int& getDepthTextureID() {
    return mDepthAttachment;
  }

private:
  unsigned int mGLID;
  unsigned int mColorAttachment;
  unsigned int mDepthAttachment;
};


#endif // OPENGL_FROM_SCRATCH_FRAMEBUFFER_H
