#ifndef ODEER_RENDER_CONTEXT_HPP
#define ODEER_RENDER_CONTEXT_HPP

#include "utils/macros.hpp"
#include <expected>
#include <string_view>

struct SDL_Window;

namespace odeer
{
  class ODEER_API render_context
  {
  public:
    enum class error
    {
      invalid_window,
      invalid_context,
      context_creation_failure,
      proc_loading_failure,
      swap_interval_unsupported,
      uninitialized
    };
  public:
    ~render_context();
    
    std::expected<void, error> init(SDL_Window* window);
    std::expected<void, error> make_current();
    std::expected<void, error> swap_buffers();
    std::expected<void, error> set_swap_interval(int interval);

    
    static constexpr std::string_view error_to_string(error err)
    {
      using namespace std::string_view_literals;
      switch(err)
      {
        case error::invalid_window: return "invalid_window"sv;
        case error::invalid_context: return "invalid_context"sv;
        case error::context_creation_failure: return "context_creation_failure"sv;
        case error::proc_loading_failure: return "proc_loading_failure"sv;
        case error::swap_interval_unsupported: return "swap_interval_unsupported"sv;
        case error::uninitialized: return "uninitialized"sv;
      }
      return "unknown"sv;
    }
  private:
    struct data;
    data* m_data;
  };
} //namespace odeer

#endif //ODEER_RENDER_CONTEXT_HPP
