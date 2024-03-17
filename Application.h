//
// Created by vector on 24/03/10.
//

#ifndef DEV_APPLICATION_H
#define DEV_APPLICATION_H

// IMPORTANT: This must be imported before glfw, which is inside WindowManager.h
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

#include "ResourceManager.h"
#include "Camera.h"

#include "Common.h"
#include "Viewport.h"


class Application {
public:
  Application( Application const& )    = delete;
  void operator=( Application const& ) = delete;

  static Application& GetSingleton() {
    // Constructor is only called the first time
    static Application app;

    return app;
  }

  static void ResizeCallback( GLFWwindow* window, int width, int height ) {
    // make sure the viewport matches the new window dimensions; note that width
    // and height will be significantly larger than specified on retina displays.
    glfwGetFramebufferSize( window, &width, &height );

    glViewport( 0, 0, width, height );
  }

  static void KeyCallback( GLFWwindow* window, int key, int scancode, int action, int mods ) {
    DEBUG( "Key pressed: {}", key );

    if ( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS ) {
      glfwSetWindowShouldClose( window, true );
    }
  }

  void Run() {
    InitializeTime();

    // Render frames in loop
    while ( !glfwWindowShouldClose( mWindow ) ) {
      // input
      //      mWindowManager.ProcessInput();

      // Update scene graph
      // Camera rotation around world up (y-axis)
      float speed            = 2.0f;
      float angle            = speed * mTimeSinceStart / 1000.0f;
      float height           = 0.0f;
      float distance         = 400.0 + 1.0f * glm::sin( mTimeSinceStart / 100.0f );
      glm::mat4 rotated      = glm::rotate( glm::mat4( 1 ), angle, { 0, 1, 0 } );
      glm::mat4 newTransform = glm::translate( rotated, { 0, height, distance } );

      mCamera->SetTransform( newTransform );

      // render
      // ------
      // Set viewport as draw target
      mViewport->SetAsDrawTarget(); // The subsequent draw calls will draw to this viewport
      glClearColor(0, 0, 0, 1);
      glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); // clear frame
      RenderOneFrame();

      // Bind default framebuffer
      glBindFramebuffer( GL_FRAMEBUFFER, 0 );
      glClearColor(0, 0, 0, 1);
      glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); // clear frame
      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();

      ImGui::NewFrame();
      ImGui::ShowDemoWindow();

      // Paint the Viewport with GUI
      mViewport->DrawGui();

      RenderGui();

      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData() );

      // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved
      glfwSwapBuffers( mWindow );
      glfwPollEvents();

      // Update time
      mTimeSinceStart = std::chrono::duration_cast<Milliseconds>( Clock::now() - mStartTime ).count();
    }
  }

private:
  Application() {
    InitializeWindow();
    InitializeGL();
    InitializeGUI();

    // Create a camera to render the scene
    mCamera = std::make_shared<Camera>( 65.0f, (float) mWindowWidth / (float) mWindowHeight, 0.001, 1000.0 );
    mCamera->SetTransform( glm::translate( glm::mat4( 1.0f ), glm::vec3( 0.0f, 0.0f, 5.0f ) ) );

    // Create viewport here
    // Create render framebuffer
    mViewport = std::make_shared<Viewport>( "GL Viewport", mWindowWidth, mWindowHeight );
    mViewport->SetCamera(mCamera);  // Will help adjust camera projection matrix based on viewport resize

    // Set callbacks
    glfwSetFramebufferSizeCallback( mWindow, Application::ResizeCallback );
    glfwSetKeyCallback( mWindow, Application::KeyCallback );

    ResourceManager::InitializeShaders();


  }

  ~Application() {
    // Destroy GUI
    DEBUG( "Destroying GUI" );
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // Destroy GL
    // ...
    DEBUG( "Destroying GL" );

    // Destroy window
    DEBUG( "Destroying Window" );
    glfwTerminate();
  }

  bool InitializeWindow( const char* name = "Application", int width = 1280, int height = 720 ) {
    mWindowWidth  = width;
    mWindowHeight = height;

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
    mWindow = glfwCreateWindow( width, height, name, nullptr, nullptr );

    if ( mWindow == nullptr ) {
      ERROR( "Could not create glfw window" );
      glfwTerminate();
    }

    glfwMakeContextCurrent( mWindow ); // std::shared_ptr.get() returns a raw pointer
  }

  bool InitializeGL() {
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if ( !gladLoadGLLoader( (GLADloadproc) glfwGetProcAddress ) ) {
      ERROR( "Could not load GLAD" );
      return false;
    }

    // Configure opengl depth settings
    glClearColor( 0.025f, 0.015f, 0.015f, 1.0f );
    glEnable( GL_DEPTH_TEST ); // ENABLE DEPTH
                               //    glEnable( GL_SCISSOR_TEST );  // Causes rectangle scissor on glfw window
    glEnable( GL_BLEND );
    glDepthFunc( GL_LEQUAL );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    // Cull settings
    glEnable( GL_CULL_FACE );
    glCullFace( GL_BACK );
    glFrontFace( GL_CCW );

    return true;
  }

  void InitializeGUI() {
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL( mWindow, true );
    ImGui_ImplOpenGL3_Init( "#version 330" );
  }

  void RenderOneFrame() {
    // Create models on device
    for ( std::shared_ptr<Model> model : ResourceManager::GetResourceList<Model>() ) {
      // Activate shader
      ResourceManager::EnsureShaderActiveState( ResourceManager::defaultShader );

      // Set Model View Projection
      glm::mat4 modelViewProjection = mCamera->GetViewMatrix() * model->GetTransform();
      glUniformMatrix4fv( glGetUniformLocation( ResourceManager::defaultShader->GetProgram(), "modelViewProjection" ),
          1, GL_FALSE, glm::value_ptr( modelViewProjection ) );
      glUniform1f(
          glGetUniformLocation( ResourceManager::defaultShader->GetProgram(), "time" ), (float) mTimeSinceStart );

      model->Draw();
    }
  }

  void RenderGui() {
    RenderGuiSceneSelector();
  }

  void RenderGuiSceneSelector() {
    ImGui::Begin( "Scene selector" );

    const char* items[] = { "resources/primitives/primitive-sphere.fbx", "resources/primitives/primitive-cube.fbx",
        "resources/stanford-bunny.fbx", "resources/Sponza2/Sponza.fbx" };

    static const char* selected = items[2];

    if ( ImGui::BeginCombo( "Scene", selected ) ) {
      for ( int n = 0; n < IM_ARRAYSIZE( items ); n++ ) {
        if ( ImGui::Selectable( items[n], items[n] == selected ) ) {
          selected = items[n];
          ImGui::SetItemDefaultFocus();
        }
      }
      ImGui::EndCombo();
    }

    static bool startPressed = true;  // Used for loading the currently selected scene for the first time without even pressing the button
    if ( ImGui::Button( "Load" ) || startPressed ) {
      startPressed = false;
      UnloadCurrentScene();
      LoadScene( selected );
    }

    ImGui::End();
  }

  void InitializeTime() {
    // Initialize timers
    mStartTime      = Clock::now();
    mTimeSinceStart = std::chrono::duration_cast<Milliseconds>( Clock::now() - mStartTime ).count();
  }

  void UnloadCurrentScene() {
    ResourceManager::textures  = {};
    ResourceManager::materials = {};
    ResourceManager::meshes    = {};
    ResourceManager::models    = {};
  }

  void LoadScene( const char* sceneFilePath ) {
    // Scene to load from file
    FbxScene* lScene;

    bool lResult = ResourceManager::LoadScene( sceneFilePath, lScene );

    DEBUG( "Objects in scene: {}", lScene->GetSrcObjectCount<FbxNode>() );
    DEBUG( "Textures in scene: {}", lScene->GetSrcObjectCount<FbxFileTexture>() );
    DEBUG( "Meshes in scene: {}", lScene->GetSrcObjectCount<FbxMesh>() );
    DEBUG( "Lights in scene: {}", lScene->GetSrcObjectCount<FbxLight>() );

    // Loading textures from scene
    for ( int i = 0; i < lScene->GetSrcObjectCount<FbxFileTexture>(); i++ ) {
      FbxFileTexture* texture = lScene->GetSrcObject<FbxFileTexture>( i );
      DEBUG( "Loading texture: {}", texture->GetFileName() );

      // Create texture
      std::shared_ptr<Texture> newTexture = std::make_shared<Texture>( texture->GetName(), texture->GetFileName() );

      // Check success
      if ( !newTexture->IsCreatedOnDevice() ) {
        continue;
      }

      // Register in resource manager
      ResourceManager::AddResource<Texture>( newTexture );
    }

    // Loading materials from scene
    for ( int i = 0; i < lScene->GetSrcObjectCount<FbxSurfaceMaterial>(); i++ ) {
      FbxSurfaceMaterial* material = lScene->GetSrcObject<FbxSurfaceMaterial>( i );

      DEBUG( "Loading material: ", material->GetName() );

      // Create material
      std::shared_ptr<Material> newMaterial = std::make_shared<Material>( material->GetName() );
      newMaterial->CreateFromFbxSurfaceMaterial( material );

      // Register in resource manager
      ResourceManager::AddResource<Material>( newMaterial );
    }

    // Create models
    for ( int oIdx = 0; oIdx < lScene->GetSrcObjectCount<FbxNode>(); oIdx++ ) {
      FbxNode* node = lScene->GetSrcObject<FbxNode>( oIdx );

      // Skip if no mesh
      if ( !node->GetMesh() ) {
        continue;
      }

      // Material for new mesh
      std::shared_ptr<Material> mat = nullptr;
      for ( const std::shared_ptr<Material>& testMaterial : ResourceManager::GetResourceList<Material>() ) {
        if ( strcmp( testMaterial->GetName(), node->GetMaterial( 0 )->GetName() ) == 0 ) {
          mat = testMaterial;
          break;
        }
      }

      // Create new material if it does not exist
      if ( mat == nullptr ) {
        std::shared_ptr<Material> newMaterial = std::make_shared<Material>( node->GetMaterial( 0 )->GetName() );
        newMaterial->CreateFromFbxSurfaceMaterial( node->GetMaterial( 0 ) );
      }

      // Create mesh for this model
      std::shared_ptr<Mesh> newMesh = std::make_shared<Mesh>( node->GetName(), mat );
      newMesh->CreateFromFbxMesh( node->GetMesh() );

      // Creating model
      std::shared_ptr<Model> newModel = std::make_shared<Model>( node->GetName(), newMesh );

      // Register in resource manager
      ResourceManager::AddResource<Mesh>( newMesh );
      ResourceManager::AddResource<Model>( newModel );
    }
  }

protected:
  // Window
  GLFWwindow* mWindow;
  int mWindowWidth, mWindowHeight;

  // Camera
  std::shared_ptr<Camera> mCamera;

  // Viewport
  std::shared_ptr<Viewport> mViewport;

  // Time
  std::chrono::time_point<std::chrono::_V2::system_clock, Nanoseconds> mStartTime;
  double mTimeSinceStart;
};


#endif // DEV_APPLICATION_H
