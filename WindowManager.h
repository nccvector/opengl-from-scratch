//
// Created by vector on 24/03/10.
//

#ifndef DEV_WINDOWMANAGER_H
#define DEV_WINDOWMANAGER_H

#include <memory>
#include <functional>

// IMPORTANT! Make sure it is imported AFTER glad import
#include <GLFW/glfw3.h>

struct Window {
  const char* name;
  int width;
  int height;
  GLFWwindow* pointer;
};

class WindowManager {
public:
  WindowManager() {
    SetMainWindowData( "LearnOpenGL", 1280, 720 );
  }

  ~WindowManager() {
    Terminate();
  }

  void SetMainWindowData( const char* name, int width, int height ) {
    mMainWindow = std::make_shared<Window>();

    mMainWindow->name   = name;
    mMainWindow->width  = width;
    mMainWindow->height = height;
  }

  std::shared_ptr<Window> GetMainWindow() {
    return mMainWindow;
  }

  bool ShouldQuit() {
    return glfwWindowShouldClose( GetMainWindow()->pointer );
  }

  void InitializeMainWindow() {
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
  }

  void SetResizeCallback(GLFWframebuffersizefun callback){
    glfwSetFramebufferSizeCallback( mMainWindow->pointer, callback);
  }

  void Terminate() {
    // Destroy all the windows and terminate
    glfwTerminate();
  }


  void ProcessInput( std::shared_ptr<Window> window ) {
    if ( glfwGetKey( window->pointer, GLFW_KEY_ESCAPE ) == GLFW_PRESS ) {
      glfwSetWindowShouldClose( window->pointer, true );
    }
  }

  void ProcessInput() {
    ProcessInput( GetMainWindow() );
  }

  void SwapAndPoll( std::shared_ptr<Window> window ) {
    glfwSwapBuffers( window->pointer );
    glfwPollEvents();
  }

  void SwapAndPoll() {
    SwapAndPoll( GetMainWindow() );
  }


protected:
  std::shared_ptr<Window> mMainWindow;
};


#endif // DEV_WINDOWMANAGER_H
