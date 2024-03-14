#include <fstream>

#include "utils.h"


namespace utils {
void loadFile( const char* filePath, std::string& out ) {
  std::ifstream t( filePath );
  out = std::string( ( std::istreambuf_iterator<char>( t ) ), std::istreambuf_iterator<char>() );
}
} // namespace utils