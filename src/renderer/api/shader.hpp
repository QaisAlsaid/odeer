#ifndef AMATERASU_SHADER_HPP
#define AMATERASU_SHADER_HPP

#include "math/math.hpp"
#include "shader_data.hpp"

#include "utils/macros.hpp"

#include <filesystem>
#include <memory>
#include <string>

namespace odeer 
{
  class ODEER_API shader 
  {
  public:
    shader();
    shader(const std::string& src);
    shader(const std::string& vert, const std::string& frag);
    shader(const std::filesystem::path& path);
    ~shader();

    void bind() const;
    static void unbind();
    
    void load_from_file(const std::filesystem::path& path);
    void load_from_string(const std::string& shader_src);
    
    void set_uniform(const std::string& name, int value);
    void set_uniform(const std::string& name, const mat4& value);
    void set_uniform(const std::string& name, const vec4& value);
    void set_uniform(const std::string& name, const vec3& value);
    void set_uniform(const std::string& name, const vec2& value);
    void set_uniform(const std::string& name, const int* value, uint32_t count);
  public:
    struct uniform_data
    {
      int32_t location{ 0 };
      shader_data_type type{ shader_data_type::none };
    };
  private:
    bool compile(const std::string& vs, const std::string& fs);
    bool create_program();
    bool preprocess(std::istringstream& filepath, std::string& vs, std::string& fs);
  private:
    struct data;
    data* m_data;
  };
} // namespace odeer

#endif //ODEER_SHADER_HPP
