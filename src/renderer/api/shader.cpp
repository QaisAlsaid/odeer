#include "shader.hpp"
#include "render_core.hpp"
#include <filesystem>
#include <fstream>
#include <print>
#include <unordered_map>

namespace odeer 
{
  struct shader::data 
  {
    std::string vertex_src;
    std::string fragment_src;
    std::unordered_map<std::string, uniform_data> cache;
    uint32_t program_id{ 0 },
             vertex_id{ 0 },
             fragment_id{ 0 };
  };

  shader::shader()
  {
    m_data = new data();
  }

  shader::shader(const std::filesystem::path& path)
  {
    m_data = new data();
    load_from_file(path);
  }

  shader::shader(const std::string& src)
  {
    m_data = new data();
    load_from_string(src);
  }

  shader::shader(const std::string& vertex, const std::string& fragment)
  {
    m_data = new data();
    //FIXME: wtf
    load_from_string(vertex + fragment);
  }

  shader::~shader()
  {
    glCall(glDeleteProgram(m_data->program_id));
    delete m_data;
    m_data = nullptr;
  }

  void shader::bind() const 
  {
    glCall(glUseProgram(m_data->program_id));
  }

  void shader::unbind()
  {
    glCall(glUseProgram(0));
  }

  void shader::load_from_file(const std::filesystem::path& path)
  {
    std::string vs, fs;
    std::ifstream fstream(path);
    std::stringstream ss;
    ss << fstream.rdbuf();
    std::istringstream iss(ss.str());
    if(preprocess(iss, vs, fs))
    {
      if(compile(vs, fs))
      {
        if(!create_program())
          std::println("error creating shader");
      }
      else 
        std::println("error compiling shader: {}", path.string());
    }
    else 
    {
      std::println("error in preprocess shader: {}", path.string());
    }
  }
  
  void shader::load_from_string(const std::string& shader_src)
  {
    std::string vs, fs;
    std::istringstream iss(shader_src);
    if(preprocess(iss, vs, fs))
    {
      std::println("row vert: [\n {} {}", vs, "\n]");
      std::println("row vert: [\n {} {}", fs, "\n]");
      if(compile(vs, fs))
      {
        if(!create_program())
          std::println("Error Creating Shader Program No Uniforms Will Be Cached");
      }
      else 
        std::println("Error Compiling Shader: [Loaded from String]");
    }
    else 
    {
      std::println("Error in preprocessShader Shader: [Loaded from String], won't be compiled!");
    }
  }
    
  void shader::set_uniform(const std::string& name, const mat4& value)
  {
    int loc = -1;
    auto uni_it = m_data->cache.find(name);
    if(uni_it != m_data->cache.end())
      loc = uni_it->second.location;
    else 
    {
      glCall(loc = glGetUniformLocation(m_data->program_id, name.c_str()));
      if(loc == -1)
        std::println("invalid uniform of type mat4 with name: {}", name);
      else 
        m_data->cache[name] = { loc, shader_data_type::mat4 };
    }
    glCall(glUniformMatrix4fv(loc, 1, 0, value_ptr(value)));
  }
  
  void shader::set_uniform(const std::string& name, const vec4& value)
  {
    int loc = -1;
    auto uni_it = m_data->cache.find(name);
    if(uni_it != m_data->cache.end())
      loc = uni_it->second.location;
    else 
    {
      glCall(loc = glGetUniformLocation(m_data->program_id, name.c_str()));
      if(loc == -1)
        std::println("invalid uniform of type float4 with name: {}", name);
      else 
        m_data->cache[name] = { loc, shader_data_type::float4 };
    }
    glCall(glUniform4fv(loc, 1, value_ptr(value)));
  }

  void shader::set_uniform(const std::string& name, const vec3& value)
  {
    int loc = -1;
    auto uni_it = m_data->cache.find(name);
    if(uni_it != m_data->cache.end())
      loc = uni_it->second.location;
    else 
    {
      glCall(loc = glGetUniformLocation(m_data->program_id, name.c_str()));
      if(loc == -1)
        std::println("invalid uniform of type float3 with name: {}", name);
      else 
        m_data->cache[name] = { loc, shader_data_type::float4 };
    }
    glCall(glUniform3fv(loc, 1, value_ptr(value)));
  }
  
  void shader::set_uniform(const std::string& name, const vec2& value)
  {
    int loc = -1;
    auto uni_it = m_data->cache.find(name);
    if(uni_it != m_data->cache.end())
      loc = uni_it->second.location;
    else 
    {
      glCall(loc = glGetUniformLocation(m_data->program_id, name.c_str()));
      if(loc == -1)
        std::println("invalid uniform of type float2 with name: {}", name);
      else 
        m_data->cache[name] = { loc, shader_data_type::float2 };
    }
    glCall(glUniform2fv(loc, 1, value_ptr(value)));
  }

  void shader::set_uniform(const std::string& name, int value)
  {
    int loc = -1;
    auto uni_it = m_data->cache.find(name);
    if(uni_it != m_data->cache.end())
      loc = uni_it->second.location;
    else 
    {
      glCall(loc = glGetUniformLocation(m_data->program_id, name.c_str()));
      if(loc == -1)
        std::println("invalid uniform of type int1 with name: {}", name);
      else 
        m_data->cache[name] = { loc, shader_data_type::int1 };
    }
    glCall(glUniform1i(loc, value));
  }
  
  void shader::set_uniform(const std::string& name, const int* value, uint32_t count)
  {
    int loc = -1;
    auto uni_it = m_data->cache.find(name);
    if(uni_it != m_data->cache.end())
      loc = uni_it->second.location;
    else 
    {
      glCall(loc = glGetUniformLocation(m_data->program_id, name.c_str()));
      if(loc == -1)
        std::println("invalid uniform of type int array with name: {}", name);
      else 
        m_data->cache[name] = { loc, shader_data_type::int1 };
    }
    glCall(glUniform1iv(loc, count, value));
  }

  bool shader::preprocess(std::istringstream& ss, std::string& vs, std::string& fs)
  {
    std::string line;
    std::stringstream shader[2];
    enum {
      _none = -1, _vertex, _fragment
    } current_shader = _none;
    
    while(std::getline(ss, line))
    {
      if(line.find("#ODEER_SHADER_TYPE") != std::string::npos)
      {
        if(line.find("ODEER_VERTEX") != std::string::npos)
          current_shader = _vertex;
        else if(line.find("ODEER_FRAGMENT") != std::string::npos)
          current_shader = _fragment;
      }
      else if(current_shader != _none)
        shader[current_shader] << line << "\n";
    }

    vs = shader[_vertex].str();
    fs = shader[_fragment].str();
    return true;
  }

  bool shader::compile(const std::string& vs, const std::string& fs)
  {
    bool status = true;
    const char *v_cstr = vs.c_str(), *f_cstr = fs.c_str();
    int status_v, status_f;
    
    m_data->vertex_id = glCall(glCreateShader(GL_VERTEX_SHADER));
    m_data->fragment_id = glCall(glCreateShader(GL_FRAGMENT_SHADER));

    glCall(glShaderSource(m_data->vertex_id, 1, &v_cstr, nullptr));
    glCall(glCompileShader(m_data->vertex_id));
    glCall(glGetShaderiv(m_data->vertex_id, GL_COMPILE_STATUS, &status_v));
    if(status_v == GL_FALSE)
    {
      std::string shader_type;
      int mlength;
      glCall(glGetShaderiv(m_data->vertex_id, GL_INFO_LOG_LENGTH, &mlength));
      char* message = (char*)alloca(mlength * sizeof(char));
      glCall(glGetShaderInfoLog(m_data->vertex_id, mlength, &mlength, message));
      std::println("Error Compiling Vertex Shader: {}", message);
      status = false;
    }

    glCall(glShaderSource(m_data->fragment_id, 1, &f_cstr, nullptr));
    glCall(glCompileShader(m_data->fragment_id));
    glCall(glGetShaderiv(m_data->fragment_id, GL_COMPILE_STATUS, &status_f));
    if(status_f == GL_FALSE)
    {
      int mlength;
      glCall(glGetShaderiv(m_data->fragment_id, GL_INFO_LOG_LENGTH, &mlength));
      char* message = (char*)alloca(mlength * sizeof(char));
      glCall(glGetShaderInfoLog(m_data->fragment_id, mlength, &mlength, message));
      std::println("Error Compiling Fragment Shader: {}", message);
      status = false;
    }
    return status;
  }

  bool shader::create_program()
  {
    bool _status = true;
    int status;
    m_data->program_id = glCall(glCreateProgram());
    std::println("gen prog id: {}", m_data->program_id);
    glCall(glAttachShader(m_data->program_id, m_data->vertex_id));
    glCall(glAttachShader(m_data->program_id, m_data->fragment_id));
    glCall(glLinkProgram(m_data->program_id));
    glCall(glGetProgramiv(m_data->program_id, GL_LINK_STATUS, &status));
	  if(status == GL_FALSE)
	  {
	  	int mlength;
	  	glCall(glGetProgramiv(m_data->program_id, GL_INFO_LOG_LENGTH, &mlength));
	  	char* message = (char*) alloca(mlength * sizeof(char));
	  	glCall(glGetProgramInfoLog(m_data->program_id, mlength, &mlength, message));
	  	m_data->program_id = 0;
	    _status = false;
    }
    return _status;
  }
}
