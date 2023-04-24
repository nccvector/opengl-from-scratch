//
// Created by vector on 23/04/17.
//
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// GLM
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <optional>

#include "Shader.h"
#include "Texture.h"

struct temp {
  std::optional<uint32_t> var;

  bool complete() {
    return var.has_value();
  }
};

float vertices[] = {
    // positions
    0.5f, 0.5f, 0.0f,   // top right vertex
    1.0, 0.0, 0.0,      // color
    0.75, 0.75,         // uv
    0.5f, -0.5f, 0.0f,  // bottom right vertex
    0.0, 1.0, 0.0,      // color
    0.75, 0.25,         // uv
    -0.5f, -0.5f, 0.0f, // bottom left vertex
    0.0, 0.0, 1.0,      // color
    0.25, 0.25,         // uv
    -0.5f, 0.5f, 0.0f,  // top left vertex
    1.0, 0.0, 0.0,      // color
    0.25, 0.75          // uv
};

unsigned int indices[] = {
    // note that we start from 0!
    0, 3, 1, // first triangle
    1, 3, 2  // second triangle
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

    // Create a texture
    Texture texture( "textures/UVMap.png" );

    // TODO: Raise error that pointer is null and continue, dont let the application crash!
    mShader->setTexture( "Texture", texture.getId() );

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
        8 * sizeof( float ),        // Stride size in bytes
        (void*) 0 );                // offset: Where the position data begins in the buffer
    glEnableVertexAttribArray( 0 ); // Enable this attribute array
    // Create one more for color data layout
    glVertexAttribPointer( 1,              // Corresponds to the location = 0 in vertex shader
        3,                                 // Entries per vertex data (r g and b)
        GL_FLOAT,                          // Type of entry
        GL_FALSE,                          // Specifies whether the data should be normalized or not
        8 * sizeof( float ),               // Stride size in bytes
        (void*) ( 3 * sizeof( float ) ) ); // offset: Where the position data begins in the buffer
    glEnableVertexAttribArray( 1 );        // Enable this attribute array
    // Create one more for uv data layout
    glVertexAttribPointer( 2,              // Corresponds to the location = 0 in vertex shader
        2,                                 // Entries per vertex data (u and v)
        GL_FLOAT,                          // Type of entry
        GL_FALSE,                          // Specifies whether the data should be normalized or not
        8 * sizeof( float ),               // Stride size in bytes
        (void*) ( 6 * sizeof( float ) ) ); // offset: Where the position data begins in the buffer
    glEnableVertexAttribArray( 2 );        // Enable this attribute array

    // 5. Draw
    if ( mShader ) {
      mShader->use();
    }
    // else throw an error

    // Culling options
    glEnable( GL_CULL_FACE );
    glCullFace( GL_BACK );
    glFrontFace( GL_CCW );

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
    float timeLastFrame    = glfwGetTime();
    float timeCurrentFrame = 0.0f;
    float deltaTime        = 0.0f;

    float degPerSec   = 10.0f;
    float newRotation = 0.0f;

    while ( !glfwWindowShouldClose( mWindow ) ) {
      timeCurrentFrame = glfwGetTime();
      deltaTime        = timeCurrentFrame - timeLastFrame;

      // Inputs
      processInputs( mWindow );

      // Physics

      // Rendering
      // Painting background
      glClearColor( 0.1f, 0.1f, 0.1f, 1.0f );
      glClear( GL_COLOR_BUFFER_BIT );

      // Applying scene transforms
      newRotation += degPerSec * deltaTime;
      glm::mat4 model = glm::mat4( 1.0f );
      model           = glm::rotate( model, glm::radians( 0.0f ), glm::vec3( 1.0f, 0.0f, 0.0f ) );

      // Creating camera vector
      glm::mat4 camera = glm::mat4( 1.0f );
      glm::vec3 cameraPosition( 0.0f, 3.0f, 3.0f );
      glm::vec3 lookAt( 0, 0, 0 );
      glm::vec3 upVector( 0, 1, 0 );

      // Provide the positions whereever you want to look in the scene, but inverse the transform because camera looks
      // in the opposite z Not taking inverse, because we already use camera inverse matrix in the shader to transform
      // all the points in camera frame. So taking inverse twice is very inefficient...skipping inverse transform.
      camera = glm::lookAt( cameraPosition, lookAt, upVector );

      // Applying render transforms
      glm::mat4 projection =
          glm::perspective( glm::radians( 45.0f ), (float) mWidth / (float) mHeight, 0.0001f, 100000.0f );

      // Send transforms to the shader
      mShader->setModelViewProjectionMatrix( model, camera, projection );

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

      timeLastFrame = timeCurrentFrame;
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
