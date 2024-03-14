//
// Created by vector on 24/03/10.
//

#include "Application.h"
#include "Common.h"


Application::Application() {
  mWindowManager = WindowManager();
  mWindowManager.InitializeMainWindow();

  InitializeGL();

  InitializeGUI();

  ResourceManager::InitializeShaders();

  LoadScene();

  // Create a camera to render the scene
  mCamera = std::make_shared<Camera>( 65.0f,
      (float) mWindowManager.GetMainWindow()->width / (float) mWindowManager.GetMainWindow()->height, 0.001, 1000.0 );

  mCamera->SetTransform( glm::translate( glm::mat4( 1.0f ), glm::vec3( 0.0f, 0.0f, 5.0f ) ) );
}

bool Application::InitializeGL() {
  // glad: load all OpenGL function pointers
  // ---------------------------------------
  if ( !gladLoadGLLoader( (GLADloadproc) glfwGetProcAddress ) ) {
    ERROR( "Could not load GLAD" );
    return false;
  }

  // Configure opengl depth settings
  glClearColor( 0.025f, 0.015f, 0.015f, 1.0f );
  glEnable( GL_DEPTH_TEST ); // ENABLE DEPTH
  glEnable( GL_SCISSOR_TEST );
  glEnable( GL_BLEND );
  glDepthFunc( GL_LEQUAL );
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

  // Cull settings
  glEnable( GL_CULL_FACE );
  glCullFace( GL_BACK );
  glFrontFace( GL_CCW );

  return true;
}

void Application::RenderOneFrame() {

  // Create models on device
  for ( std::shared_ptr<Model> model : ResourceManager::GetResourceList<Model>() ) {
    // Activate shader
    ResourceManager::EnsureShaderActiveState( ResourceManager::defaultShader );

    // Set Model View Projection
    glm::mat4 modelViewProjection = mCamera->GetViewMatrix() * model->GetTransform();
    glUniformMatrix4fv( glGetUniformLocation( ResourceManager::defaultShader->GetProgram(), "modelViewProjection" ), 1,
        GL_FALSE, glm::value_ptr( modelViewProjection ) );
    glUniform1f(
        glGetUniformLocation( ResourceManager::defaultShader->GetProgram(), "time" ), (float) mTimeSinceStart );

    model->Draw();
  }


}

void Application::LoadScene() {
  // Scene to load from file
  FbxScene* lScene;

  // Load the scene.
  //  bool lResult = ResourceManager::LoadScene( "resources/primitives/primitive-sphere.fbx", lScene );
//  bool lResult = ResourceManager::LoadScene( "resources/primitives/primitive-cube.fbx", lScene );
      bool lResult = ResourceManager::LoadScene( "resources/stanford-bunny.fbx", lScene );
//    bool lResult = ResourceManager::LoadScene( "resources/Sponza2/Sponza.fbx", lScene );

  DEBUG("Objects in scene: {}", lScene->GetSrcObjectCount<FbxNode>());
  DEBUG("Textures in scene: {}", lScene->GetSrcObjectCount<FbxFileTexture>());
  DEBUG("Meshes in scene: {}", lScene->GetSrcObjectCount<FbxMesh>());
  DEBUG("Lights in scene: {}", lScene->GetSrcObjectCount<FbxLight>());

  // Loading textures from scene
  for ( int i = 0; i < lScene->GetSrcObjectCount<FbxFileTexture>(); i++ ) {
    FbxFileTexture* texture = lScene->GetSrcObject<FbxFileTexture>( i );
    DEBUG("Loading texture: {}", texture->GetFileName());

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

    DEBUG("Loading material: ", material->GetName());

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