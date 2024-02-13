#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <fbxsdk.h>

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "common.h"
#include "utils.h"
#include "Shader.h"
#include "Camera.h"


// settings
const unsigned int SCR_WIDTH  = 800;
const unsigned int SCR_HEIGHT = 600;


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


int main() {

  // Scene to load from file
  FbxScene* lScene;

  // Load the scene.
  bool lResult = utils::LoadScene( "resources/box.fbx", lScene );

  std::cout << "Lights in scene: " << lScene->GetSrcObjectCount<FbxLight>() << std::endl;
  std::cout << "Objects in scene: " << lScene->GetSrcObjectCount<FbxNode>() << std::endl;

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
  GLFWwindow* window = glfwCreateWindow( SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL );
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
    return -1;
  }

  // Create, load and compile shaders
  PhongShader phongShader;

  std::vector<Model> models;

  // Create models on device
  for ( int oIdx = 0; oIdx < lScene->GetSrcObjectCount<FbxNode>(); oIdx++ ) {
    FbxNode* node = lScene->GetSrcObject<FbxNode>( oIdx );

    if ( !node->GetMesh() ) {
      continue;
    }

    Model newModel;
    ModelTools::CreateModelFromFbxNode( node, newModel );
    ModelTools::LoadOnDevice( newModel );

    models.push_back(newModel);
  }

  // Create a camera to render the scene
  Camera camera( 65.0f, 1.3333f, 0.1, 100.0 );

  // render loop
  // -----------
  while ( !glfwWindowShouldClose( window ) ) {
    // input
    // -----
    processInput( window );

    // render
    // ------
    glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT );

    // Create models on device
    for ( auto model : models ) {
      phongShader.Draw( &camera, &model );
    }

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers( window );
    glfwPollEvents();
  }

  // glfw: terminate, clearing all previously allocated GLFW resources.
  // ------------------------------------------------------------------
  glfwTerminate();
  return 0;
}
