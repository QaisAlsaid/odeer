#include "shader_data.hpp"

namespace odeer 
{
  std::string shader_data_type_to_string(shader_data_type type)
  {
    switch(type)
    {
      case shader_data_type::none:
        return "none";
      case shader_data_type::int1:
        return "int1";
      case shader_data_type::int2:
        return "int2";
      case shader_data_type::int3:
        return "int3";
      case shader_data_type::int4:
        return "int4";
      case shader_data_type::float1:
        return "float1";
      case shader_data_type::float2:
        return "float2";
      case shader_data_type::float3:
        return "float3";
      case shader_data_type::float4:
        return "float4";
      case shader_data_type::mat2:
        return "mat2";
      case shader_data_type::mat3:
        return "mat3";
      case shader_data_type::mat4:
        return "mat4";
      case shader_data_type::boolean:
        return "boolean";
      case shader_data_type::sampler2d:
        return "sampler2d";
      case shader_data_type::byte1:
        return "byte1";
      case shader_data_type::byte2:
        return "byte2";
      case shader_data_type::byte3:
        return "byte3";
      case shader_data_type::byte4:
        return "byte4";
    }
    return "";
  }

  uint32_t get_shader_data_type_size(shader_data_type type)
  {
    switch(type)
    {
      case shader_data_type::none:
        return 0;
      case shader_data_type::int1:
        return sizeof(int);
      case shader_data_type::int2:
        return sizeof(int) * 2;
      case shader_data_type::int3:
        return sizeof(int) * 3;
      case shader_data_type::int4:
        return sizeof(int) * 4;
      case shader_data_type::float1:
        return sizeof(float);
      case shader_data_type::float2:
        return sizeof(float) * 2;
      case shader_data_type::float3:
        return sizeof(float) * 3;
      case shader_data_type::float4:
        return sizeof(float) * 4;
      case shader_data_type::mat2:
        return sizeof(float) * 2 * 2;
      case shader_data_type::mat3:
        return sizeof(float) * 3 * 3;
      case shader_data_type::mat4:
        return sizeof(float) * 4 * 4;
      case shader_data_type::boolean:
        return sizeof(bool);
      case shader_data_type::sampler2d:
        return sizeof(int);
      case shader_data_type::byte1:
        return sizeof(uint8_t);
      case shader_data_type::byte2:
        return sizeof(uint8_t) * 2;
      case shader_data_type::byte3:
        return sizeof(uint8_t) * 3;
      case shader_data_type::byte4:
        return sizeof(uint8_t) * 4;
    }
  }
  
  uint32_t get_shader_data_type_component_count(shader_data_type type)
  {
    switch(type)
    {
      case shader_data_type::none:
        return 0;
      case shader_data_type::int1:
        return 1;
      case shader_data_type::int2:
        return 2;
      case shader_data_type::int3:
        return 3;
      case shader_data_type::int4:
        return 4;
      case shader_data_type::float1:
        return 1;
      case shader_data_type::float2:
        return 2;
      case shader_data_type::float3:
        return 3;
      case shader_data_type::float4:
        return 4;
      case shader_data_type::mat2:
        return 2 * 2;
      case shader_data_type::mat3:
        return 3 * 3;
      case shader_data_type::mat4:
        return 4 * 4;
      case shader_data_type::boolean:
        return 1;
      case shader_data_type::sampler2d:
        return 1;
      case shader_data_type::byte1:
        return 1;
      case shader_data_type::byte2:
        return 2;
      case shader_data_type::byte3:
        return 3;
      case shader_data_type::byte4:
        return 4;
    }
  }
}
