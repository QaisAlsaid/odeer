#include "render_context.hpp"
#include "render_core.hpp"
#include "SDL3/SDL_video.h"

namespace odeer 
{
  struct render_context::data 
  {
    SDL_Window* window;
    SDL_GLContext context;
  };

  render_context::~render_context() 
  {
    if(m_data && m_data->window && m_data->context)
      SDL_GL_DestroyContext(m_data->context);
    
    m_data->context = nullptr;
    m_data->window = nullptr;
    delete m_data;
    m_data = nullptr;
  }

  auto render_context::init(SDL_Window* window) -> std::expected<void, error>
  {
    m_data = new data();
    m_data->window = window;
    if(!m_data->window)
      return std::unexpected(error::invalid_window);

    m_data->context = SDL_GL_CreateContext(m_data->window);
    if(!m_data->context)
      return std::unexpected(error::context_creation_failure);

    SDL_GL_MakeCurrent(m_data->window, m_data->context);
    if(!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
      return std::unexpected(error::proc_loading_failure);
      SDL_GL_DestroyContext(m_data->context);
      SDL_DestroyWindow(m_data->window);
    }
    return {};
  }

  auto render_context::make_current() -> std::expected<void, error> 
  {
    if(!m_data) 
      return std::unexpected(error::uninitialized);
    if(!m_data->window)
      return std::unexpected(error::invalid_window);
    if(!m_data->context)
      return std::unexpected(error::invalid_context);

    SDL_GL_MakeCurrent(m_data->window, m_data->context);
    return {};
  }

  auto render_context::swap_buffers() -> std::expected<void, error>
  {
    if(!m_data) 
      return std::unexpected(error::uninitialized);
    if(!m_data->window)
      return std::unexpected(error::invalid_window);
    if(!m_data->context)
      return std::unexpected(error::invalid_context);

    SDL_GL_SwapWindow(m_data->window);
    return {};
  }

  auto render_context::set_swap_interval(int interval) -> std::expected<void, error> 
  {
    auto sdl_ret = SDL_GL_SetSwapInterval(interval);

    if(sdl_ret == 0)
      return std::unexpected(error::swap_interval_unsupported);

    return {};
  }
}
