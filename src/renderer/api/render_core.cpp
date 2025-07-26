#include "render_core.hpp"
#include <print>

void odeer_gl_print_error(const char *fn, const char *file, int line)
{
  while(auto err = glGetError())
  {
    std::println("opengl error: code: {}, file: {}, line: {}, function: {}", err, file, line, fn);
  }
}
