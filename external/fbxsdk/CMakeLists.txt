cmake_minimum_required(VERSION 3.0.0)

project(fbxsdk)

find_package(LibXml2 REQUIRED)
find_package(ZLIB REQUIRED)

set(FBXSDK_DEPENDENCIES
        ${LIBXML2_LIBRARY}
        ${ZLIB_LIBRARY}
)

set(FBXSDK_INCLUDE_DIRS
        ${LIBXML2_INCLUDE_DIR}
        ${ZLIB_INCLUDE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
set(FBXSDK_INCLUDE_DIRS ${FBXSDK_INCLUDE_DIRS} CACHE INTERNAL "")

option(DEBUG "Build Debug" ON)

if(${DEBUG})
    message("FBXSDK::Debug")
    set(FBXSDK_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/lib/gcc/x64/debug/libfbxsdk.so)
else ()
    message("FBXSDK::Release")
    set(FBXSDK_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/lib/gcc/x64/release/libfbxsdk.so)
endif ()

set(FBXSDK_LIBRARY ${FBXSDK_LIBRARY} ${FBXSDK_DEPENDENCIES} CACHE INTERNAL "")

message(${FBXSDK_LIBRARY})