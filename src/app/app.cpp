#include "app.hpp"
#include "app_events.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "math/math.hpp"
#include "renderer/api/buffer_layout.hpp"
#include "renderer/api/index_buffer.hpp"
#include "renderer/api/render_context.hpp"
#include "renderer/api/render_api.hpp"
#include "renderer/api/vertex_array.hpp"
#include "renderer/api/vertex_buffer.hpp"
#include "renderer/api/shader.hpp"
#include "utils/utils.hpp"
#include "window/window.hpp"
#include <memory>
#include <print>





std::string src = R"(
#ODEER_SHADER_TYPE ODEER_VERTEX
#version 330 core
layout(location = 0) in vec3 a_pos;
void main() {
  gl_Position = vec4(a_pos, 1.0);
}
#ODEER_SHADER_TYPE ODEER_FRAGMENT
#version 330 core
out vec4 color;
void main() {
  color = vec4(1.0,1.0,0.0,1.0);
}
)";


namespace odeer 
{



class Ortho 
{
public:
  Ortho(float left, float right, float bottom, float top, glm::vec3 pos = glm::vec3(0.0f))
    : m_pos(pos)
  {
    setRect(left, right, bottom, top);
  }
  Ortho()
  {

  }

  void setRect(float left, float right, float bottom, float top)
  {
    m_left = left;
    m_right = right;
    m_bottom = bottom;
    m_top = top;
  }

  inline glm::vec4 getRect() 
  {
    return glm::vec4(m_left, m_right, m_bottom, m_top);
  }

  void onUpdate(float delta);
  inline const glm::mat4& getView() const
  {
    m_view = glm::mat4(1.0f); 
    m_view = glm::rotate(m_view, glm::radians(m_rotation), glm::vec3(0, 0, 1)); 
    
    m_view = glm::translate(m_view, m_pos);
    m_view = glm::inverse(m_view);

    return m_view;
  }

  inline const glm::mat4& getProj() const
  {
    m_proj = glm::orthoRH(m_left * m_zoom, m_right * m_zoom, m_bottom * m_zoom, m_top * m_zoom, -100.0f, 100.0f);
    return m_proj;
  }

  inline void setPosition(glm::vec3 pos)
  {
    m_pos = pos;
  }

  inline const glm::vec3& getPosition() const
  {
    return m_pos;
  }

  inline void setZoom(float zoom)
  {
    m_zoom = zoom;
  }

  inline float getZoom() const
  {
    return m_zoom;
  }

  inline void setRotation(float angle)
  {
    m_rotation = angle;
  }

  inline float getRotation() const
  {
    return m_rotation;
  }
private:
  float m_left = -1.0f, m_right = 1.0f, m_bottom = -1.0f, m_top = 1.0f;
  float m_zoom = 1.0f;
  float m_rotation = 0.0f;
  glm::vec3 m_pos = { 0.0f, 0.0f, 0.0f };
  mutable glm::mat4 m_proj{1};
  mutable glm::mat4 m_view{1};
};


  void app::run() 
  {
    Ortho cam;
    float vertices[] = {
    // positions       
     0.5f,  0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f
  };
    uint32_t indices [6] =
    {
      0, 1, 2,
      2, 3, 0
    };

    
    window::init();
    window_specs specs;
    window win(specs);
    render_api::init();

    auto varr = std::make_shared<vertex_array>();
    varr->bind();
    auto vbuff = std::make_shared<vertex_buffer>(sizeof(vertices), vertices);
    auto ibuff = std::make_shared<index_buffer>(6, indices, index_buffer_usage::static_draw);
    auto _shader = std::make_shared<shader>();
    _shader->load_from_string(src);
    buffer_layout bl = {
      { "pos", shader_data_type::float3 }
    };
    vbuff->set_layout(bl);
    varr->bind();
    varr->add_vertex_buffer(vbuff);
    varr->set_index_buffer(ibuff);
    


    bool running = true;
    while(running) {
      event ev;
      while (poll_event(ev))
      {
        std::println("ev: {}", ev.type);
        if(ev.type == to_utype(event::type::application_close))
          running = false;
      }
      mat4 trans{1};
      trans = glm::translate(trans, {0,0,0});
      trans = glm::scale(trans, {1280, 720, 1});
      render_api::set_clear_color({1, 0, 1, 1});
      render_api::clear();
      _shader->bind();
      varr->bind();
      render_api::draw_indexed(varr);
      auto gc = win.get_graphics_context();
      auto err = gc->swap_buffers();
      if(!err.has_value())
        std::println("error: {}", render_context::error_to_string(err.error()));
    }
    render_api::shutdown();
  }
}
