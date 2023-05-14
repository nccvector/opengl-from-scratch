//
// Author: Faizan Ali (github.com/nccvector)
// Date: 23/05/14
//

#ifndef OPENGL_FROM_SCRATCH_FRAMEBUFFER_H
#define OPENGL_FROM_SCRATCH_FRAMEBUFFER_H


class FrameBuffer {
public:
  FrameBuffer();
  void addRenderTexture(const Texture& texture);
  void bind();
  void unbind();
  bool complete();
  unsigned int& getID(){
    return mGLID;
  }

private:
  unsigned int mGLID;
};


#endif // OPENGL_FROM_SCRATCH_FRAMEBUFFER_H
