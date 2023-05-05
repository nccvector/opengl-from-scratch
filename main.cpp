//
// Created by vector on 23/04/17.
//

#include <iostream>
#include <optional>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Model.h"
#include "Shader.h"
#include "Texture.h"
#include "Types.h"

// GLM
#include <glm/gtc/matrix_transform.hpp>

// assimp include files. These three are usually needed.
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <fstream>

const aiScene* Import3DFromFile( const std::string& filename ) {
  // the global Assimp scene object
  const aiScene* g_scene = nullptr;

  // Create an instance of the Importer class
  Assimp::Importer importer;

  // Check if file exists
  std::ifstream fin( filename.c_str() );
  if ( fin.fail() ) {
    std::string message = "Couldn't open file: " + filename;
    std::wstring targetMessage;
    // utf8::utf8to16(message.c_str(), message.c_str() + message.size(), targetMessage);
    std::cout << importer.GetErrorString() << std::endl;
    return nullptr;
  }

  fin.close();

  g_scene = importer.ReadFile( filename, aiProcessPreset_TargetRealtime_Quality );

  // If the import failed, report it
  if ( g_scene == nullptr ) {
    std::cout << importer.GetErrorString() << std::endl;
    return nullptr;
  }

  // Now we can access the file's contents.
  std::cout << "Import of scene " << filename << " succeeded." << std::endl;

  // We're done. Everything will be cleaned up by the importer destructor
  return g_scene;
}

VertexList vertices = {
    Vertex { Vec3( 0.5f, 0.5f, 0.0f ), Vec3( 0.0, 0.0, 1.0 ), Vec2( 0.75, 0.75 ) },   // top right
    Vertex { Vec3( 0.5f, -0.5f, 0.0f ), Vec3( 0.0, 0.0, 1.0 ), Vec2( 0.75, 0.25 ) },  // bottom right
    Vertex { Vec3( -0.5f, -0.5f, 0.0f ), Vec3( 0.0, 0.0, 1.0 ), Vec2( 0.25, 0.25 ) }, // bottom left
    Vertex { Vec3( -0.5f, 0.5f, 0.0f ), Vec3( 0.0, 0.0, 1.0 ), Vec2( 0.25, 0.75 ) },  // top left
};

UIntList indices = {
    0, 3, 1, // first triangle
    1, 3, 2  // second triangle
};

class Application {
public:
  explicit Application( const char* title = "My Application" ) : mTitle( title ), mWidth( 800 ), mHeight( 600 ) {
    initWindow();
    initGL( mWidth, mHeight );
  }

  ~Application() {
    // TODO: Delete all the models before shaders
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
    // Loading, Compiling and creating shaders (Keep the shaders application level)
    // ================================================================================

    mShader = new Shader( "./shaders/vertex.glsl", "./shaders/fragment.glsl" );

    // ================================================================================

    // Query supported attributes
    int numAttributes;
    glGetIntegerv( GL_MAX_VERTEX_ATTRIBS, &numAttributes );
    std::cout << "Maximum nr of vertex attributes supported: " << numAttributes << std::endl;

    // Culling options
    // glEnable( GL_CULL_FACE );
    // glCullFace( GL_BACK );
    glEnable( GL_DEPTH_TEST );
    glFrontFace( GL_CCW );

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
    // Load textures
    Texture newTexture( "textures/UVMap.png" );
    mTextures.push_back( newTexture );

    // Create a Material
    PhongMaterial material( mTextures );
    mMaterials.push_back( material );

    // Create models and assign material
    for ( int i = 0; i < 4; i++ ) {
      Model model(vertices, indices, material);
      mModels.push_back( model );
    }

    float timeCurrentFrame;
    float deltaTime;
    auto timeLastFrame = (float) glfwGetTime();
    while ( !glfwWindowShouldClose( mWindow ) ) {
      timeCurrentFrame = (float) glfwGetTime();
      deltaTime        = timeCurrentFrame - timeLastFrame;

      // Inputs
      processInputs( mWindow );

      // Physics

      // Rendering
      // Clear background
      glClearColor( 0.1f, 0.1f, 0.1f, 1.0f );
      glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

      // Update Models
      float angle = timeCurrentFrame / 1000.0f;

      for ( int i = 0; i < mModels.size(); i++ ) {
        Model& model             = mModels[i];
        glm::mat4 modelTransform = model.getTransform();
        modelTransform           = glm::rotate( modelTransform, angle * ( i + 1 ), glm::vec3( 0, 1, 0 ) );
        model.setTransform( modelTransform );
      }

      // Update View TODO: view can also be a model
      glm::mat4 camera = glm::mat4( 1.0f );
      glm::vec3 cameraPosition( 3.0f, 3.0f, 3.0f );
      glm::vec3 lookAt( 0, 0, 0 );
      glm::vec3 upVector( 0, 1, 0 );

      // Provide the positions wherever you want to look in the scene, but inverse the transform because camera looks
      // in the opposite z Not taking inverse, because we already use camera inverse matrix in the shader to transform
      // all the points in camera frame. So taking inverse twice is very inefficient...skipping inverse transform.
      camera = glm::lookAt( cameraPosition, lookAt, upVector );

      // Applying render transforms
      glm::mat4 projection =
          glm::perspective( glm::radians( 45.0f ), (float) mWidth / (float) mHeight, 0.0001f, 100000.0f );

      // Send view projection transforms to shader
      mShader->setModelViewProjectionMatrix( glm::mat4( 1.0 ), camera, projection );

      // Enable shader
      mShader->use();

      // Draw models
      for ( auto model : mModels ) {
        mShader->draw( model );
      }

      // Swap buffers and poll I/O events
      glfwSwapBuffers( mWindow );
      glfwPollEvents();

      timeLastFrame = timeCurrentFrame;
    }
  }

private:
  // Glfw vars
  const char* mTitle;
  int mWidth;
  int mHeight;
  GLFWwindow* mWindow = nullptr;

  // Application vars
  Shader* mShader = nullptr; // Only one shader supported as of now
  std::vector<Texture> mTextures;
  std::vector<PhongMaterial> mMaterials;
  std::vector<Model> mModels;
};

int main() {
  Application app;
  app.run();

  std::cout << "Exit Success.\n";

  return 0;
}
