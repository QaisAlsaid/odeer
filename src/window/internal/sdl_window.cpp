#include "sdl_window.hpp"

#include <SDL3/SDL.h>
#include <print>
#include <string_view>

//TODO: maybe remove the separation betwen s_sdl_init and s_init

namespace odeer
{
  std::atomic<bool> window::s_init = false;

  static SDL_WindowFlags get_sdl_flag(window_options opt);

  void window::init()
  {
    //subscribe to configuration
    //configuration::get_event_bus().subscribe([](configuration::event ev) -> bool 
    //{
    //  if(ev == configuration::event::read)
    //    read_config();
    //  else 
    //    write_config();

    //  return false;
    //});

    //init sdl
    if(!s_init)
    {
      if(!SDL_InitSubSystem(SDL_INIT_VIDEO))
        std::println("failed to initlize SDL3: {}", SDL_GetError());
      else 
        s_init = true;
    }
    else 
      s_init = true;
  }

  void window::shutdown()
  {
    s_init = false;
  }

  window::window(const window_specs& specs, window* shared_context)
  {
    ODEER_ASSERT((s_sdl_init && s_init), "window system isn't initilized!");
    
    SDL_WindowFlags sdl_flags;
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    sdl_flags = SDL_WINDOW_OPENGL; 

    for(auto i = 0; i < to_utype(window_options::internal_count); i++)
    {
      if(specs.bit_flags.test(i))
      {
        auto flag = get_sdl_flag(static_cast<window_options>(i));
        if(flag != 0)
          sdl_flags |= flag;
      }
    }

    if(sdl_flags & SDL_WINDOW_OPENGL)
      std::println("window opengl is set");

    m_sdl_window = SDL_CreateWindow(specs.title.c_str(), specs.width, specs.height, sdl_flags);
    if(!m_sdl_window)
      std::println("error creating sdl window: {}", SDL_GetError());
   
    m_context = std::make_shared<render_context>();
    auto val = m_context->init(m_sdl_window);
    if(!val.has_value())
      std::println("failed to create a graphics context: {}", render_context::error_to_string(val.error()));
  }

  window::~window()
  {
    SDL_DestroyWindow(m_sdl_window);
  }

  vec2u window::get_size() const
  {
    int w, h;
    SDL_GetWindowSizeInPixels(m_sdl_window, &w, &h);
    return { w, h };
  }

  bool window::set_option(window_options opt, bool state)
  {
    switch(opt)
    {
      case window_options::resizable: 
      {
        return SDL_SetWindowResizable(m_sdl_window, state);
      }
      case window_options::borderless:
      {
        return SDL_SetWindowBordered(m_sdl_window, !state);
      }
      case window_options::focused:
      {
        return SDL_SetWindowFocusable(m_sdl_window, state);
      }
      case window_options::hidden:
      case window_options::maximized:
      case window_options::minimized:
      case window_options::transparent_frame_buffer:
      {
        return false;
      }
      case window_options::fullscreen:
      {
        return SDL_SetWindowFullscreen(m_sdl_window, state);
      }
      default: 
      {
        return false;
      }
    }
    std::println("invalid window_option: {}", to_utype(opt));
    return false;
  }

  bool window::set_size(vec2u new_size, bool block)
  {
    auto status = SDL_SetWindowSize(m_sdl_window, new_size.x, new_size.y);
    if(status && block)
      SDL_SyncWindow(m_sdl_window);
    return status;
  }

  bool window::set_title(std::string_view new_title)
  {
    return SDL_SetWindowTitle(m_sdl_window, new_title.data());
  }

  bool window::set_parent(const window& window)
  {
    return SDL_SetWindowParent(m_sdl_window, window.m_sdl_window);
  }

  static SDL_WindowFlags get_sdl_flag(window_options opt)
  {
    switch(opt)
    {
      case window_options::resizable: return SDL_WINDOW_RESIZABLE;
      case window_options::borderless: return SDL_WINDOW_BORDERLESS;
      case window_options::focused: return SDL_WINDOW_INPUT_FOCUS;
      case window_options::maximized: return SDL_WINDOW_MAXIMIZED;
      case window_options::minimized: return SDL_WINDOW_MINIMIZED;
      case window_options::hidden: return SDL_WINDOW_HIDDEN;
      case window_options::transparent_frame_buffer: return SDL_WINDOW_TRANSPARENT;
      case window_options::fullscreen: return SDL_WINDOW_FULLSCREEN;
      default: return 0; //invalid option
    }
  }

  //no window config yet...
  //maybe default window options should be configureable ?!
  //TODO: logger settings
  void window::read_config()
  {
  }

  void window::write_config()
  {

  } 

#if defined(AMATERASU_WINDOW_ENABLE_NLOHMANN_JSON_CONVERT)
 

#endif //defined(AMATERASU_WINDOW_ENABLE_NLOHMANN_JSON_CONVERT)

} //namespace amaterasu
