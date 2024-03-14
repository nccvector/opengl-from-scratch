//
// Created by vector on 24/03/03.
//

#include <fstream>
#include <fbxsdk.h>

#include "Common.h"
#include "ResourceManager.h"


namespace ResourceManager {

std::shared_ptr<PhongShader> defaultShader       = nullptr;
std::vector<std::shared_ptr<Texture>> textures   = {};
std::vector<std::shared_ptr<Material>> materials = {};
std::vector<std::shared_ptr<Mesh>> meshes        = {};
std::vector<std::shared_ptr<Model>> models       = {};

void InitializeShaders() {
  defaultShader = std::make_shared<PhongShader>();
}

void EnsureShaderActiveState( std::shared_ptr<PhongShader> shader ) {
  // Deactivate currently bound shader before activing new one
  // currentShader.Deativate();
  // curretnShader = shader;
  if ( !shader->IsActive() ) {
    shader->Activate();
  }
}

template <typename T>
std::vector<std::shared_ptr<T>> GetResourceList() {}

template <>
std::vector<std::shared_ptr<Texture>> GetResourceList<Texture>() {
  return textures;
}

template <>
std::vector<std::shared_ptr<Material>> GetResourceList<Material>() {
  return materials;
}

template <>
std::vector<std::shared_ptr<Mesh>> GetResourceList<Mesh>() {
  return meshes;
}

template <>
std::vector<std::shared_ptr<Model>> GetResourceList<Model>() {
  return models;
}

template <typename T>
void AddResource( const std::shared_ptr<T>& resource ) {}

template <>
void AddResource<Texture>( const std::shared_ptr<Texture>& resource ) {
  textures.push_back( resource );
}

template <>
void AddResource<Material>( const std::shared_ptr<Material>& resource ) {
  materials.push_back( resource );
}

template <>
void AddResource<Mesh>( const std::shared_ptr<Mesh>& resource ) {
  meshes.push_back( resource );
}

template <>
void AddResource<Model>( const std::shared_ptr<Model>& resource ) {
  models.push_back( resource );
}


void LoadFile( const char* filePath, std::string& out ) {
  std::ifstream t( filePath );
  out = std::string( ( std::istreambuf_iterator<char>( t ) ), std::istreambuf_iterator<char>() );
}


#ifdef IOS_REF
#undef IOS_REF
#define IOS_REF ( *( lSdkManager->GetIOSettings() ) )
#endif

bool LoadScene( const char* path, FbxScene*& scene ) {
  // SETTING UP FBX RESOURCES
  // FBX SDK Default Manager
  FbxManager* lSdkManager = FbxManager::Create();
  if ( !lSdkManager ) {
    ERROR( "ERROR CREATING FBX MANAGER" );
    exit( 1 );
  } else {
    DEBUG( "Autodesk FBX SDK version: {}", lSdkManager->GetVersion() );
  }

  // Create an IOSettings object. This object holds all import/export settings.
  FbxIOSettings* ios = FbxIOSettings::Create( lSdkManager, IOSROOT );
  lSdkManager->SetIOSettings( ios );

  // Load plugins from the executable directory (optional)
  FbxString lPath = FbxGetApplicationDirectory();
  lSdkManager->LoadPluginsDirectory( lPath.Buffer() );

  // Create an FBX scene. This object holds most objects imported/exported from/to files.
  scene = FbxScene::Create( lSdkManager, "My Scene" );
  if ( !scene ) {
    ERROR( "Error: Unable to create FBX scene!\n" );
    exit( 1 );
  }

  // LOADING THE ACTUAL SCENE
  int lFileMajor, lFileMinor, lFileRevision;
  int lSDKMajor, lSDKMinor, lSDKRevision;
  // int lFileFormat = -1;
  int lAnimStackCount;
  bool lStatus;
  char lPassword[1024];

  // Get the file version number generate by the FBX SDK.
  FbxManager::GetFileFormatVersion( lSDKMajor, lSDKMinor, lSDKRevision );

  // Create an importer.
  FbxImporter* lImporter = FbxImporter::Create( lSdkManager, "" );

  // Initialize the importer by providing a filename.
  const bool lImportStatus = lImporter->Initialize( path, -1, lSdkManager->GetIOSettings() );
  lImporter->GetFileVersion( lFileMajor, lFileMinor, lFileRevision );

  if ( !lImportStatus ) {
    FbxString error = lImporter->GetStatus().GetErrorString();
    ERROR( "Call to FbxImporter::Initialize() failed.\n" );
    ERROR( "Error returned: %s\n\n", error.Buffer() );

    if ( lImporter->GetStatus().GetCode() == FbxStatus::eInvalidFileVersion ) {
      DEBUG( "FBX file format version for this FBX SDK is {}.{}.{}\n", lSDKMajor, lSDKMinor, lSDKRevision );
      DEBUG(
          "FBX file format version for file '%s' is {}.{}.{}\n\n", path, lFileMajor, lFileMinor, lFileRevision );
    }

    return false;
  }

  DEBUG( "FBX file format version for this FBX SDK is {}.{}.{}\n", lSDKMajor, lSDKMinor, lSDKRevision );

  if ( lImporter->IsFBX() ) {
    DEBUG(
        "FBX file format version for file '%s' is {}.{}.{}", path, lFileMajor, lFileMinor, lFileRevision );

    // From this point, it is possible to access animation stack information without
    // the expense of loading the entire file.

    DEBUG( "Animation Stack Information" );

    lAnimStackCount = lImporter->GetAnimStackCount();

    DEBUG( "    Number of Animation Stacks: {}", lAnimStackCount );
    DEBUG( "    Current Animation Stack: {}", lImporter->GetActiveAnimStackName().Buffer() );

    for ( int i = 0; i < lAnimStackCount; i++ ) {
      FbxTakeInfo* lTakeInfo = lImporter->GetTakeInfo( i );

      DEBUG( "    Animation Stack {}", i );
      DEBUG( "         Name: {}", lTakeInfo->mName.Buffer() );
      DEBUG( "         Description: {}", lTakeInfo->mDescription.Buffer() );

      // Change the value of the import name if the animation stack should be imported
      // under a different name.
      DEBUG( "         Import Name: {}", lTakeInfo->mImportName.Buffer() );

      // Set the value of the import state to false if the animation stack should be not
      // be imported.
      DEBUG( "         Import State: {}", lTakeInfo->mSelect ? "true" : "false" );
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
  lStatus = lImporter->Import( scene );
  if ( lStatus == false && lImporter->GetStatus() == FbxStatus::ePasswordError ) {
    FBXSDK_printf( "Please enter password: " );

    lPassword[0] = '\0';

    FBXSDK_CRT_SECURE_NO_WARNING_BEGIN
    scanf( "%s", lPassword );
    FBXSDK_CRT_SECURE_NO_WARNING_END

    FbxString lString( lPassword );

    IOS_REF.SetStringProp( IMP_FBX_PASSWORD, lString );
    IOS_REF.SetBoolProp( IMP_FBX_PASSWORD_ENABLE, true );

    lStatus = lImporter->Import( scene );

    if ( lStatus == false && lImporter->GetStatus() == FbxStatus::ePasswordError ) {
      ERROR( "\nPassword is wrong, import aborted.\n" );
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

  FbxGeometryConverter converter( lSdkManager );
  converter.Triangulate( scene, true );

  return lStatus;
}

} // namespace ResourceManager
