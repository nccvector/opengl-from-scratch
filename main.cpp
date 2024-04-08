#include "Common.h"
#include "Application.h"

int main(){
  // Set logging level
//  spdlog::set_pattern("[source %s] [function %!] [line %#] %v");
  spdlog::set_level(spdlog::level::trace);

  Application::getSingleton().Run();
}