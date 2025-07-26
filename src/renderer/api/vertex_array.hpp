#ifndef ODEER_VERTEX_ARRAY_HPP
#define ODEER_VERTEX_ARRAY_HPP

#include "utils/macros.hpp"
#include "vertex_buffer.hpp"
#include "index_buffer.hpp"

#include <memory>

namespace odeer
{
  class ODEER_API vertex_array 
  {
  public:
    vertex_array();
    ~vertex_array();

    void bind() const;
    static void unbind();

    void add_vertex_buffer(const std::shared_ptr<vertex_buffer>&);
    void set_index_buffer(const std::shared_ptr<index_buffer>&);

    const std::vector<std::shared_ptr<vertex_buffer>>& get_vertex_buffers() const;
    const std::shared_ptr<index_buffer>& get_index_buffer() const;
  private:
    struct data;
    data* m_data;
  };
} // namespace odeer

#endif //ODEER_VERTEX_ARRAY_HPP
