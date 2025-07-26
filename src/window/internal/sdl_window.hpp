#ifndef ODEER_SDL3_WINDOW_HPP
#define ODEER_SDL3_WINDOW_HPP

#include <atomic>
#include <cstdint>
#include <bitset>
#include <memory>

#include "renderer/api/render_context.hpp"
#include "utils/macros.hpp"
#include "utils/utils.hpp"
#include "math/math.hpp"

struct SDL_Window;

/*
 * ! context sharing isn't supported
 * ! window icon isn't supported
 */

namespace odeer
{ 
  constexpr auto default_window_title = "odeer";
  constexpr auto default_window_width = 1280;
  constexpr auto default_window_height = 720;


  //TODO: modal
  enum class window_options
  {
    none = 0,
    resizable,
    borderless,
    focused,
    maximized,
    fullscreen,
    hidden,
    transparent_frame_buffer,
    minimized,
    internal_opengl,
    internal_count //keeps track of the number of options don't use
  };
  
  constexpr std::string_view window_options_to_string(window_options opt)
  {
    switch(opt)
    {
      case window_options::resizable:                return "resizable";
      case window_options::borderless:               return "borderless";
      case window_options::focused:                  return "focused";
      case window_options::maximized:                return "maximized";
      case window_options::minimized:                return "minimized";
      case window_options::hidden:                   return "hidden";
      case window_options::transparent_frame_buffer: return "transparent_frame_buffer";
      case window_options::fullscreen:               return "fullscreen";
      case window_options::internal_opengl:          return "internal_opengl";
      default:                                       return "none";
    }
  }
  using window_bit_flags = std::bitset<to_utype(window_options::internal_count)>; 


  struct window_specs
  {
    std::string title = default_window_title;
    uint32_t width = default_window_width,
             height = default_window_height;

    window_bit_flags bit_flags = make_default_options_bit_flags();

    static consteval std::bitset<to_utype(window_options::internal_count)> make_default_options_bit_flags()
    {
      std::bitset<to_utype(window_options::internal_count)> bit{};

      bit.set(to_utype(window_options::resizable), true);
      bit.set(to_utype(window_options::borderless), false);
      bit.set(to_utype(window_options::focused), true);
      bit.set(to_utype(window_options::maximized), false);
      bit.set(to_utype(window_options::hidden), false);
      bit.set(to_utype(window_options::transparent_frame_buffer), false);
      bit.set(to_utype(window_options::fullscreen), false);
      return bit;
    }
  };

  class ODEER_API window
  {
  public:
    static void init();
    static void shutdown();

    window(const window_specs& spec, window* shared_context = nullptr);
    virtual ~window();

    vec2u get_size() const;
    SDL_Window* get_native_window() const;

    bool set_option(window_options, bool state);
    bool set_size(vec2u new_size, bool block);
    bool set_title(std::string_view new_title);
    bool set_parent(const window& window);

    inline std::shared_ptr<render_context> get_graphics_context()
    {
      return m_context;
    }
  private:
    static void read_config();
    static void write_config();
  private:
    SDL_Window* m_sdl_window;
    std::shared_ptr<render_context> m_context;
    static std::atomic<bool> s_init;
  };
} //namespace amaterasu

#endif //AMATERASU_SDL3_WINDOW_HPP
