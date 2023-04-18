//
// Created by vector on 23/04/17.
//
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// Loading shaders
#include <string>
#include <sstream>
#include <fstream>

float vertices[] = {
    -0.5f, -0.5f, 0.0f, // x y z
    0.5f, -0.5f, 0.0f,  // x y z
    0.0f, 0.5f, 0.0f    // x y z
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
    initGlad( mWidth, mHeight );
  }

  ~Application() {
    glfwTerminate();

    // De-init OpenGL
    //glDeleteShader( mGLVertexShader );
    //glDeleteShader( mGLFragmentShader );
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

  int initGlad( int width, int height ) {
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

    // Create a vertex buffer
    glGenBuffers( 1, &mGlVBO );

    // Create a vertex array object
    glGenVertexArrays( 1, &mGlVAO);

    // 1. Bind VAO
    glBindVertexArray( mGlVAO );
    // 2. Bind VBO and copy data
    glBindBuffer( GL_ARRAY_BUFFER, mGlVBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );
    // 3. Linking vertex attributes (how the data is layed out in memory)
    // OpenGL does not know how the vertices are layed out in memory
    // To let the OpenGL know how to connect the vertices, we need to specify
    glVertexAttribPointer( 0, // Corresponds to the location = 0 in vertex shader
        3,                    // Entries per vertex data (x y and z)
        GL_FLOAT,             // Type of entry
        GL_FALSE,             // Specifies whether the data should be normalized or not
        3 * sizeof( float ),  // Stride size in bytes
        (void*) 0 );          // Where the position data begins in the buffer
    glEnableVertexAttribArray( 0 );

    // 4. Draw
    glUseProgram( mGLProgram );
    glBindVertexArray( mGlVAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

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
      glClearColor( 1.0f, 0.0f, 1.0f, 1.0f );
      glClear( GL_COLOR_BUFFER_BIT );

      // 4. Draw
      glUseProgram( mGLProgram );
      glBindVertexArray( mGlVAO);
      glDrawArrays(GL_TRIANGLES, 0, 3);

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

  // OpenGL vars
  unsigned int mGLVertexShader;
  unsigned int mGLFragmentShader;
  unsigned int mGLProgram;
  unsigned int mGlVBO;
  unsigned int mGlVAO;
};

int main() {

  Application app;
  app.run();

  std::cout << "I am alive\n";

  return 0;
}
