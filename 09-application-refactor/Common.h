//
// Created by vector on 24/02/05.
//

#ifndef COMMON_H
#define COMMON_H

#include <chrono>
#include <iostream>
#include <vector>
#include <string>
#include <memory>

// GL
#include "glad/glad.h"

// GL Mathematics
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"

// Image read write headers
#include "stb_image.h"
#include "stb_image_write.h"

#include "ResourceManager.h"


typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::milliseconds Milliseconds;
typedef std::chrono::nanoseconds Nanoseconds;

extern std::chrono::time_point<std::chrono::_V2::system_clock, Nanoseconds> startTime;
extern double timeSinceStart;

#endif // COMMON_H
