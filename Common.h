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

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include <spdlog/spdlog.h>

// GL
#include "glad/glad.h"

// GL Mathematics
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"

// Image read write headers
#include "stb_image.h"
#include "stb_image_write.h"


typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::milliseconds Milliseconds;
typedef std::chrono::nanoseconds Nanoseconds;

extern std::chrono::time_point<std::chrono::_V2::system_clock, Nanoseconds> startTime;
extern double timeSinceStart;

// Use default logger for now...
#define DEBUG(...) SPDLOG_LOGGER_DEBUG(spdlog::default_logger(), __VA_ARGS__)
#define INFO(...) SPDLOG_LOGGER_INFO(spdlog::default_logger(), __VA_ARGS__)
#define WARN(...) SPDLOG_LOGGER_INFO(spdlog::default_logger(), __VA_ARGS__)
#define ERROR(...) SPDLOG_LOGGER_ERROR(spdlog::default_logger(), __VA_ARGS__)
// Now you may set the logging level at your application entry point...


#endif // COMMON_H
