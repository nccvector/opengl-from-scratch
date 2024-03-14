//
// Created by vector on 24/03/10.
//

#include "WindowManager.h"

void framebufferSizeCallback( GLFWwindow* window, int width, int height ) {
  // make sure the viewport matches the new window dimensions; note that width
  // and height will be significantly larger than specified on retina displays.
  glfwGetFramebufferSize( window, &width, &height );

  glViewport( 0, 0, width, height );
}

WindowManager::WindowManager() {
  SetMainWindowData( "LearnOpenGL", 1280, 720 );
}

void WindowManager::SetMainWindowData( const char* name, int width, int height ) {
  mMainWindow = std::make_shared<Window>();

  mMainWindow->name   = name;
  mMainWindow->width  = width;
  mMainWindow->height = height;
}

void WindowManager::InitializeMainWindow() {
  // glfw: initialize and configure
  // ------------------------------
  glfwInit();
  glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
  glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
  glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

#ifdef __APPLE__
  glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
#endif

  // glfw window creation
  // --------------------
  mMainWindow->pointer =
      glfwCreateWindow( mMainWindow->width, mMainWindow->height, mMainWindow->name, nullptr, nullptr );

  if ( mMainWindow->pointer == nullptr ) {
    // TODO: Add logging
    glfwTerminate();
  }

  glfwMakeContextCurrent( mMainWindow->pointer ); // std::shared_ptr.get() returns a raw pointer
  glfwSetFramebufferSizeCallback( mMainWindow->pointer, framebufferSizeCallback );
}

void WindowManager::ProcessInput() {
  ProcessInput( GetMainWindow() );
}

void WindowManager::ProcessInput( std::shared_ptr<Window> window ) {
  if ( glfwGetKey( window->pointer, GLFW_KEY_ESCAPE ) == GLFW_PRESS ) {
    glfwSetWindowShouldClose( window->pointer, true );
  }
}

void WindowManager::SwapAndPoll() {
  SwapAndPoll( GetMainWindow() );
}

void WindowManager::SwapAndPoll( std::shared_ptr<Window> window ) {
  glfwSwapBuffers( window->pointer );
  glfwPollEvents();
}


void WindowManager::Terminate() {
  // Destroy all the windows and terminate
  glfwTerminate();
}