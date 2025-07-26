#include "texture.hpp"
#include "render_core.hpp"
#include "render_utils.hpp"
#include <print>

namespace odeer 
{
  struct texture2d::data 
  {
    uint32_t width{ 0 },
             height{ 0 },
             renderer_id{ 0 },
             data_format,
             internal_format;
    struct specs specs;
  };

  texture2d::texture2d(uint32_t width, uint32_t height, size_t data_size, const void* data)
  {
    m_data = new struct data();
    m_data->internal_format = GL_RGBA;
    m_data->data_format = GL_RGBA;
    m_data->width = width;
    m_data->height = height;
    glCall(glGenTextures(1, &m_data->renderer_id));
    glCall(glActiveTexture(GL_TEXTURE0));
    glCall(glBindTexture(GL_TEXTURE_2D, m_data->renderer_id));
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)); 
    if(data_size != 0 && data) set_data(data_size, data);
  }

  texture2d::texture2d(uint8_t* data, int width, int height, int color_channels, const specs& s)
  {
    if(!data)
    {
      std::println("Error Loading Texture data is nullptr");
      return;
    }

    m_data = new struct data();

    glCall(glGenTextures(1, &m_data->renderer_id));
    glCall(glActiveTexture(GL_TEXTURE0));
    glCall(glBindTexture(GL_TEXTURE_2D, m_data->renderer_id));
    if(s.auto_fmt)
    {
      switch(color_channels)
      {
        case 3:
          m_data->internal_format = GL_RGB;
          m_data->data_format = GL_RGB;
          break;
        case 4:
          m_data->internal_format = GL_RGBA;
          m_data->data_format = GL_RGBA;
          break;
      }
      if(!(color_channels > 2 && color_channels < 5))
      {
        std::println("Unsupported Image format: color channels: {}", color_channels);
        glCall(glDeleteTextures(1, &m_data->renderer_id));
        return;
      }
    }
    else 
    {
      m_data->internal_format = utils::to_gl_internal_format(s.ifmt);
      m_data->data_format = utils::to_gl_format(s.fmt);
    }
    
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, utils::to_gl_wrap(s.s)));
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, utils::to_gl_wrap(s.t)));
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, utils::to_gl_filter(s.min)));
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, utils::to_gl_filter(s.mag))); 
    glCall(glTexImage2D(GL_TEXTURE_2D, 0, m_data->internal_format, width, height
        , 0, m_data->data_format, GL_UNSIGNED_BYTE, data));

    if(s.gen_mip_map)
      glCall(glGenerateMipmap(GL_TEXTURE_2D));
    m_data->width = width;
    m_data->height = height;
  }

  texture2d::~texture2d()
  {
    glCall(glDeleteTextures(1, &m_data->renderer_id));
    delete m_data;
    m_data = nullptr;
  }


  void texture2d::bind(uint8_t slot) const
  {
    glCall(glActiveTexture(GL_TEXTURE0 + slot));
    glCall(glBindTexture(GL_TEXTURE_2D, m_data->renderer_id));
  }

  void texture2d::set_data(size_t size, const void* data, uint8_t slot)
  {
    bind(slot);
    glCall(glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLenum>(m_data->data_format), 
          m_data->width, m_data->height, 0, static_cast<GLenum>(m_data->internal_format), 
          GL_UNSIGNED_BYTE, data));
  }

  uint32_t texture2d::get_width() const
  {
    return m_data->width;
  }

  uint32_t texture2d::get_height() const
  {
    return m_data->height;
  }

  uint32_t texture2d::get_renderer_id() const
  {
    return m_data->renderer_id;
  }
}
