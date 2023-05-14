//
// Created by vector on 23/04/17.
//

#include <iostream>
#include <memory>
#include <optional>

#include "cuBuffer.h"
#include <sutil/sutil.h>
#include <sutil/vec_math.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Model.h"
#include "PhongShader.h"
#include "Texture.h"
#include "Light.h"
#include "Types.h"
#include "FrameBuffer.h"

// GLM
#include <glm/gtc/matrix_transform.hpp>

// Importing imgui
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "optix/Scene.h"

class Application {
public:
  explicit Application( const char* title = "My Application", int width = 800, int height = 600 ) : mTitle( title ) {
    initWindow( width, height );
    initGL( width, height );

    // Initialize optixRenderTexture
    optixRenderTexture = { "Optix Render", TextureType::Diffuse, width, height, 3 };
    TextureTools::GenTextureOnDevice( optixRenderTexture );
  }

  ~Application() {
    // TODO: Delete all the models before shaders
    mPhongShader.release();

    // Imgui cleanup
    {
      // Cleanup
      ImGui_ImplOpenGL3_Shutdown();
      ImGui_ImplGlfw_Shutdown();
      ImGui::DestroyContext();
    }

    // Glfw cleanup
    glfwDestroyWindow( mWindow );
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

    // Setup Dear ImGui context
    {
      IMGUI_CHECKVERSION();
      ImGui::CreateContext();
      ImGuiIO& io = ImGui::GetIO();
      (void) io;
      io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
      io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

      // Setup Dear ImGui style
      ImGui::StyleColorsDark();
      // ImGui::StyleColorsLight();

      // Setup Platform/Renderer backends
      ImGui_ImplGlfw_InitForOpenGL( mWindow, true );
      ImGui_ImplOpenGL3_Init();

      // Load Fonts
      // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use
      // ImGui::PushFont()/PopFont() to select them.
      // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among
      // multiple.
      // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your
      // application (e.g. use an assertion, or display an error and quit).
      // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling
      // ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
      // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
      // - Read 'docs/FONTS.md' for more instructions and details.
      // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double
      // backslash \\ !
      // - Our Emscripten build process allows embedding fonts to be accessible at runtime from the "fonts/" folder. See
      // Makefile.emscripten for details.
      // io.Fonts->AddFontDefault();
      // io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
      // io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
      // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
      // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
      // ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr,
      // io.Fonts->GetGlyphRangesJapanese()); IM_ASSERT(font != nullptr);

      // Our state
      bool show_demo_window    = true;
      bool show_another_window = false;
      ImVec4 clear_color       = ImVec4( 0.45f, 0.55f, 0.60f, 1.00f );
    }

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

    mPhongShader = std::unique_ptr<PhongShader>( new PhongShader() );

    // ================================================================================

    // Query supported attributes
    int numAttributes;
    glGetIntegerv( GL_MAX_VERTEX_ATTRIBS, &numAttributes );
    std::cout << "Maximum nr of vertex attributes supported: " << numAttributes << std::endl;

    // Culling options
    if ( enableBackfaceCulling ) {
      glEnable( GL_CULL_FACE );
      glCullFace( GL_BACK );
      glFrontFace( GL_CCW );
    }
    if ( enableDepthTest ) {
      glEnable( GL_DEPTH_TEST );
    }

    // Initializing frame buffer and renderTexture
    mFramebuffer = std::unique_ptr<FrameBuffer>( new FrameBuffer() );

    // Initialize render texture
    mRenderTexture = { "Application Render", TextureType::Ambient, width, height, 4 };
    TextureTools::GenTextureOnDevice( mRenderTexture );
    mFramebuffer->addRenderTexture( mRenderTexture );
    assert( mFramebuffer->complete() );

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
    Texture newTexture;
    TextureTools::LoadOnHost( "textures/UVMap.png", newTexture );
    TextureTools::LoadOnDevice( newTexture );
    TextureTools::FreeOnHost( newTexture._data );

    mTextures.push_back( newTexture );

    // Create a Material
    Material material = { Color( 0.01 ), Color( 0.5 ), Color( 1.0 ), mTextures };

    mMaterials.push_back( material );

    // Create lights
    PointLight pointLight = {
        1.0,
        Color( 0.0f, 1.0f, 0.0f ),
        Vec3( 10.0f, 10.0f, 10.0f ),
    };
    mPointLights.push_back( pointLight );

    // Push some default textures and materials before calling this!
    ModelTools::LoadModelsFromFile( "models/bunny.obj", mModels, mMaterials, 5.0f );
    ModelTools::LoadModelsFromFile( "models/cube.obj", mModels, mMaterials, 0.5f );

    float timeCurrentFrame;
    float deltaTime;
    auto timeLastFrame = (float) glfwGetTime();
    while ( !glfwWindowShouldClose( mWindow ) ) {
      int width, height;
      glfwGetWindowSize( mWindow, &width, &height );

      //      // Get pixels from optix
      //      int2 fbSize = make_int2( width, height );
      //      scene.resize( fbSize );
      //      pixels.resize( fbSize.x * fbSize.y );
      //      scene.render();
      //      scene.download_pixels( pixels.data() );

      // Update optixRenderTexture from pixel data
      optixRenderTexture._data = (unsigned char*) pixels.data();
      TextureTools::UpdateTextureData( optixRenderTexture );
      std::cout << "Optix Render Texture ID: " << optixRenderTexture.GLID << std::endl;

      timeCurrentFrame = (float) glfwGetTime();
      deltaTime        = timeCurrentFrame - timeLastFrame;

      std::cout << "fps: " << 1 / deltaTime << std::endl;

      // Inputs
      glfwPollEvents();
      processInputs( mWindow );

      // Physics

      // Rendering


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

      // Bind render frame buffer before drawing scene
      // Render to our framebuffer
      mFramebuffer->bind();
      glViewport(
          0, 0, width, height ); // Render on the whole framebuffer, complete from the lower left corner to the upp

      // Clear background
      glClearColor( 0.1f, 0.1f, 0.1f, 1.0f );
      glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

      // Applying render transforms
      glm::mat4 projection =
          glm::perspective( glm::radians( 45.0f ), (float) width / (float) height, 0.0001f, 100000.0f );

      // Send view projection transforms to shader
      mPhongShader->setModelViewProjectionMatrix( glm::mat4( 1.0 ), camera, projection );

      // Enable shader
      mPhongShader->use();

      // Send light data to shader
      mPhongShader->setPointLights( mPointLights );

      // Draw models
      for ( auto model : mModels ) {
        mPhongShader->draw( model );
      }

      // Draw imgui on default frame buffer
      glBindFramebuffer( GL_FRAMEBUFFER, 0 );
      glViewport( 0, 0, width, height ); // Render on the whole framebuffer, complete from the lower left corner to the

      // Clear background
      glClearColor( 1.0f, 0.1f, 0.1f, 1.0f );
      glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

      // Imgui frame init
      {
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        bool showDemoWindow = true;
        ImGui::ShowDemoWindow( &showDemoWindow );

        // Render view
        ImGui::Begin( "Viewport" );
        ImGui::Image( (void*) (intptr_t) mRenderTexture.GLID, ImVec2( 800, 600 ), ImVec2( 0, 1 ), ImVec2( 1, 0 ) );
        ImGui::End();
        //
        //        // Optix render view
        //        ImGui::Begin( "Optix" );
        //        ImGui::Image( (void*) (intptr_t) optixRenderTexture.GLID, ImVec2( 800, 600 ) );
        //        ImGui::End();
      }

      // Drawing imgui
      {
        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize( mWindow, &display_w, &display_h );
        glViewport( 0, 0, display_w, display_h );
        ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData() );
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
  std::unique_ptr<PhongShader> mPhongShader; // Only one shader supported as of now
  std::vector<Texture> mTextures;
  std::vector<Material> mMaterials;
  std::vector<Model> mModels;
  std::vector<PointLight> mPointLights;

  std::unique_ptr<FrameBuffer> mFramebuffer;
  Texture mRenderTexture;

  // optix vars
  std::vector<uint32_t> pixels;
  optix7tutorial::Scene scene;
  Texture optixRenderTexture;
};

int main() {

  Application app;
  app.run();

  std::cout << "Exit Success.\n";

  return 0;
}
