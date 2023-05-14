//
// Author: Faizan Ali (github.com/nccvector)
// Date: 23/05/02
//

#include "Model.h"

// assimp include files. These three are usually needed.
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <fstream>

namespace ModelTools {

void LoadOnDevice( Model& model ) {
  // Create a vertex buffer
  glGenBuffers( 1, &( model._VBO ) );

  // Create a vertex array object
  glGenVertexArrays( 1, &( model._VAO ) );

  // Create an index buffer
  glGenBuffers( 1, &( model._EBO ) );

  // 1. Bind VAO
  glBindVertexArray( model._VAO );
  // 2. Bind VBO and copy vertex data
  glBindBuffer( GL_ARRAY_BUFFER, model._VBO );
  glBufferData( GL_ARRAY_BUFFER, sizeof( Vertex ) * model.Vertices.size(), &(model.Vertices)[0], GL_STATIC_DRAW );
  // 3. Bind EBO and copy index data
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, model._EBO );
  glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( unsigned int ) * model.Indices.size(), &(model.Indices)[0], GL_STATIC_DRAW );

  // 4. Linking vertex attributes (how the data is layed out in memory)
  // OpenGL does not know how the vertices are layed out in memory
  // To let the OpenGL know how to connect the vertices, we need to specify
  glVertexAttribPointer( 0,                    // Corresponds to the location = 0 in vertex shader
      (GLint) Vertex::getPositionNumEntries(), // Entries per vertex data (x y and z)
      GL_FLOAT,                                // Type of entry
      GL_FALSE,                                // Specifies whether the data should be normalized or not
      (GLint) Vertex::getSize(),               // Stride size in bytes
      (void*) Vertex::getPositionOffset() );   // offset: Where the position data begins in the buffer
  glEnableVertexAttribArray( 0 );              // Enable this attribute array
  // Create one more for color data layout
  glVertexAttribPointer( 1,                  // Corresponds to the location = 0 in vertex shader
      (GLint) Vertex::getNormalNumEntries(), // Entries per vertex data (r g and b)
      GL_FLOAT,                              // Type of entry
      GL_FALSE,                              // Specifies whether the data should be normalized or not
      (GLint) Vertex::getSize(),             // Stride size in bytes
      (void*) Vertex::getNormalOffset() );   // offset: Where the Normal data begins in the buffer
  glEnableVertexAttribArray( 1 );            // Enable this attribute array
  // Create one more for uv data layout
  glVertexAttribPointer( 2,                    // Corresponds to the location = 0 in vertex shader
      (GLint) Vertex::getTexCoordNumEntries(), // Entries per vertex data (u and v)
      GL_FLOAT,                                // Type of entry
      GL_FALSE,                                // Specifies whether the data should be normalized or not
      (GLint) Vertex::getSize(),               // Stride size in bytes
      (void*) Vertex::getTexCoordOffset() );   // offset: Where the position data begins in the buffer
  glEnableVertexAttribArray( 2 );              // Enable this attribute array

  // Unbind after creation
  glBindVertexArray( 0 );
}

void LoadModelsFromFile( const std::string& filename, std::vector<Model>& models, std::vector<Material>& materials, float scale ) {
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
    VertexList vertexList;
    UIntList indexList;

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

      vertexList.push_back( newVertex );
    }

    // Filling in the indices data
    for ( unsigned int vf = 0; vf < mesh->mNumFaces; vf++ ) {
      aiFace face = mesh->mFaces[vf];
      for ( unsigned int vi = 0; vi < face.mNumIndices; vi++ ) {
        unsigned int aiIndex = face.mIndices[vi];
        indexList.push_back( aiIndex );
      }
    }

    // TODO: Consider deleting host side model data in case of memory shortage
    Model newModel = { vertexList, indexList, glm::mat4( 1.0 ), materials[0] };
    ModelTools::LoadOnDevice( newModel ); // load on device
    newModel.Transform = glm::scale( newModel.Transform, glm::vec3( scale ) );
    models.push_back( newModel );
  }
}

} // namespace ModelTools