#include "render_utils.hpp"
#include "render_core.hpp"

namespace odeer::utils 
{
  uint32_t to_gl_filter(texture::filter f)
  {
    switch(f)
    {
      case texture::filter::nearest: return GL_NEAREST;
      case texture::filter::linear: return GL_LINEAR;
    }
    return 0;
  }

  uint32_t to_gl_wrap(texture::wrap w)
  {
    switch(w)
    {
      case texture::wrap::clamp_to_edge: return GL_CLAMP_TO_EDGE;
      case texture::wrap::clamp_to_border: return GL_CLAMP_TO_BORDER;
      case texture::wrap::repeat: return GL_REPEAT;
    }
    return 0;
  }
  
  uint32_t to_gl_format(texture::format fmt)
  {
    switch(fmt)
    {
      case texture::format::depth_stencil: return GL_DEPTH_STENCIL;
      case texture::format::rgb: return GL_RGB;
      case texture::format::rgba: return GL_RGBA;
      case texture::format::red_int: return GL_RED_INTEGER;
      case texture::format::rgb_int: return GL_RGB_INTEGER;
      case texture::format::none: return 0;
    }
    return 0;
  }

  uint32_t to_gl_internal_format(texture::internal_format ifmt)
  {
    switch(ifmt)
    {
      case texture::internal_format::depth24stencil8: return GL_DEPTH24_STENCIL8;
      case texture::internal_format::rgba8: return GL_RGBA8;
      case texture::internal_format::rgb8: return GL_RGB8;
      case texture::internal_format::rgb: return GL_RGB;
      case texture::internal_format::rgba: return GL_RGBA;
      case texture::internal_format::rgb32ui: return GL_RGB32UI;
      case texture::internal_format::int32: return GL_R32I;
      case texture::internal_format::uint32: return GL_R32UI;
      case texture::internal_format::none: return 0;
    }
    return 0;
  }

  void bind_texture(uint32_t id, int samples)
  {
    if(samples > 1)
    {
      glCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, id));
    }
    else
    {
      glCall(glBindTexture(GL_TEXTURE_2D, id));
    }
  }
  
  void applay_texture_specs(const texture::specs& s)
  {
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, to_gl_filter(s.min)));
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, to_gl_filter(s.mag)));

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, to_gl_wrap(s.s));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, to_gl_wrap(s.r));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, to_gl_wrap(s.t));
  }
}
