#ifndef ODEER_OPENGL_CORE_HPP
#define ODEER_OPENGL_CORE_HPP

#include "utils/macros.hpp"

#ifndef ODEER_MOBILE
  #include <glad/glad.h>
#define GLES 0
#else
  #include <glad/gles2.h>
#define GLES 1
#endif //ODEER_MOBILE

#define glCall(x) x; \
  odeer_gl_print_error(#x, __FILE__, __LINE__);
void odeer_gl_print_error(const char* fn, const char* file, int line);


#endif //ODEER_OPENGL_CORE_HPP
