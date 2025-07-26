#ifndef ODEER_VERTEX_BUFFER_HPP
#define ODEER_VERTEX_BUFFER_HPP

#include "utils/macros.hpp"
#include "buffer_layout.hpp"

#include <cstdint>

namespace odeer
{
  class ODEER_API vertex_buffer 
  {
  public:
    vertex_buffer(uint32_t size, const float* data = nullptr);
    ~vertex_buffer();

    void bind() const;
    static void unbind();
    
    void set_data(uint32_t size, const void* data = nullptr, uint32_t offset = 0);
    void set_layout(const buffer_layout& layout);
    const buffer_layout& get_layout() const;
  private:
    struct data;
    data* m_data;
  };
}

#endif //ODEER_VERTEX_BUFFER_HPP
