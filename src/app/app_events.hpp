#ifndef ODEER_APPLICATION_EVENTS_HPP
#define ODEER_APPLICATION_EVENTS_HPP

#include "key_codes.hpp"
#include "utils/utils.hpp"
#include "utils/macros.hpp"

#include <chrono>
#include <string_view>
#include <span>
#include <cstdint>
#include <variant>
#include <expected>

union SDL_Event;

namespace odeer 
{
  namespace events
  {
    struct application_close
    {

    };

    struct window_visibility
    {
      bool shown;
    };

    struct window_move
    {
      int32_t position1;
      int32_t position2;
    };

    struct window_resize
    {
      int32_t size1;
      int32_t size2;
    };

    struct window_pixel_resize
    {
      int32_t size1;
      int32_t size2;
    };

    struct window_iconify
    {
      enum class type
      {
        minimize, maximize, restore
      };
      type state;
    };

    struct window_mouse
    {
      bool enter;
    };

    struct window_focus
    {
      bool gain;
    };

    struct window_fullscreen
    {
      bool enter;
    };

    struct locale
    {

    };

    struct theme
    {

    };

    struct keyboard
    {
      enum class type
      {
        key_press, key_release
      };
      type state;
      key key_code;
      mod_key key_mod;
      bool press;
      bool repeat;
    };

    struct text_input
    {
      std::string text;
    };

    struct mouse_move
    {
      mouse_id mouse;
      mouse_buttons_state buttons;
      float x;
      float y;
      float dx;
      float dy;
    };

    struct mouse_button
    {
      enum class type
      {
        button_press, button_release
      };
      type state;
      mouse_id mouse;
      button button_code;
      uint8_t clicks;
      float x;
      float y;
      bool press;
    };

    struct mouse_wheel
    {
      mouse_id mouse;
      float dx;
      float dy;
      float x;
      float y;
    };

    struct drop
    {
      enum class type
      {
        begin, file, text, position, end
      };
      type state;
      float x;
      float y;
      std::string src;
      std::string data;
    };

    struct clipboard
    {
      std::span<std::string_view> data;
    };

    struct generic
    {
      uint32_t code;
      void* data1;
      void* data2;
    };
  }

  enum class event_error
  {
    generic_event_registeration_failure, //from: register_generic_events
    general_event_failure, //from: push_event, poll_event, wait_event
    non_generic_event_push //from: push_event
  };

  class ODEER_API event
  {
  public:
    enum class type
    {
      application_close,
      window_visibility, window_move, window_resize,
      window_pixel_resize, window_iconify, window_mouse,
      window_focus, window_fullscreen,
      locale, theme,
      keyboard, text_input,
      mouse_move, mouse_button, mouse_wheel,
      drop, clipboard,
      generic, unknown
    };
    using event_variant = std::variant<events::application_close,
                                       events::window_visibility,
                                       events::window_move,
                                       events::window_resize,
                                       events::window_pixel_resize,
                                       events::window_iconify,
                                       events::window_mouse,
                                       events::window_focus,
                                       events::window_fullscreen,
                                       events::locale,
                                       events::theme,
                                       events::keyboard,
                                       events::text_input,
                                       events::mouse_move,
                                       events::mouse_button,
                                       events::mouse_wheel,
                                       events::drop,
                                       events::clipboard,
                                       events::generic>;

  static_assert(std::variant_size_v<event_variant> ==
    static_cast<std::size_t>(event::type::generic) + 1,
    "Mismatch between event::type and event_variant");
  public:
    event() = default;
    event(SDL_Event& sdl_event);
  public:
    event_variant variant;
    uint32_t type = to_utype(type::unknown);
    uint32_t window_id;
    std::chrono::nanoseconds time_stamp_ns;
    bool handled{false};
  private:
    event_variant convert_key_event(SDL_Event&);
    event_variant convert_mouse_move_event(SDL_Event&);
    event_variant convert_mouse_button_event(SDL_Event&);
    event_variant convert_mouse_wheel_event(SDL_Event&);
    event_variant convert_drop_event(SDL_Event&);
  };

  ODEER_API std::expected<uint32_t, event_error> register_generic_events(uint32_t count = 1);
  ODEER_API std::expected<void, event_error> push_event(const event& event);
  ODEER_API bool poll_event(event&);

  ODEER_API bool wait_event_ms(event& e, int timeout_ms);

  template <typename Rep, typename Period>
  ODEER_API inline bool wait_event(event& e, const std::chrono::duration<Rep, Period>& timeout)
  {
    int wait = std::chrono::duration_cast<std::chrono::milliseconds>(timeout).count();
    return wait_event_ms(e, wait);
  }
} //namespace odeer

#endif //ODEER_APPLICATION_EVENTS_HPP
