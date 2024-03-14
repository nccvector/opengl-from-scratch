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
  ~Application() {
    DestroyGUI();
  }

  bool InitializeGL();

  inline void InitializeGUI() {
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL( mWindowManager.GetMainWindow()->pointer, false );
    ImGui_ImplOpenGL3_Init();
  }

  inline void DestroyGUI() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
  }

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
      // input
      mWindowManager.ProcessInput();

      // Update scene graph
      // Camera rotation around world up (y-axis)
      float speed            = 2.0f;
      float angle            = speed * mTimeSinceStart / 1000.0f;
      float height           = 0.0f;
      float distance         = 3.0 + 1.0f * glm::sin( mTimeSinceStart / 100.0f );
      glm::mat4 rotated      = glm::rotate( glm::mat4( 1 ), angle, { 0, 1, 0 } );
      glm::mat4 newTransform = glm::translate( rotated, { 0, height, distance } );

      mCamera->SetTransform( newTransform );

      // render
      // ------
      glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); // clear frame

      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();
      ImGui::ShowDemoWindow();

      RenderOneFrame();

      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData() );

      // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved
      mWindowManager.SwapAndPoll();

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
