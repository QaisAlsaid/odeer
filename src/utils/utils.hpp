#ifndef ODEER_UTILS_HPP
#define ODEER_UTILS_HPP

#include <type_traits>

namespace odeer 
{
  template <typename ScopedEnum>
  requires std::is_scoped_enum_v<ScopedEnum>
  constexpr auto to_utype(ScopedEnum e)
  {
    return static_cast<std::underlying_type_t<ScopedEnum>>(e);
  }
} //namespace odeer

#endif //ODEER_UTILS_HPP
