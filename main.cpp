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
#include "Light.h"
#include "Types.h"

// GLM
#include <glm/gtc/matrix_transform.hpp>

// assimp include files. These three are usually needed.
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <fstream>

class Application {
public:
  explicit Application( const char* title = "My Application", int width = 800, int height = 600 ) : mTitle( title ) {
    initWindow( width, height );
    initGL( width, height );
  }

  ~Application() {
    // TODO: Delete all the models before shaders
    delete mShader;
    glfwTerminate();
  }

  int initWindow( int width, int height ) {
    glfwInit();
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    mWindow = glfwCreateWindow( width, height, mTitle, nullptr, nullptr );
    if ( mWindow == nullptr ) {
      std::cout << "Failed to create GLFW window" << std::endl;
      glfwTerminate();
      return -1;
    }

    glfwMakeContextCurrent( mWindow );
    glfwSwapInterval( 0 );
    glfwSetFramebufferSizeCallback( mWindow, resizeCallback );

    return 0;
  }

  int initGL( int width, int height, bool enableBackfaceCulling = true, bool enableDepthTest = true ) {
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
    if ( enableBackfaceCulling ) {
      glEnable( GL_CULL_FACE );
      glCullFace( GL_BACK );
    }
    if ( enableDepthTest ) {
      glEnable( GL_DEPTH_TEST );
      glFrontFace( GL_CCW );
    }
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

  void LoadModelsFromFile( const std::string& filename, float scale = 1.0f ) {
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
      return;
    }

    fin.close();

    g_scene = importer.ReadFile( filename, aiProcessPreset_TargetRealtime_Quality );

    // If the import failed, report it
    if ( g_scene == nullptr ) {
      std::cout << importer.GetErrorString() << std::endl;
      return;
    }

    // Now we can access the file's contents.
    std::cout << "Import of scene " << filename << " succeeded." << std::endl;

    for ( unsigned int i = 0; i < g_scene->mNumMeshes; i++ ) {
      VertexList vlist;
      UIntList ilist;

      const aiMesh* mesh = g_scene->mMeshes[i];

      // Filling in the vertex data
      for ( unsigned int v = 0; v < mesh->mNumVertices; v++ ) {
        Vertex newVertex;

        aiVector3D aiVertex = mesh->mVertices[v];
        newVertex.Position  = glm::vec3( aiVertex.x, aiVertex.y, aiVertex.z );

        aiVector3D aiNormal = mesh->mNormals[v];
        newVertex.Normal    = glm::vec3( aiNormal.x, aiNormal.y, aiNormal.z );

        aiVector3t<float> aiTexCoord;
        if ( mesh->mTextureCoords[0] != nullptr ) {
          aiTexCoord         = mesh->mTextureCoords[0][v];
          newVertex.TexCoord = glm::vec2( aiTexCoord.x, aiTexCoord.y );
        } else {
          newVertex.TexCoord = glm::vec2( 0.0f );
        }

        vlist.push_back( newVertex );
      }

      // Filling in the indices data
      for ( unsigned int vf = 0; vf < mesh->mNumFaces; vf++ ) {
        aiFace face = mesh->mFaces[vf];
        for ( unsigned int vi = 0; vi < face.mNumIndices; vi++ ) {
          unsigned int aiIndex = face.mIndices[vi];
          ilist.push_back( aiIndex );
        }
      }

      // TODO: Consider deleting host side model data in case of memory shortage
      Model newModel = { vlist, ilist, glm::mat4( 1.0 ), mMaterials[0] };
      ModelTools::LoadOnDevice( newModel ); // load on device
      newModel.Transform = glm::scale( newModel.Transform, glm::vec3( scale ) );
      mModels.push_back( newModel );
    }
  }

  void run() {
    // Load textures
    Texture newTexture;
    TextureTools::LoadOnHost( "textures/UVMap.png", newTexture );
    TextureTools::LoadOnDevice( newTexture );
    TextureTools::FreeOnHost( newTexture._data );

    mTextures.push_back( newTexture );

    // Create a Material
    PhongMaterial material = { Color( 0.01 ), Color( 0.5 ), Color( 1.0 ), mTextures };

    mMaterials.push_back( material );

    // Create lights
    PointLight pointLight = {
        1.0,
        Color( 0.0f, 1.0f, 0.0f ),
        Vec3( 10.0f, 10.0f, 10.0f ),
    };
    mPointLights.push_back( pointLight );

    // Push some default textures and materials before calling this!
    LoadModelsFromFile( "models/bunny.obj", 5.0f );
    LoadModelsFromFile( "models/cube.obj", 0.5f );

    float timeCurrentFrame;
    float deltaTime;
    auto timeLastFrame = (float) glfwGetTime();
    while ( !glfwWindowShouldClose( mWindow ) ) {
      timeCurrentFrame = (float) glfwGetTime();
      deltaTime        = timeCurrentFrame - timeLastFrame;

      std::cout << "fps: " << 1 / deltaTime << std::endl;

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
        Model& model    = mModels[i];
        model.Transform = glm::rotate( model.Transform, angle, glm::vec3( 0, 1, 0 ) );
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
      int width, height;
      glfwGetWindowSize( mWindow, &width, &height );
      glm::mat4 projection =
          glm::perspective( glm::radians( 45.0f ), (float) width / (float) height, 0.0001f, 100000.0f );

      // Send view projection transforms to shader
      mShader->setModelViewProjectionMatrix( glm::mat4( 1.0 ), camera, projection );

      // Enable shader
      mShader->use();

      // Send light data to shader
      mShader->setPointLights( mPointLights );

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
  GLFWwindow* mWindow = nullptr;

  // Application vars
  Shader* mShader = nullptr; // Only one shader supported as of now
  std::vector<Texture> mTextures;
  std::vector<PhongMaterial> mMaterials;
  std::vector<Model> mModels;
  std::vector<PointLight> mPointLights;
};

int main() {

  Application app;
  app.run();

  std::cout << "Exit Success.\n";

  return 0;
}
