#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <fbxsdk.h>

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "common.h"
#include "utils.h"
#include "Shader.h"


#ifdef IOS_REF
#undef IOS_REF
#define IOS_REF ( *( pManager->GetIOSettings() ) )
#endif

void InitializeSdkObjects( FbxManager*& pManager, FbxScene*& pScene ) {
  // The first thing to do is to create the FBX Manager which is the object allocator for almost all the classes in the
  // SDK
  pManager = FbxManager::Create();
  if ( !pManager ) {
    FBXSDK_printf( "Error: Unable to create FBX Manager!\n" );
    exit( 1 );
  } else {
    FBXSDK_printf( "Autodesk FBX SDK version %s\n", pManager->GetVersion() );
  }

  // Create an IOSettings object. This object holds all import/export settings.
  FbxIOSettings* ios = FbxIOSettings::Create( pManager, IOSROOT );
  pManager->SetIOSettings( ios );

  // Load plugins from the executable directory (optional)
  FbxString lPath = FbxGetApplicationDirectory();
  pManager->LoadPluginsDirectory( lPath.Buffer() );

  // Create an FBX scene. This object holds most objects imported/exported from/to files.
  pScene = FbxScene::Create( pManager, "My Scene" );
  if ( !pScene ) {
    FBXSDK_printf( "Error: Unable to create FBX scene!\n" );
    exit( 1 );
  }
}


void DestroySdkObjects( FbxManager* pManager, bool pExitStatus ) {
  // Delete the FBX Manager. All the objects that have been allocated using the FBX Manager and that haven't been
  // explicitly destroyed are also automatically destroyed.
  if ( pManager ) {
    pManager->Destroy();
  }
  if ( pExitStatus ) {
    FBXSDK_printf( "Program Success!\n" );
  }
}


bool LoadScene( FbxManager* pManager, FbxDocument* pScene, const char* pFilename ) {
  int lFileMajor, lFileMinor, lFileRevision;
  int lSDKMajor, lSDKMinor, lSDKRevision;
  // int lFileFormat = -1;
  int lAnimStackCount;
  bool lStatus;
  char lPassword[1024];

  // Get the file version number generate by the FBX SDK.
  FbxManager::GetFileFormatVersion( lSDKMajor, lSDKMinor, lSDKRevision );

  // Create an importer.
  FbxImporter* lImporter = FbxImporter::Create( pManager, "" );

  // Initialize the importer by providing a filename.
  const bool lImportStatus = lImporter->Initialize( pFilename, -1, pManager->GetIOSettings() );
  lImporter->GetFileVersion( lFileMajor, lFileMinor, lFileRevision );

  if ( !lImportStatus ) {
    FbxString error = lImporter->GetStatus().GetErrorString();
    FBXSDK_printf( "Call to FbxImporter::Initialize() failed.\n" );
    FBXSDK_printf( "Error returned: %s\n\n", error.Buffer() );

    if ( lImporter->GetStatus().GetCode() == FbxStatus::eInvalidFileVersion ) {
      FBXSDK_printf( "FBX file format version for this FBX SDK is %d.%d.%d\n", lSDKMajor, lSDKMinor, lSDKRevision );
      FBXSDK_printf(
          "FBX file format version for file '%s' is %d.%d.%d\n\n", pFilename, lFileMajor, lFileMinor, lFileRevision );
    }

    return false;
  }

  FBXSDK_printf( "FBX file format version for this FBX SDK is %d.%d.%d\n", lSDKMajor, lSDKMinor, lSDKRevision );

  if ( lImporter->IsFBX() ) {
    FBXSDK_printf(
        "FBX file format version for file '%s' is %d.%d.%d\n\n", pFilename, lFileMajor, lFileMinor, lFileRevision );

    // From this point, it is possible to access animation stack information without
    // the expense of loading the entire file.

    FBXSDK_printf( "Animation Stack Information\n" );

    lAnimStackCount = lImporter->GetAnimStackCount();

    FBXSDK_printf( "    Number of Animation Stacks: %d\n", lAnimStackCount );
    FBXSDK_printf( "    Current Animation Stack: \"%s\"\n", lImporter->GetActiveAnimStackName().Buffer() );
    FBXSDK_printf( "\n" );

    for ( int i = 0; i < lAnimStackCount; i++ ) {
      FbxTakeInfo* lTakeInfo = lImporter->GetTakeInfo( i );

      FBXSDK_printf( "    Animation Stack %d\n", i );
      FBXSDK_printf( "         Name: \"%s\"\n", lTakeInfo->mName.Buffer() );
      FBXSDK_printf( "         Description: \"%s\"\n", lTakeInfo->mDescription.Buffer() );

      // Change the value of the import name if the animation stack should be imported
      // under a different name.
      FBXSDK_printf( "         Import Name: \"%s\"\n", lTakeInfo->mImportName.Buffer() );

      // Set the value of the import state to false if the animation stack should be not
      // be imported.
      FBXSDK_printf( "         Import State: %s\n", lTakeInfo->mSelect ? "true" : "false" );
      FBXSDK_printf( "\n" );
    }

    // Set the import states. By default, the import states are always set to
    // true. The code below shows how to change these states.
    IOS_REF.SetBoolProp( IMP_FBX_MATERIAL, true );
    IOS_REF.SetBoolProp( IMP_FBX_TEXTURE, true );
    IOS_REF.SetBoolProp( IMP_FBX_LINK, true );
    IOS_REF.SetBoolProp( IMP_FBX_SHAPE, true );
    IOS_REF.SetBoolProp( IMP_FBX_GOBO, true );
    IOS_REF.SetBoolProp( IMP_FBX_ANIMATION, true );
    IOS_REF.SetBoolProp( IMP_FBX_GLOBAL_SETTINGS, true );
  }

  // Import the scene.
  lStatus = lImporter->Import( pScene );
  if ( lStatus == false && lImporter->GetStatus() == FbxStatus::ePasswordError ) {
    FBXSDK_printf( "Please enter password: " );

    lPassword[0] = '\0';

    FBXSDK_CRT_SECURE_NO_WARNING_BEGIN
    scanf( "%s", lPassword );
    FBXSDK_CRT_SECURE_NO_WARNING_END

    FbxString lString( lPassword );

    IOS_REF.SetStringProp( IMP_FBX_PASSWORD, lString );
    IOS_REF.SetBoolProp( IMP_FBX_PASSWORD_ENABLE, true );

    lStatus = lImporter->Import( pScene );

    if ( lStatus == false && lImporter->GetStatus() == FbxStatus::ePasswordError ) {
      FBXSDK_printf( "\nPassword is wrong, import aborted.\n" );
    }
  }

  if ( !lStatus || ( lImporter->GetStatus() != FbxStatus::eSuccess ) ) {
    FBXSDK_printf( "********************************************************************************\n" );
    if ( lStatus ) {
      FBXSDK_printf( "WARNING:\n" );
      FBXSDK_printf( "   The importer was able to read the file but with errors.\n" );
      FBXSDK_printf( "   Loaded scene may be incomplete.\n\n" );
    } else {
      FBXSDK_printf( "Importer failed to load the file!\n\n" );
    }

    if ( lImporter->GetStatus() != FbxStatus::eSuccess ) {
      FBXSDK_printf( "   Last error message: %s\n", lImporter->GetStatus().GetErrorString() );
    }

    FbxArray<FbxString*> history;
    lImporter->GetStatus().GetErrorStringHistory( history );
    if ( history.GetCount() > 1 ) {
      FBXSDK_printf( "   Error history stack:\n" );
      for ( int i = 0; i < history.GetCount(); i++ ) {
        FBXSDK_printf( "      %s\n", history[i]->Buffer() );
      }
    }
    FbxArrayDelete<FbxString*>( history );
    FBXSDK_printf( "********************************************************************************\n" );
  }

  // Destroy the importer.
  lImporter->Destroy();

  return lStatus;
}


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

  // FBX SDK Default Manager
  FbxManager* lSdkManager = NULL;

  // Scene to load from file
  FbxScene* lScene = NULL;

  // Prepare the FBX SDK.
  InitializeSdkObjects( lSdkManager, lScene );

  // Load the scene.
  bool lResult = LoadScene( lSdkManager, lScene, "~/learn-opengl/06-model-loading/fbxsdk/samples/ConvertScene/box.fbx" );


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

  // Create shaders
  PhongShader phongShader;

  // set up vertex data (and buffer(s)) and configure vertex attributes
  // ------------------------------------------------------------------
  std::vector<float> vertices = {
      // position        // uv
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
      0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  // bottom right
      0.5f, 0.5f, 0.0f, 1.0f, 1.0f,   // top right
      -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,  // top left
  };

  std::vector<int> indices = {
      // note that we start from 0!
      0, 1, 2, // first Triangle
      0, 2, 3, // second Triangle
  };

  unsigned int VBO, VAO, EBO;
  glGenVertexArrays( 1, &VAO );
  glGenBuffers( 1, &VBO );
  glGenBuffers( 1, &EBO );
  // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
  glBindVertexArray( VAO );

  glBindBuffer( GL_ARRAY_BUFFER, VBO );
  glBufferData( GL_ARRAY_BUFFER, sizeof( float ) * vertices.size(), vertices.data(), GL_STATIC_DRAW );

  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO );
  glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( int ) * indices.size(), indices.data(), GL_STATIC_DRAW );

  glVertexAttribPointer( 0, // shader location index
      3,                    // number of components per attribute
      GL_FLOAT,
      GL_FALSE,            // normalized (clamp between -1 to 1)
      5 * sizeof( float ), // stride in bytes to the next same vertex attribute
      (void*) 0            // byte size offset for the first component
  );
  glEnableVertexAttribArray( 0 );

  glVertexAttribPointer( 1, // shader location index
      2,                    // number of components per attribute
      GL_FLOAT,
      GL_FALSE,                       // normalized
      5 * sizeof( float ),            // stride in bytes to the next same vertex attribute
      (void*) ( 3 * sizeof( float ) ) // byte size offset for first component
  );
  glEnableVertexAttribArray( 1 );

  // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex
  // buffer object so afterwards we can safely unbind
  glBindBuffer( GL_ARRAY_BUFFER, 0 );

  // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep
  // the EBO bound.
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens.
  // Modifying other VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when
  // it's not directly necessary.
  glBindVertexArray( 0 );

  // Load image data
  int texWidth, texHeight, texChannels;
  stbi_set_flip_vertically_on_load( true ); // tell stb_image.h to flip loaded texture's on the y-axis.
  unsigned char* data = stbi_load( "resources/uv-check.jpg", &texWidth, &texHeight, &texChannels, 0 );

  if ( !data ) {
    throw "Could not load texture";
  }

  // Create texture
  GLuint textureId;
  glGenTextures( 1, &textureId );
  glBindTexture( GL_TEXTURE_2D, textureId );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

  glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data );
  glGenerateMipmap( GL_TEXTURE_2D );

  stbi_image_free( data );

  // uncomment this call to draw in wireframe polygons.
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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

    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, textureId );

    // draw our first triangle
    phongShader.use();
    glUniform1i( glGetUniformLocation( textureId, "colorTexture" ), 0 ); // set it manually

    glBindVertexArray( VAO ); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do
                              // so to keep things a bit more organized
    // glDrawArrays(GL_TRIANGLES, 0, 6);
    glDrawElements( GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0 );
    // glBindVertexArray(0); // no need to unbind it every time

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers( window );
    glfwPollEvents();
  }

  // optional: de-allocate all resources once they've outlived their purpose:
  // ------------------------------------------------------------------------
  glDeleteVertexArrays( 1, &VAO );
  glDeleteBuffers( 1, &VBO );
  glDeleteBuffers( 1, &EBO );
  phongShader.destroy();

  // glfw: terminate, clearing all previously allocated GLFW resources.
  // ------------------------------------------------------------------
  glfwTerminate();
  return 0;
}
