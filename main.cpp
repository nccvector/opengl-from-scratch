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
    initGlad();
  }

  int initWindow() {
    glfwInit();
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    mWindow = glfwCreateWindow( 800, 600, mTitle, NULL, NULL );
    if ( mWindow == NULL ) {
      std::cout << "Failed to create GLFW window" << std::endl;
      glfwTerminate();
      return -1;
    }
    glfwMakeContextCurrent( mWindow );
  }

  int initGlad() {
    if ( !gladLoadGLLoader( (GLADloadproc) glfwGetProcAddress ) ) {
      std::cout << "Failed to initialize GLAD" << std::endl;
      return -1;
    }
  }


private:
  const char* mTitle;
  GLFWwindow* mWindow;
};

int main() {

  Application app;

  std::cout << "I am alive\n";

  return 0;
}
