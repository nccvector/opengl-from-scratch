#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <fbxsdk.h>

#include <iostream>

#include "common.h"
#include "utils.h"
#include "Shader.h"
#include "Camera.h"


// settings
const unsigned int SCR_WIDTH  = 1280;
const unsigned int SCR_HEIGHT = 720;
GLFWwindow* window;


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput( GLFWwindow* window ) {
  if ( glfwGetKey( window, GLFW_KEY_ESCAPE ) == GLFW_PRESS ) {
    glfwSetWindowShouldClose( window, true );
  }
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebufferSizeCallback( GLFWwindow* window, int width, int height ) {
  // make sure the viewport matches the new window dimensions; note that width and
  // height will be significantly larger than specified on retina displays.
  glViewport( 0, 0, width, height );
}


bool Init() {
  // glfw: initialize and configure
  // ------------------------------
  glfwInit();
  glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
  glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
  glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

#ifdef __APPLE__
  glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
#endif

  // glfw window creation
  // --------------------
  window = glfwCreateWindow( SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL );
  if ( window == NULL ) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent( window );
  glfwSetFramebufferSizeCallback( window, framebufferSizeCallback );

  // glad: load all OpenGL function pointers
  // ---------------------------------------
  if ( !gladLoadGLLoader( (GLADloadproc) glfwGetProcAddress ) ) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return false;
  }

  return true;
}


bool Quit() {
  // glfw: terminate, clearing all previously allocated GLFW resources.
  // ------------------------------------------------------------------
  glfwTerminate();
}


int main() {
  if ( !Init() ) {
    std::cerr << "Could not initialize\n";
    Quit();
    return -1;
  }

  // ================================================================================
  // Load scene and resources
  // ================================================================================
  // Scene to load from file
  FbxScene* lScene;

  // Load the scene.
  //    bool lResult = utils::LoadScene( "resources/stanford-bunny.fbx", lScene );
  bool lResult = utils::LoadScene( "resources/Sponza2/Sponza.fbx", lScene );

  std::cout << "Objects in scene: " << lScene->GetSrcObjectCount<FbxNode>() << std::endl;
  std::cout << "Meshes in scene: " << lScene->GetSrcObjectCount<FbxMesh>() << std::endl;
  std::cout << "Textures in scene: " << lScene->GetSrcObjectCount<FbxFileTexture>() << std::endl;
  std::cout << "Lights in scene: " << lScene->GetSrcObjectCount<FbxLight>() << std::endl;

  // Loading textures from scene
  for ( int i = 0; i < lScene->GetSrcObjectCount<FbxFileTexture>(); i++ ) {
    FbxFileTexture* texture = lScene->GetSrcObject<FbxFileTexture>( i );
    std::cout << "Loading texture: " << texture->GetFileName() << "\n";

    Texture* newTexture        = new Texture;
    const char* newTextureName = texture->GetName();
    texture->GetTypeName();
    FbxClassId id = texture->GetClassId();

    unsigned char* data;

    // Add to global textures if loaded successfully
    if ( ModelTools::CreateTextureFromPath( texture->GetFileName(), newTexture, newTextureName ) ) {
      global::textures.push_back( newTexture );
    }
  }

  // Create models on device
  std::vector<Model> models;
  for ( int oIdx = 0; oIdx < lScene->GetSrcObjectCount<FbxNode>(); oIdx++ ) {
    FbxNode* node = lScene->GetSrcObject<FbxNode>( oIdx );

    if ( !node->GetMesh() ) {
      continue;
    }

    Model newModel;
    ModelTools::CreateModelFromFbxNode( node, newModel );

    models.push_back( newModel );
  }

  // Create, load and compile shaders
  PhongShader phongShader;
  // ================================================================================

  // Create a camera to render the scene
  Camera camera( 65.0f, (float) SCR_WIDTH / (float) SCR_HEIGHT, 0.0001, 10000.0 );
  camera.SetTransform( glm::translate( glm::mat4( 1.0f ), glm::vec3( 0.0f, 0.0f, 500.0f ) ) );

  // render loop
  // -----------
  auto startTime        = Clock::now();
  double timeSinceStart = std::chrono::duration_cast<Milliseconds>( Clock::now() - startTime ).count();

  // Configure opengl depth settings
  glClearColor( 0.05f, 0.03f, 0.03f, 1.0f );
  glEnable( GL_DEPTH_TEST ); // ENABLE DEPTH
  glEnable( GL_SCISSOR_TEST );
  glDepthFunc( GL_LEQUAL );
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

  glEnable( GL_CULL_FACE );
  glCullFace( GL_BACK );
  glFrontFace( GL_CCW );
  while ( !glfwWindowShouldClose( window ) ) {
    // input
    // -----
    processInput( window );

    // Camera rotation around world up (y-axis)
    float angle            = timeSinceStart / 10000.0f;
    float height           = 100.0f;
    float distance         = 300.0f;
    glm::mat4 rotated      = glm::rotate( glm::mat4( 1 ), angle, { 0, 1, 0 } );
    glm::mat4 newTransform = glm::translate( rotated, { 0, height, distance } );

    camera.SetTransform( newTransform );

    // render
    // ------
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // Create models on device
    for ( auto model : models ) {
      phongShader.Draw( &camera, &model );
    }

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers( window );
    glfwPollEvents();

    // Update time
    timeSinceStart = std::chrono::duration_cast<Milliseconds>( Clock::now() - startTime ).count();
  }

  return Quit();
}
