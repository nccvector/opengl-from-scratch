//
// Created by vector on 23/04/17.
//
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Application {
public:
  explicit Application( const char* title = "My Application" ) : mTitle( title ) {
    initWindow();
    initGlad( mWidth, mHeight );
  }

  ~Application() {
    glfwTerminate();
  }

  int initWindow() {
    glfwInit();
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    mWindow = glfwCreateWindow( mWidth, mHeight, mTitle, nullptr, nullptr );
    if ( mWindow == nullptr ) {
      std::cout << "Failed to create GLFW window" << std::endl;
      glfwTerminate();
      return -1;
    }

    glfwMakeContextCurrent( mWindow );
    glfwSetFramebufferSizeCallback( mWindow, resizeCallback );

    return 0;
  }

  static int initGlad( int width, int height ) {
    if ( !gladLoadGLLoader( (GLADloadproc) glfwGetProcAddress ) ) {
      std::cout << "Failed to initialize GLAD" << std::endl;
      return -1;
    }
    glViewport( 0, 0, width, height );
    return 0;
  }

  static void resizeCallback( GLFWwindow* window, int width, int height ) {
    glViewport( 0, 0, width, height );
  }

  static void processInputs( GLFWwindow* window ) {
    if ( glfwGetKey( window, GLFW_KEY_ESCAPE ) == GLFW_PRESS ) {
      glfwSetWindowShouldClose( window, true );
    }
  }

  void run() {
    while ( !glfwWindowShouldClose( mWindow ) ) {
      // Inputs
      processInputs( mWindow );

      // Physics

      // Rendering
      glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      // Events and swap buffers
      glfwSwapBuffers( mWindow );
      glfwPollEvents();
    }
  }

private:
  // Glfw vars
  const char* mTitle;
  int mWidth          = 800;
  int mHeight         = 600;
  GLFWwindow* mWindow = nullptr;
};

int main() {

  Application app;
  app.run();

  std::cout << "I am alive\n";

  return 0;
}
