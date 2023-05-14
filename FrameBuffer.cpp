//
// Author: Faizan Ali (github.com/nccvector)
// Date: 23/05/14
//


#include <glad/glad.h>
#include "Texture.h"

#include "FrameBuffer.h"
FrameBuffer::FrameBuffer( int width, int height ) {
  glGenFramebuffers( 1, &mGLID );

  this->bind();

  // Create GL texture object
  glGenTextures( 1, &( mColorAttachment ) );

  glActiveTexture( GL_TEXTURE0 );
  glBindTexture( GL_TEXTURE_2D, mColorAttachment );

  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

  // Copy the data to texture
  glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL );
  glGenerateMipmap( GL_TEXTURE_2D );

  // Attach texture to GL_COLOR_ATTACHMENT0
  glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mColorAttachment, 0 );

  // DEPTH
  glGenRenderbuffers( 1, &mDepthAttachment );
  glBindRenderbuffer( GL_RENDERBUFFER, mDepthAttachment );
  glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height );
  glBindRenderbuffer( GL_RENDERBUFFER, 0 );

  glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mDepthAttachment );

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
