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

  }

  void Terminate() {
    // Destroy all the windows and terminate
    glfwTerminate();
  }

  void SwapAndPoll( std::shared_ptr<Window> window ) {

  }

  void SwapAndPoll() {
    SwapAndPoll( GetMainWindow() );
  }


protected:
  std::shared_ptr<Window> mMainWindow;
};


#endif // DEV_WINDOWMANAGER_H
