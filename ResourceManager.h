//
// Created by vector on 24/03/03.
//

#ifndef INC_08_RESOURCE_MANAGEMENT_RESOURCEMANAGER_H
#define INC_08_RESOURCE_MANAGEMENT_RESOURCEMANAGER_H

#include "Shader.h"
#include "Types.h"
#include "Texture.h"
#include "Material.h"
#include "Mesh.h"
#include "Model.h"

// Resources which need access from multiple consumers (But be-careful to only mutate in only one place, the rest of the
// places should only consume)
namespace ResourceManager {

void reinitializeResources(); // Used for reloading a different scene

std::shared_ptr<PhongShader> getDefaultShader();

template <typename T>
std::vector<std::shared_ptr<T>> getResourceList();

template <typename T>
void addResource( const std::shared_ptr<T>& resource );

void initializeShaders();
void ensureShaderActiveState( const std::shared_ptr<PhongShader>& shader );

void loadFile( const char* filePath, std::string& out );

bool loadScene( const char* path, FbxScene*& scene );

} // namespace ResourceManager

#endif // INC_08_RESOURCE_MANAGEMENT_RESOURCEMANAGER_H
