#ifndef ODEER_INDEX_BUFFER_HPP
#define ODEER_INDEX_BUFFER_HPP

#include "utils/macros.hpp"
#include <cstdint>

namespace odeer 
{
  enum class index_buffer_usage
  {
    static_draw, dynamic_draw
  };

  class ODEER_API index_buffer 
  {
  public:
    index_buffer(uint32_t count, const uint32_t* data, index_buffer_usage usage = index_buffer_usage::static_draw);
    ~index_buffer();

    void bind() const;
    static void unbind();
    void set_data(uint32_t count, const uint32_t* data, index_buffer_usage usage);
    uint32_t get_count() const;
  private:
    struct data;
    data* m_data;
  };
}

#endif //ODEER_INDEX_BUFFER_HPP
