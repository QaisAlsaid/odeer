#include "render_api.hpp"

#include "math/math.hpp"
#include "render_core.hpp"

namespace odeer 
{
  struct render_api::data 
  {
    vec4 clear_color{ 0.0f, 0.0f, 0.0f, 1.0f };
  };

  render_api::data* render_api::s_data = nullptr;

  void render_api::init()
  {
    s_data = new data();
  }

  void render_api::shutdown()
  {
    delete s_data;
    s_data = 0;
  }

  void render_api::set_clear_color(const vec4& color) 
  {
    s_data->clear_color = color;
    glCall(glClearColor(color.r, color.g, color.b, color.a));
  }

  vec4 render_api::get_clear_color()
  {
    return s_data->clear_color;
  }

  void render_api::clear()
  {
    //TODO: buffer options
    glCall(glClear(GL_COLOR_BUFFER_BIT));
  }

  void render_api::set_viewport(const vec4i& vp)
  {
    glCall(glViewport(vp.x, vp.y, vp.z, vp.w));
  }

  void render_api::set_blend_mode(blend_mode b) 
  {
    switch(b)
    {
      case blend_mode::alpha:
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        break;
      case blend_mode::additive:
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        break;
      case blend_mode::multiplicative:
        glBlendFunc(GL_ZERO, GL_SRC_COLOR);
        break;
    }
  }

  void render_api::enable_blend(bool enable) 
  {
    if(enable)
    {
      glCall(glEnable(GL_BLEND));
    }
    else 
    {
      glCall(glDisable(GL_BLEND));
    }
  }

  void render_api::enable_depth_test(bool enable)
  {
    if(enable)
    {
      glCall(glEnable(GL_DEPTH_TEST));
    }
    else 
    {
      glCall(glDisable(GL_DEPTH_TEST));
    }
  }

  void render_api::enable_depth_writes(bool enable)
  {
    if(enable)
    {
      glDepthMask(GL_TRUE);
    }
    else 
    {
      glDepthMask(GL_FALSE);
    }
  }

  void render_api::set_depth_fun(depth_fun fun) 
  {
    switch(fun)
    {
      case depth_fun::never:
        glDepthFunc(GL_NEVER);
      case depth_fun::less:
        glDepthFunc(GL_LESS);
      case depth_fun::equal:
        glDepthFunc(GL_EQUAL);
      case depth_fun::less_equal:
        glDepthFunc(GL_LEQUAL);
      case depth_fun::greater:
        glDepthFunc(GL_GREATER);
      case depth_fun::not_equal:
        glDepthFunc(GL_NOTEQUAL);
      case depth_fun::greater_equal:
        glDepthFunc(GL_GEQUAL);
      case depth_fun::always:
        glDepthFunc(GL_ALWAYS);
    }
  }

  void render_api::draw_indexed(const std::shared_ptr<vertex_array>& varr, uint32_t index_count, uint32_t offset) 
  {
    const uint32_t count = index_count == 0 ? varr->get_index_buffer()->get_count() : index_count;
    varr->bind();
    glCall(glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, (void*)(offset * sizeof(GLuint))));
  }

  void render_api::draw_arrays(draw_mode mode, uint32_t first, uint32_t count) 
  {
    GLenum gl_mode;
    switch(mode)
    {
      case draw_mode::line_loop:
        gl_mode = GL_LINE_LOOP;
        break;
      case draw_mode::line_strip:
        gl_mode = GL_LINE_STRIP;
        break;
      case draw_mode::line_strip_adjacency:
        gl_mode = GL_LINE_STRIP_ADJACENCY;
        break;
      case draw_mode::lines_adjacency:
        gl_mode = GL_LINES_ADJACENCY;
        break;
      case draw_mode::lines:
        gl_mode = GL_LINES;
        break;
      case draw_mode::points:
        gl_mode = GL_POINTS;
        break;
      case draw_mode::patches:
        //TODO: wtf gl_mode = GL_PATCHES;
        break;
      case draw_mode::triangle_fan:
        gl_mode = GL_TRIANGLE_FAN;
        break;
      case draw_mode::triangle_strip:
        gl_mode = GL_TRIANGLE_STRIP;
        break;
      case draw_mode::triangle_strip_adjacency:
        gl_mode = GL_TRIANGLE_STRIP_ADJACENCY;
        break;
      case draw_mode::triangles_adjacency:
        gl_mode = GL_TRIANGLES_ADJACENCY;
        break;
      case draw_mode::triangles:
        gl_mode = GL_TRIANGLES;
        break;
    }

    glCall(glDrawArrays(gl_mode, first, count));
  }
}
