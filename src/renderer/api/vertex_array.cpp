#include "vertex_array.hpp"
#include "render_core.hpp"
#include <memory>
#include <vector>

namespace odeer 
{
  struct vertex_array::data 
  {
    std::vector<std::shared_ptr<vertex_buffer>> vertex_buffers;
    std::shared_ptr<index_buffer> _index_buffer{ nullptr };
    uint32_t renderer_id{ 0 },
             vertex_buffer_index{ 0 };
  };

  vertex_array::vertex_array()
  {
    m_data = new data();
    glCall(glGenVertexArrays(1, &m_data->renderer_id));
  }


  vertex_array::~vertex_array()
  {
    glCall(glDeleteVertexArrays(1, &m_data->renderer_id));
    glCall(glBindVertexArray(0));
    m_data->renderer_id = 0;
    delete m_data;
    m_data = nullptr;
  }

  void vertex_array::bind() const 
  {
    glCall(glBindVertexArray(m_data->renderer_id));
  }

  void vertex_array::unbind() 
  {
    glCall(glBindVertexArray(0));
  }

  void vertex_array::add_vertex_buffer(const std::shared_ptr<vertex_buffer>& vb)
  {
    glBindVertexArray(m_data->renderer_id);
    vb->bind();
    const auto& layout = vb->get_layout();
    for(uint32_t i = 0; i < layout.get_elements().size(); ++i)
    {
      const auto& elem = layout.get_elements().at(i);
      switch(elem.type)
      {
        case shader_data_type::float1:
        case shader_data_type::float2:
        case shader_data_type::float3:
        case shader_data_type::float4:
        {
          glEnableVertexAttribArray(m_data->vertex_buffer_index);
          glVertexAttribPointer(m_data->vertex_buffer_index, 
            elem.count, GL_FLOAT, 
            elem.normalized == true ? GL_TRUE : GL_FALSE, 
            layout.get_stride(), (const void*)(uintptr_t)(elem.offset)
          );
          m_data->vertex_buffer_index++;
          break;
        }
        case shader_data_type::sampler2d:
        case shader_data_type::int1:
        case shader_data_type::int2:
        case shader_data_type::int3:
        case shader_data_type::int4:
        {
          glEnableVertexAttribArray(m_data->vertex_buffer_index);
          glVertexAttribIPointer(
            m_data->vertex_buffer_index,
            elem.count, GL_INT,
            layout.get_stride(), (const void*)(uintptr_t)(elem.offset)
          );
          m_data->vertex_buffer_index++;
          break;
        }
        case shader_data_type::byte1:
        case shader_data_type::byte2:
        case shader_data_type::byte3:
        case shader_data_type::byte4:
        {
          glEnableVertexAttribArray(m_data->vertex_buffer_index);
          glVertexAttribPointer(
            m_data->vertex_buffer_index,
            elem.count, GL_UNSIGNED_BYTE, GL_TRUE, 
            layout.get_stride(), (const void*)(uintptr_t)(elem.offset)
          );
          m_data->vertex_buffer_index++;
          break;
        }
        case shader_data_type::mat2:
        case shader_data_type::mat3:
        case shader_data_type::mat4:
        case shader_data_type::none:
        case shader_data_type::boolean:
        default: break;
      }
    }
    m_data->vertex_buffers.push_back(vb);
    vb->unbind();
    glBindVertexArray(0);
  }

  void vertex_array::set_index_buffer(const std::shared_ptr<index_buffer>& ib) 
  {
    glCall(glBindVertexArray(m_data->renderer_id));
    m_data->_index_buffer = ib;
    ib->bind();
  }

  const std::vector<std::shared_ptr<vertex_buffer>>& vertex_array::get_vertex_buffers() const 
  {
    return m_data->vertex_buffers;
  }

  const std::shared_ptr<index_buffer>& vertex_array::get_index_buffer() const
  {
    return m_data->_index_buffer;
  }
}
