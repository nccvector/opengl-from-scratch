//
// Created by vector on 24/03/03.
//

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

} // namespace ResourceManager
