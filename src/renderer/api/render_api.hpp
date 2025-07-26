#ifndef ODEER_RENDER_API_HPP
#define ODEER_RENDER_API_HPP


#include "utils/macros.hpp"
#include "math/math.hpp"
#include "vertex_array.hpp"

#include <memory>

namespace odeer 
{
  enum class blend_mode
  {
    alpha, // std
    additive, // src + dst
    multiplicative // src * dst
  };

  enum class depth_fun
  {
    never, 
    less,
    equal,
    less_equal,
    greater,
    not_equal,
    greater_equal,
    always
  };

  class ODEER_API render_api 
  {
  public:
    enum class api 
    {
      none = 0, opengl
    };
 
    struct config 
    {
      api _api;
    };

    enum class draw_mode 
    {
      points, lines, line_strip, line_loop, line_strip_adjacency, lines_adjacency,
      triangles, triangle_strip, triangle_fan, triangle_strip_adjacency, triangles_adjacency,
      patches
    };

  public:
    static void init();
    static void shutdown();

    //TODO: color class
    static void set_clear_color(const vec4& color);
    static void clear();
    
    // left - bottom - right - top
    static void set_viewport(const vec4i& vp);

    static vec4 get_clear_color();


    static void draw_indexed(const std::shared_ptr<vertex_array>& varr, 
        uint32_t index_count = 0, uint32_t offset = 0);

    static void draw_arrays(draw_mode mode, uint32_t first, uint32_t count);

    static void enable_blend(bool enable);
    static void set_blend_mode(blend_mode);

    static void enable_depth_test(bool enable);
    static void set_depth_fun(depth_fun);

    static void enable_depth_writes(bool enable);
  private:
    struct data;
    static data* s_data;
  };
} //namespace odeer

#endif //ODEER_RENDER_API_HPP
