//
// Author: Faizan Ali (github.com/nccvector)
// Date: 23/05/14
//


#include <glad/glad.h>
#include "Texture.h"

#include "FrameBuffer.h"
FrameBuffer::FrameBuffer() {
  glGenFramebuffers( 1, &mGLID );
  glBindFramebuffer( GL_FRAMEBUFFER, mGLID );

  // Unbind before leaving
  this->unbind();
}

void FrameBuffer::addRenderTexture( const Texture& texture ) {
  // bind this before adding texture
  this->bind();

  // Attach texture to GL_COLOR_ATTACHMENT0
  glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mGLID, 0 );

  // DEPTH
  unsigned int rbo;
  glGenRenderbuffers( 1, &rbo );
  glBindRenderbuffer( GL_RENDERBUFFER, rbo );
  glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, texture.Width, texture.Height );
  glBindRenderbuffer( GL_RENDERBUFFER, 0 );

  glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo );

  // Unbind before leaving
  this->unbind();
}

void FrameBuffer::bind() {
  glBindFramebuffer( GL_FRAMEBUFFER, mGLID );
}

void FrameBuffer::unbind() {
  glBindFramebuffer( GL_FRAMEBUFFER, 0 );
}

bool FrameBuffer::complete() {
  // Bind this framebuffer before checking
  this->bind();
  bool complete = glCheckFramebufferStatus( GL_FRAMEBUFFER ) == GL_FRAMEBUFFER_COMPLETE;

  // Unbind before leaving
  this->unbind();

  return complete;
}

