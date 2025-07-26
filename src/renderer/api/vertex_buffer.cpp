#include "vertex_buffer.hpp"
#include "render_core.hpp"


namespace odeer 
{
  struct vertex_buffer::data 
  {
    buffer_layout layout;
    uint32_t renderer_id{ 0 };
  };

  
  vertex_buffer::vertex_buffer(uint32_t size, const float* data)
  {
    m_data = new struct data;
    glCall(glGenBuffers(1, &m_data->renderer_id));
    glCall(glBindBuffer(GL_ARRAY_BUFFER, m_data->renderer_id));
    if(data != nullptr)
    {
      glCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
    }
    else 
    {
      glCall(glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW));
    }
  }

  vertex_buffer::~vertex_buffer()
  {
    glDeleteBuffers(1, &m_data->renderer_id);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    m_data->renderer_id = 0;
    delete m_data;
    m_data = 0;
  }

  void vertex_buffer::bind() const
  {
    glCall(glBindBuffer(GL_ARRAY_BUFFER, m_data->renderer_id));
  }

  void vertex_buffer::unbind()
  {
    glCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
  }

  void vertex_buffer::set_data(uint32_t size, const void* data, uint32_t offset)
  {
    glCall(glBindBuffer(GL_ARRAY_BUFFER, m_data->renderer_id));
    glCall(glBufferSubData(GL_ARRAY_BUFFER, offset, size, data));
  }

  void vertex_buffer::set_layout(const buffer_layout& layout)
  {
    m_data->layout = layout;
  }

  const buffer_layout& vertex_buffer::get_layout() const 
  {
    return m_data->layout;
  }
}
