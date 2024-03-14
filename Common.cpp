//
// Created by vector on 24/02/19.
//

#include "Common.h"

// Image read write implementations
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


std::chrono::time_point<std::chrono::_V2::system_clock, Nanoseconds> startTime = Clock::now();
double timeSinceStart = std::chrono::duration_cast<Milliseconds>( Clock::now() - startTime ).count();
