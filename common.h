//
// Created by vector on 24/02/05.
//

#ifndef COMMON_H
#define COMMON_H

#include <chrono>
#include <iostream>
#include <vector>
#include <string>


// Image read write headers
#include "stb_image.h"
#include "stb_image_write.h"

typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::milliseconds Milliseconds;

enum TextureType { DIFFUSE = 0, NORMAL, TEXTURETYPE_NUM_ITEMS };

// Application Level Data structures and Resources
struct Texture {
  const char* name;
  int width;
  int height;
  int channels;
  unsigned int handle;
};

struct Material {
  Texture* textures[TEXTURETYPE_NUM_ITEMS];
};

// Resources which need access from multiple consumers (But becareful to only mutate in only one place, the rest of the
// places should only consume)
namespace global {

extern std::vector<Texture*> textures;

}

#endif // COMMON_H
