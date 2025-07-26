#ifndef ODEER_BUFFER_LAYOUT_HPP
#define ODEER_BUFFER_LAYOUT_HPP

#include "utils/macros.hpp"
#include "shader_data.hpp"

#include <cstdint>
#include <initializer_list>
#include <sstream>
#include <string>
#include <vector>

namespace odeer
{
  struct ODEER_API buffer_element 
  {
    buffer_element(const std::string& name, shader_data_type type, bool normalized = false) 
      : type(type), offset(0), size(get_shader_data_type_size(type)), 
      count(get_shader_data_type_component_count(type)), normalized(normalized) 
    {
    }
    
    buffer_element() 
    {
    }
    
    shader_data_type type;
    uint32_t offset = 0;
    uint32_t size = 0;
    uint32_t count = 0;
    bool normalized = false;
  };

  class ODEER_API buffer_layout 
  {
  public:
    buffer_layout() 
    {
    }
    
    buffer_layout(const std::initializer_list<buffer_element>& elements)
      : m_elements(elements) 
    {
      calc();
    }

    inline uint32_t get_stride() const 
    {
      return m_stride;
    }
    
    inline const std::vector<buffer_element>& get_elements() const 
    {
      return m_elements;
    }

    std::string to_string() const
    {
      std::stringstream ss;
      ss << "stride: " << m_stride;
      for(auto& e : m_elements)
      {
        ss << "type: "<< shader_data_type_to_string(e.type) << " offset: " << e.offset << " count: " << e.count << " normalized: " << e.normalized;
      }
      return ss.str();
    }
  private:
    inline void calc()
    {
      uint32_t offset = 0;
      for(auto& e : m_elements)
      {
        e.offset = offset;
        offset += e.size;
        m_stride += e.size;
      }
    }
  private:
    std::vector<buffer_element> m_elements;
    uint32_t m_stride = 0;
  };

  inline std::ostream& operator<< (std::ostream& os, shader_data_type type)
  {
    return os << shader_data_type_to_string(type);
  }
} //namespace odeer 

#endif //ODEER_BUFFER_LAYOUT_HPP
