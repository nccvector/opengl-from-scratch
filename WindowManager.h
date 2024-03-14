//
// Created by vector on 24/03/10.
//

#ifndef DEV_WINDOWMANAGER_H
#define DEV_WINDOWMANAGER_H

#include <memory>

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
  WindowManager();

  void SetMainWindowData( const char* name, int width, int height );

  inline std::shared_ptr<Window> GetMainWindow() {
    return mMainWindow;
  }

  inline bool ShouldQuit() {
    return glfwWindowShouldClose( GetMainWindow()->pointer );
  }

  void InitializeMainWindow();
  void Terminate();

  void ProcessInput();
  void ProcessInput( std::shared_ptr<Window> window );

  void SwapAndPoll();
  void SwapAndPoll( std::shared_ptr<Window> window );

protected:
  std::shared_ptr<Window> mMainWindow;
};


#endif // DEV_WINDOWMANAGER_H
