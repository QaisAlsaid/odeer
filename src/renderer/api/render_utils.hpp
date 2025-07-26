#ifndef ODEER_RENDER_UTILS_HPP
#define ODEER_RENDER_UTILS_HPP

#include "texture.hpp"

namespace odeer::utils 
{
  uint32_t to_gl_filter(texture::filter);
  uint32_t to_gl_wrap(texture::wrap);
  uint32_t to_gl_format(texture::format);
  uint32_t to_gl_internal_format(texture::internal_format);
  void bind_texture(uint32_t id, int samples);
  void applay_texture_specs(const texture::specs& s);
}

#endif //ODEER_RENDER_UTILS_HPP
