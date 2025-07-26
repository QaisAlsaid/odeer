#include "index_buffer.hpp"
#include "render_core.hpp"

namespace odeer 
{
  struct index_buffer::data 
  {
    uint32_t renderer_id{ 0 };
    uint32_t count{ 0 };
  };

  index_buffer::index_buffer(uint32_t count, const uint32_t* data, index_buffer_usage usage)
  {
    m_data = new struct data();
    glCall(glGenBuffers(1, &m_data->renderer_id));
    glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_data->renderer_id));
    glCall(set_data(count, data, usage));
  }

  index_buffer::~index_buffer()
  {
    glCall(glDeleteBuffers(1, &m_data->renderer_id));
    glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    m_data->renderer_id = 0;
    delete m_data;
    m_data = nullptr;
  }

  void index_buffer::bind() const 
  {
    glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_data->renderer_id));
  }

  void index_buffer::unbind()
  {
    glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
  }

  void index_buffer::set_data(uint32_t count, const uint32_t* data, index_buffer_usage usage)
  {
    glCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), data, usage == index_buffer_usage::dynamic_draw ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW));
  }

  uint32_t index_buffer::get_count() const 
  {
    return m_data->count;
  }
}
