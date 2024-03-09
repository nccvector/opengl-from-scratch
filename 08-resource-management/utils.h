#ifndef UTIL_H
#define UTIL_H

#include "common.h"
#include <fbxsdk.h>

namespace utils {

void loadFile( const char* path, std::string& out );

bool LoadScene( const char* path, FbxScene*& scene );

} // namespace utils

#endif // UTIL_H
