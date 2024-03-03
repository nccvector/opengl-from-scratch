//
// Created by vector on 24/03/03.
//

#ifndef INC_08_RESOURCE_MANAGEMENT_RESOURCEMANAGER_H
#define INC_08_RESOURCE_MANAGEMENT_RESOURCEMANAGER_H

#include "common.h"
#include "Types.h"

// Resources which need access from multiple consumers (But becareful to only mutate in only one place, the rest of the
// places should only consume)
namespace ResourceManager {

extern std::vector<std::shared_ptr<Texture>> textures;
extern std::vector<std::shared_ptr<Material>> materials;
extern std::vector<std::shared_ptr<Mesh>> meshes;
extern std::vector<std::shared_ptr<Model>> models;

template <typename T>
std::vector<std::shared_ptr<T>> GetResourceList();

template <typename T>
void AddResource( std::shared_ptr<T> resource );

} // namespace ResourceManager

#endif // INC_08_RESOURCE_MANAGEMENT_RESOURCEMANAGER_H
