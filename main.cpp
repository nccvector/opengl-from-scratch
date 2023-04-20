//
// Created by vector on 23/04/17.
//
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

#include <optional>

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
    -0.5f, 0.5f, 0.0f, 0.0, 0.0, 0.0,  // top left
};

unsigned int indices[] = {
    // note that we start from 0!
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
};

std::string loadShaderString( const std::string& filepath ) {
  std::ostringstream stringStream;
  std::ifstream inputFileStream( filepath );
  stringStream << inputFileStream.rdbuf();

  std::string shaderString( stringStream.str() );
  return shaderString;
}

class Application {
public:
  explicit Application( const char* title = "My Application" ) : mTitle( title ) {
    initWindow();
    initGL( mWidth, mHeight );
  }

  ~Application() {
    glfwTerminate();

    // De-init OpenGL
    // glDeleteShader( mGLVertexShader );
    // glDeleteShader( mGLFragmentShader );
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

    // Create vertex shader
    const std::string& vertexShaderString = loadShaderString( "./shaders/vertex.glsl" );
    const char* vertexShaderSource        = vertexShaderString.c_str();

    mGLVertexShader = glCreateShader( GL_VERTEX_SHADER );
    glShaderSource( mGLVertexShader, 1, &vertexShaderSource, nullptr );
    glCompileShader( mGLVertexShader );

    // Shader validation
    int success;
    char infoLog[512];
    glGetShaderiv( mGLVertexShader, GL_COMPILE_STATUS, &success );
    if ( !success ) {
      glGetShaderInfoLog( mGLVertexShader, 512, nullptr, infoLog );
      std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Create fragment shader
    const std::string& fragmentShaderString = loadShaderString( "./shaders/fragment.glsl" );
    const char* fragmentShaderSource        = fragmentShaderString.c_str();

    mGLFragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
    glShaderSource( mGLFragmentShader, 1, &fragmentShaderSource, nullptr );
    glCompileShader( mGLFragmentShader );

    // Shader validation
    glGetShaderiv( mGLFragmentShader, GL_COMPILE_STATUS, &success );
    if ( !success ) {
      glGetShaderInfoLog( mGLFragmentShader, 512, nullptr, infoLog );
      std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Creating shader program
    // Shader program:
    //    A shader program object is the final linked version of multiple shaders combined. To
    //    use the recently compiled shaders we have to link them to a shader program object and then activate this
    //    shader program when rendering objects. The activated shader program's shaders will be used when we issue
    //    render calls. When linking the shaders into a program it links the outputs of each shader to the inputs of the
    //    next shader. This is also where you'll get linking errors if your outputs and inputs do not match.
    mGLProgram = glCreateProgram();
    glAttachShader( mGLProgram, mGLVertexShader );
    glAttachShader( mGLProgram, mGLFragmentShader );
    glLinkProgram( mGLProgram );

    // Program validation
    glGetProgramiv( mGLProgram, GL_LINK_STATUS, &success );
    if ( !success ) {
      glGetProgramInfoLog( mGLProgram, 512, nullptr, infoLog );
      std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

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
    glUseProgram( mGLProgram );
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
      //      int colorAttribLocation = glGetUniformLocation( mGLProgram, "color" );
      glUseProgram( mGLProgram );
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
  unsigned int mGLVertexShader;
  unsigned int mGLFragmentShader;
  unsigned int mGLProgram;
  unsigned int mGlVBO;
  unsigned int mGlVAO;
  unsigned int mGlEBO;
};

int main() {

  Application app;
  app.run();

  std::cout << "Exit Success.\n";

  return 0;
}
