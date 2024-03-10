//
// Created by vector on 24/03/10.
//

#ifndef DEV_APPLICATION_H
#define DEV_APPLICATION_H

// IMPORTANT: This must be imported before glfw, which is inside WindowManager.h
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include "WindowManager.h"
#include "ResourceManager.h"
#include "Camera.h"

#include "Common.h"

class Application {
public:
  Application();

  bool InitializeGL();

  void RenderOneFrame();

  inline void InitializeTime() {
    // Initialize timers
    mStartTime      = Clock::now();
    mTimeSinceStart = std::chrono::duration_cast<Milliseconds>( Clock::now() - mStartTime ).count();
  }

  inline void Run() {
    InitializeTime();

    // Render frames in loop
    while ( !mWindowManager.ShouldQuit() ) {
      RenderOneFrame();

      // Update time
      mTimeSinceStart = std::chrono::duration_cast<Milliseconds>( Clock::now() - mStartTime ).count();
    }
  }

  void LoadScene();

  inline void Quit() {
    mWindowManager.Terminate();
  }

protected:
  WindowManager mWindowManager;
  std::shared_ptr<Camera> mCamera;

  std::chrono::time_point<std::chrono::_V2::system_clock, Nanoseconds> mStartTime;
  double mTimeSinceStart;
};


#endif // DEV_APPLICATION_H
