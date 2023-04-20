//
// Created by vector on 23/04/17.
//
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

#include <optional>

#include "Shader.h"

struct temp {
  std::optional<uint32_t> var;

  bool complete() {
    return var.has_value();
  }
};

float vertices[] = {
    // positions                      // Colors
    0.5f, 0.5f, 0.0f, 1.0, 0.0, 0.0,   // top right
    0.5f, -0.5f, 0.0f, 0.0, 1.0, 0.0,  // bottom right
    -0.5f, -0.5f, 0.0f, 0.0, 0.0, 1.0, // bottom left
    -0.5f, 0.5f, 0.0f, 1.0, 0.0, 0.0,  // top left
};

unsigned int indices[] = {
    // note that we start from 0!
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
};

class Application {
public:
  explicit Application( const char* title = "My Application" ) : mTitle( title ) {
    initWindow();
    initGL( mWidth, mHeight );
  }

  ~Application() {
    delete mShader;
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

  int initGL( int width, int height ) {
    if ( !gladLoadGLLoader( (GLADloadproc) glfwGetProcAddress ) ) {
      std::cout << "Failed to initialize GLAD" << std::endl;
      return -1;
    }
    glViewport( 0, 0, width, height );

    // ================================================================================
    // Loading, Compiling and creating shaders
    // ================================================================================

    mShader = new Shader( "./shaders/vertex.glsl", "./shaders/fragment.glsl" );

    // ================================================================================

    int numAttributes;
    glGetIntegerv( GL_MAX_VERTEX_ATTRIBS, &numAttributes );
    std::cout << "Maximum nr of vertex attributes supported: " << numAttributes << std::endl;

    // Create a vertex buffer
    glGenBuffers( 1, &mGlVBO );

    // Create a vertex array object
    glGenVertexArrays( 1, &mGlVAO );

    // Create an index buffer
    glGenBuffers( 1, &mGlEBO );

    // 1. Bind VAO
    glBindVertexArray( mGlVAO );
    // 2. Bind VBO and copy vertex data
    glBindBuffer( GL_ARRAY_BUFFER, mGlVBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );
    // 3. Bind EBO and copy index data
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mGlEBO );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( indices ), indices, GL_STATIC_DRAW );

    // 4. Linking vertex attributes (how the data is layed out in memory)
    // OpenGL does not know how the vertices are layed out in memory
    // To let the OpenGL know how to connect the vertices, we need to specify
    glVertexAttribPointer( 0,       // Corresponds to the location = 0 in vertex shader
        3,                          // Entries per vertex data (x y and z)
        GL_FLOAT,                   // Type of entry
        GL_FALSE,                   // Specifies whether the data should be normalized or not
        6 * sizeof( float ),        // Stride size in bytes
        (void*) 0 );                // offset: Where the position data begins in the buffer
    glEnableVertexAttribArray( 0 ); // Enable this attribute array
    // Create one more for color data layout
    glVertexAttribPointer( 1,              // Corresponds to the location = 0 in vertex shader
        3,                                 // Entries per vertex data (x y and z)
        GL_FLOAT,                          // Type of entry
        GL_FALSE,                          // Specifies whether the data should be normalized or not
        6 * sizeof( float ),               // Stride size in bytes
        (void*) ( 3 * sizeof( float ) ) ); // offset: Where the position data begins in the buffer
    glEnableVertexAttribArray( 1 );        // Enable this attribute array

    // 5. Draw
    mShader->use();
    glBindVertexArray( mGlVAO );
    glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );

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
      // Painting background
      glClearColor( 0.1f, 0.1f, 0.1f, 1.0f );
      glClear( GL_COLOR_BUFFER_BIT );

      // Draw call

      //      // Configure shader uniforms
      //      auto timeValue          = (float) glfwGetTime();
      //      float greenValue        = ( std::sin( timeValue ) / 2.0f ) + 0.5f;
      //      int colorAttribLocation = glGetUniformLocation( mShader->getId(), "color" );
      mShader->use();
      //      glUniform4f( colorAttribLocation, 0.0f, greenValue, 0.0f, 1.0f );

      // Draw VAOs
      glBindVertexArray( mGlVAO );
      glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );
      // Unbind VAOs
      glBindVertexArray( 0 ); // unbind

      // Swap buffers and poll I/O events
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

  // OpenGL vars

  unsigned int mGlVBO;
  unsigned int mGlVAO;
  unsigned int mGlEBO;

  //
  Shader* mShader = nullptr;
};

int main() {

  Application app;
  app.run();

  std::cout << "Exit Success.\n";

  return 0;
}
