//
// Created by vector on 24/02/10.
//

#ifndef INC_06_MODEL_LOADING_MODEL_H
#define INC_06_MODEL_LOADING_MODEL_H

#include "glad/glad.h"
#include <fbxsdk.h>

#include "common.h"

namespace ModelTools {

std::shared_ptr<Texture> CreateTextureFromData(
    unsigned char* data, const char* name, int width, int height, int channels );
std::shared_ptr<Texture> CreateTextureFromPath( const char* filePath, const char* name );

std::shared_ptr<Material> CreateMaterialFromFbxNode( FbxNode* node );
std::shared_ptr<Mesh> CreateMeshFromFbxNode( FbxNode* node );

std::shared_ptr<Mesh> CreateDeviceMesh( std::vector<Vertex> vertices );

} // namespace ModelTools

#endif // INC_06_MODEL_LOADING_MODEL_H
