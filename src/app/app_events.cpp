#include "app_events.hpp"

#include <chrono>
#include <expected>
#include <vector>

#include <SDL3/SDL_events.h>

namespace odeer
{
  event::event(SDL_Event& sdl_event)
  {
    time_stamp_ns = std::chrono::nanoseconds(sdl_event.common.timestamp);
    window_id = 0;

    switch(sdl_event.type)
    {
      case SDL_EVENT_QUIT:
      {
        type = to_utype(type::application_close);
        return;
      }
      case SDL_EVENT_WINDOW_HIDDEN:
      {
        type = to_utype(type::window_visibility);
        window_id = sdl_event.window.windowID;
        variant.emplace<events::window_visibility>(false);
        return;
      }
      case SDL_EVENT_WINDOW_SHOWN:
      {
        type = to_utype(type::window_visibility);
        window_id = sdl_event.window.windowID;
        variant.emplace<events::window_visibility>(true);
      }
      case SDL_EVENT_WINDOW_MOVED:
      {
        type = to_utype(type::window_move);
        window_id = sdl_event.window.windowID;
        variant.emplace<events::window_move>(sdl_event.window.data1, sdl_event.window.data2);
        return;
      }
      case SDL_EVENT_WINDOW_RESIZED:
      {
        type = to_utype(type::window_resize);
        window_id = sdl_event.window.windowID;
        variant.emplace<events::window_resize>(sdl_event.window.data1, sdl_event.window.data2);
        return;
      }
      case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
      {
        type = to_utype(type::window_pixel_resize);
        window_id = sdl_event.window.windowID;
        variant.emplace<events::window_pixel_resize>(sdl_event.window.data1, sdl_event.window.data2);
        return;
      }
      case SDL_EVENT_WINDOW_MINIMIZED:
      {
        type = to_utype(type::window_iconify);
        window_id = sdl_event.window.windowID;
        variant.emplace<events::window_iconify>(events::window_iconify::type::minimize);
        return;
      }
      case SDL_EVENT_WINDOW_MAXIMIZED:
      {
        type = to_utype(type::window_iconify);
        window_id = sdl_event.window.windowID;
        variant.emplace<events::window_iconify>(events::window_iconify::type::maximize);
        return;
      }
      case SDL_EVENT_WINDOW_RESTORED:
      {
        type = to_utype(type::window_iconify);
        window_id = sdl_event.window.windowID;
        variant.emplace<events::window_iconify>(events::window_iconify::type::restore);
        return;
      }
      case SDL_EVENT_WINDOW_MOUSE_ENTER:
      {
        type = to_utype(type::window_mouse);
        window_id = sdl_event.window.windowID;
        variant.emplace<events::window_mouse>(true);
        return;
      }
      case SDL_EVENT_WINDOW_MOUSE_LEAVE:
      {
        type = to_utype(type::window_mouse);
        window_id = sdl_event.window.windowID;
        variant.emplace<events::window_mouse>(false);
        return;
      }
      case SDL_EVENT_WINDOW_FOCUS_GAINED:
      {
        type = to_utype(type::window_focus);
        window_id = sdl_event.window.windowID;
        variant.emplace<events::window_focus>(true);
        return;
      }
      case SDL_EVENT_WINDOW_FOCUS_LOST:
      {
        type = to_utype(type::window_focus);
        window_id = sdl_event.window.windowID;
        variant.emplace<events::window_focus>(false);
        return;
      }
      case SDL_EVENT_WINDOW_ENTER_FULLSCREEN:
      {
        type = to_utype(type::window_fullscreen);
        window_id = sdl_event.window.windowID;
        variant.emplace<events::window_fullscreen>(true);
        return;
      }
      case SDL_EVENT_WINDOW_LEAVE_FULLSCREEN:
      {
        type = to_utype(type::window_fullscreen);
        window_id = sdl_event.window.windowID;
        variant.emplace<events::window_fullscreen>(false);
        return;
      }
      case SDL_EVENT_LOCALE_CHANGED:
      {
        type = to_utype(type::locale);
        //TODO: see what is this event's data is
        variant.emplace<events::locale>();
        return;
      }
      case SDL_EVENT_SYSTEM_THEME_CHANGED:
      {
        type = to_utype(type::theme);
        //TODO: see what is this event's data is
        variant.emplace<events::theme>();
        return;
      }
      case SDL_EVENT_KEY_UP:
      case SDL_EVENT_KEY_DOWN:
      {
        type = to_utype(type::keyboard);
        window_id = sdl_event.key.windowID;
        variant = convert_key_event(sdl_event);
        return;
      }
      case SDL_EVENT_TEXT_INPUT:
      {
        type = to_utype(type::text_input);
        window_id = sdl_event.key.windowID;
        variant.emplace<events::text_input>(sdl_event.text.text);
        return;
      }
      case SDL_EVENT_MOUSE_MOTION:
      {
        type = to_utype(type::mouse_move);
        window_id = sdl_event.motion.windowID;
        variant = convert_mouse_move_event(sdl_event);
        return;
      }
      case SDL_EVENT_MOUSE_BUTTON_DOWN:
      case SDL_EVENT_MOUSE_BUTTON_UP:
      {
        type = to_utype(type::mouse_button);
        window_id = sdl_event.button.windowID;
        variant = convert_mouse_button_event(sdl_event);
        return;
      }
      case SDL_EVENT_MOUSE_WHEEL:
      {
        type = to_utype(type::mouse_wheel);
        window_id = sdl_event.wheel.windowID;
        variant = convert_mouse_wheel_event(sdl_event);
        return;
      }
      case SDL_EVENT_DROP_BEGIN:
      case SDL_EVENT_DROP_FILE:
      case SDL_EVENT_DROP_TEXT:
      case SDL_EVENT_DROP_POSITION:
      case SDL_EVENT_DROP_COMPLETE:
      {
        type = to_utype(type::drop);
        window_id = sdl_event.drop.windowID;
        variant = convert_drop_event(sdl_event);
        return;
      }
      case SDL_EVENT_CLIPBOARD_UPDATE:
      {
        type = to_utype(type::clipboard);
        //since string_view(s) don't copy the underlying data it's ok to do so,
        //but it's a bit slower than passing char**, but im ok with that,
        //the benefits of span and string_view outweigh the cost
        std::vector<std::string_view> vec(sdl_event.clipboard.num_mime_types);
        for(auto i = 0; i < sdl_event.clipboard.num_mime_types; i++)
        {
          vec[i] = sdl_event.clipboard.mime_types[i];
        }
        std::span<std::string_view> span{vec};
        variant.emplace<events::clipboard>(span);
        return;
      }
    }
    if(sdl_event.type >= SDL_EVENT_USER) [[likely]]
    {
      type = sdl_event.type;
      window_id = sdl_event.user.windowID;
      variant.emplace<events::generic>(sdl_event.user.code, sdl_event.user.data1, sdl_event.user.data2);
      return;
    }

    
    type = to_utype(type::unknown);
  }

  inline static constexpr key sdl3_to_amaterasu_key_code(uint32_t sdl_key)
  {
    return static_cast<key>(sdl_key);
  }
  inline static constexpr mod_key sdl3_to_amaterasu_key_mod(uint16_t sdl_mod)
  {
    return static_cast<mod_key>(sdl_mod);
  }
  inline static constexpr button sdl3_to_amaterasu_button(uint8_t sdl_button)
  {
    return static_cast<button>(sdl_button);
  }



  //these functions invoke a copy ctor insted of emplacing directly
  //TODO: fix it after setting up each event type properly

  auto event::convert_key_event(SDL_Event& e) -> event_variant
  {
    event_variant variant;
    events::keyboard::type state;
    if(e.type == SDL_EVENT_KEY_DOWN)
      state = events::keyboard::type::key_press;
    variant.emplace<events::keyboard>(events::keyboard{.state = state,
                                                        .key_code = sdl3_to_amaterasu_key_code(e.key.key),
                                                        .key_mod = sdl3_to_amaterasu_key_mod(e.key.mod),
                                                        .press = e.key.down,
                                                        .repeat = e.key.repeat});
    return variant;
  }

  auto event::convert_mouse_move_event(SDL_Event& e) -> event_variant
  {
    event_variant variant;
    variant.emplace<events::mouse_move>(events::mouse_move{.mouse = e.motion.which,
                                                           .buttons = e.motion.state,
                                                           .x = e.motion.x,
                                                           .y = e.motion.y,
                                                           .dx = e.motion.xrel,
                                                           .dy = e.motion.yrel});
    return variant;
  }

  auto event::convert_mouse_button_event(SDL_Event& e) -> event_variant
  {
    event_variant variant;
    events::mouse_button::type state;
    if(e.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
      state = events::mouse_button::type::button_press;

    variant.emplace<events::mouse_button>(events::mouse_button{.state = state,
                                                               .mouse = e.button.which,
                                                               .button_code = sdl3_to_amaterasu_button(e.button.button),
                                                               .clicks = e.button.clicks,
                                                               .x = e.button.x,
                                                               .y = e.button.y,
                                                               .press = e.button.down});
    return variant;
  }

  auto event::convert_mouse_wheel_event(SDL_Event& e) -> event_variant
  {
    event_variant variant;
    variant.emplace<events::mouse_wheel>(e.wheel.which, e.wheel.x, e.wheel.y, e.wheel.mouse_x, e.wheel.mouse_y);
    return variant;
  }

  auto event::convert_drop_event(SDL_Event& e) -> event_variant
  {
    event_variant variant;
    events::drop::type state;
    bool _free = false;
    switch(e.type)
    {
      case SDL_EVENT_DROP_BEGIN:
      {
        state = events::drop::type::begin;
        break;
      }
      case SDL_EVENT_DROP_FILE:
      {
        state = events::drop::type::file;
        _free = true;
        break;
      }
      case SDL_EVENT_DROP_TEXT:
      {
        state = events::drop::type::text;
        _free = true;
        break;
      }
      case SDL_EVENT_DROP_POSITION:
      {
        state = events::drop::type::position;
        break;
      }
      case SDL_EVENT_DROP_COMPLETE:
      {
        state = events::drop::type::end;
        break;
      }
    }

    variant.emplace<events::drop>(events::drop{.state = state,
                                               .x = e.drop.x,
                                               .y = e.drop.y,
                                               .src = e.drop.source,
                                               .data = e.drop.data});
    return variant;
  }

  static std::expected<SDL_Event, event_error> to_sdl_event(const event& ev);

  

  std::expected<uint32_t, event_error> register_generic_events(uint32_t count)
  {
    auto ret = SDL_RegisterEvents(count);
    if(ret == (uint32_t) - 1)
      return std::unexpected(event_error::generic_event_registeration_failure);

    return ret;

    return 0;
  }

  std::expected<void, event_error> push_event(const event& event)
  {
    auto exp = to_sdl_event(event);
    if(!exp.has_value())
      return std::unexpected(exp.error());

    auto ret = SDL_PushEvent(&exp.value());
    if(!ret)
      return std::unexpected(event_error::general_event_failure);

    return {};
  }

  bool poll_event(event& e)
  {
    SDL_Event sdl_event;
    bool ret = SDL_PollEvent(&sdl_event);
    e = { sdl_event };
    return ret;
  }

  bool wait_event_ms(event& e, int timeout_ms)
  {
    SDL_Event sdl_event;
    bool ret = SDL_WaitEventTimeout(&sdl_event, timeout_ms);
    e = { sdl_event };
    return ret;
  }

  static std::expected<SDL_Event, event_error> to_sdl_event(const event& event)
  {
    if(event.type < to_utype(event::type::generic))
      return std::unexpected(event_error::non_generic_event_push);

    auto generic = std::get<events::generic>(event.variant);
    SDL_Event sdl_event;
    sdl_event.user.type = event.type;
    sdl_event.user.code = static_cast<int>(generic.code);
    sdl_event.user.data1 = generic.data1;
    sdl_event.user.data2 = generic.data2;

    return sdl_event;
  }
} //namespace odeer
