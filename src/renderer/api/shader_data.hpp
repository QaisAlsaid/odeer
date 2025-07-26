#ifndef ODEER_SHADER_DATA_HPP
#define ODEER_SHADER_DATA_HPP

#include <cstdint>
#include <string>

namespace odeer 
{
  enum class shader_data_type
  {
    none = 0,
    int1, int2, int3, int4,
    float1, float2, float3, float4,
    mat2, mat3, mat4,
    sampler2d, 
    boolean, 
    byte1, byte2, byte3, byte4
  };

  std::string shader_data_type_to_string(shader_data_type type);
  uint32_t get_shader_data_type_size(shader_data_type type);
  uint32_t get_shader_data_type_component_count(shader_data_type type);
} // namespace odeer

#endif //ODEER_SHADER_DATA_HPP
