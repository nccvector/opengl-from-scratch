# CMAKE configs
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/optix/cmake")
include_directories(${CMAKE_SOURCE_DIR}/optix)

# Find CUDA
find_package(CUDA REQUIRED)
include_directories("${CUDA_INCLUDE_DIRS}")
message(STATUS "Found CUDA in ${CUDA_INCLUDE_DIR}")
mark_as_advanced(CLEAR CUDA_64_BIT_DEVICE_CODE)
mark_as_advanced(CUDA_SDK_ROOT_DIR)

# Find OptiX
find_package(OptiX REQUIRED VERSION 7.0)
include_directories(${OptiX_INCLUDE})
message(STATUS "Found OptiX in ${OPTIX_INCLUDE_DIR}")

# compile common libraries
include_directories(optix/common)
include(${CMAKE_MODULE_PATH}/configure_optix.cmake)

# compile nvidia support libraries
configure_file(optix/sutil/sampleConfig.h.in optix/sutil/sampleConfig.h @ONLY)
include_directories(optix/sutil)
add_subdirectory(optix/sutil/support)
add_subdirectory(optix/sutil)

# set all LIBS
set(OptiX_LIBS ${LIBS} ${optix_LIBRARY} ${CUDA_LIBRARIES} ${CUDA_CUDA_LIBRARY})
