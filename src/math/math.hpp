#ifndef ODEER_MATH_HPP
#define ODEER_MATH_HPP

// #define AMATERASU_ENABLE_EXPERIMENTAL
// #if defined(AMATERASU_ENABLE_EXPERIMENTAL)
//   #define GLM_ENABLE_EXPERIMENTAL
//   #include <glm/ext.hpp>
//   #include <glm/gtx/string_cast.hpp> // Enabled for glm::to_string
// #endif

#include <glm/common.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/matrix.hpp>

// #define AMATERASU_MATH_ENABLE_NLOHMANN_JSON_CONVERT
// #if defined(AMATERASU_MATH_ENABLE_NLOHMANN_JSON_CONVERT)
//   #include "amaterasu/configuration/configuration.hpp"
// #endif

// #define AMATERASU_MATH_ENABLE_FMT_CONVERT
// #if defined(AMATERASU_MATH_ENABLE_FMT_CONVERT)
//   #include <spdlog/fmt/fmt.h> // Use spdlog's bundled fmt
//   #include <spdlog/fmt/ostr.h> // Enable ostream formatting
// #endif

namespace odeer::inline math
{
  // Vector
  template <size_t L, typename T>
  using vec = glm::vec<L, T>;

  using vec2 = glm::vec2;
  using vec3 = glm::vec3;
  using vec4 = glm::vec4;

  using vec2u = glm::uvec2;
  using vec3u = glm::uvec3;
  using vec4u = glm::uvec4;

  using vec2i = glm::ivec2;
  using vec3i = glm::ivec3;
  using vec4i = glm::ivec4;

  // Matrix
  template <size_t Col, size_t Row, typename T>
  using mat = glm::mat<Col, Row, T>;

  using mat2 = glm::mat2;
  using mat3 = glm::mat3;
  using mat4 = glm::mat4;

  template <size_t Col, size_t Row, typename T>
  inline constexpr mat<Col, Row, T> transpose(const mat<Col, Row, T>& mat)
  {
    return glm::transpose(mat);
  }

  template <typename T>
  inline constexpr mat<4, 4, T> translate(const mat<4, 4, T>& mat, const vec<3, T>& vec)
  {
    return glm::translate(mat, vec);
  }

  template <typename T>
  inline constexpr mat<4, 4, T> rotate(const mat<4, 4, T>& mat, T angle, const vec<3, T>& axis)
  {
    return glm::rotate(mat, angle, axis);
  }

  template <size_t L, typename T>
  inline constexpr const T* value_ptr(const vec<L, T>& v)
  {
    return glm::value_ptr(v);
  }

  template <size_t L, typename T>
  inline constexpr T* value_ptr(vec<L, T>& v)
  {
    return glm::value_ptr(v);
  }

  template <size_t Col, size_t Row, typename T>
  inline constexpr const T* value_ptr(const mat<Col, Row, T>& m)
  {
    return glm::value_ptr(m);
  }

  template <size_t Col, size_t Row, typename T>
  inline constexpr T* value_ptr(mat<Col, Row, T>& m)
  {
    return glm::value_ptr(m);
  }

  template <typename T>
  inline constexpr mat<4, 4, T> ortho(T left, T right, T bottom, T top, T near, T far)
  {
    return glm::ortho(left, right, bottom, top, near, far);
  }

  template <size_t Col, size_t Row, typename T>
  inline constexpr mat<Col, Row, T> inverse(const mat<Col, Row, T>& mat)
  {
    return glm::inverse(mat);
  }

  template <typename T>
  inline constexpr T min(T x, T y)
  {
    return glm::min(x, y);
  }

  template <typename T>
  inline constexpr T clamp(T x, T min, T max)
  {
    return glm::clamp(x, min, max);
  }

  // #if defined(AMATERASU_MATH_ENABLE_FMT_CONVERT)
  //   // ostream operator overloads for amaterasu::math types
  //   template<typename OStream, size_t L, typename T>
  //   inline OStream& operator<<(OStream& os, const vec<L, T>& vec)
  //   {
  //     return os << glm::to_string(vec);
  //   }

  //   template<typename OStream, size_t C, size_t R, typename T>
  //   inline OStream& operator<<(OStream& os, const mat<C, R, T>& mat)
  //   {
  //     return os << glm::to_string(mat);
  //   }

  //   // ostream operator overloads for glm types
  //   template<typename OStream, size_t L, typename T, glm::qualifier Q>
  //   inline OStream& operator<<(OStream& os, const glm::vec<L, T, Q>& vec)
  //   {
  //     return os << glm::to_string(vec);
  //   }

  //   template<typename OStream, size_t C, size_t R, typename T, glm::qualifier Q>
  //   inline OStream& operator<<(OStream& os, const glm::mat<C, R, T, Q>& mat)
  //   {
  //     return os << glm::to_string(mat);
  //   }
  // #endif
} // namespace odeer::inline math

// #if defined(AMATERASU_MATH_ENABLE_FMT_CONVERT)
// namespace fmt
// {
//   // Formatter for amaterasu::math::vec<L, T>
//   template <size_t L, typename T>
//   struct formatter<amaterasu::math::vec<L, T>, char, std::enable_if_t<std::is_arithmetic_v<T>>>
//   {
//     constexpr auto parse(format_parse_context& ctx)
//     {
//       return ctx.begin();
//     }

//     template <typename FormatContext>
//     auto format(const amaterasu::math::vec<L, T>& v, FormatContext& ctx) const
//     {
//       return format_to(ctx.out(), "{}", glm::to_string(v));
//     }
//   };

//   // Formatter for glm::vec<L, T, Q>
//   template <size_t L, typename T, glm::qualifier Q>
//   struct formatter<glm::vec<L, T, Q>, char, std::enable_if_t<std::is_arithmetic_v<T>>>
//   {
//     constexpr auto parse(format_parse_context& ctx)
//     {
//       return ctx.begin();
//     }

//     template <typename FormatContext>
//     auto format(const glm::vec<L, T, Q>& v, FormatContext& ctx) const
//     {
//       return format_to(ctx.out(), "{}", glm::to_string(v));
//     }
//   };

//   // Formatter for amaterasu::math::mat<C, R, T>
//   template <size_t C, size_t R, typename T>
//   struct formatter<amaterasu::math::mat<C, R, T>, char, std::enable_if_t<std::is_arithmetic_v<T>>>
//   {
//     constexpr auto parse(format_parse_context& ctx)
//     {
//       return ctx.begin();
//     }

//     template <typename FormatContext>
//     auto format(const amaterasu::math::mat<C, R, T>& m, FormatContext& ctx) const
//     {
//       return format_to(ctx.out(), "{}", glm::to_string(m));
//     }
//   };

//   // Formatter for glm::mat<C, R, T, Q>
//   template <size_t C, size_t R, typename T, glm::qualifier Q>
//   struct formatter<glm::mat<C, R, T, Q>, char, std::enable_if_t<std::is_arithmetic_v<T>>>
//   {
//     constexpr auto parse(format_parse_context& ctx)
//     {
//       return ctx.begin();
//     }

//     template <typename FormatContext>
//     auto format(const glm::mat<C, R, T, Q>& m, FormatContext& ctx) const
//     {
//       return format_to(ctx.out(), "{}", glm::to_string(m));
//     }
//   };
// } // namespace fmt
// #endif // defined(AMATERASU_MATH_ENABLE_FMT_CONVERT)

// #if defined(AMATERASU_MATH_ENABLE_NLOHMANN_JSON_CONVERT)
// namespace nlohmann
// {
//   void to_json(json& j, const amaterasu::vec2& vec);
//   void to_json(json& j, const amaterasu::vec3& vec);
//   void to_json(json& j, const amaterasu::vec4& vec);

//   void to_json(json& j, const amaterasu::vec2u& vec);
//   void to_json(json& j, const amaterasu::vec3u& vec);
//   void to_json(json& j, const amaterasu::vec4u& vec);

//   void to_json(json& j, const amaterasu::vec2i& vec);
//   void to_json(json& j, const amaterasu::vec3i& vec);
//   void to_json(json& j, const amaterasu::vec4i& vec);

//   void to_json(json& j, const amaterasu::mat2& mat);
//   void to_json(json& j, const amaterasu::mat3& mat);
//   void to_json(json& j, const amaterasu::mat4& mat);

//   void from_json(const json& j, amaterasu::vec2& vec);
//   void from_json(const json& j, amaterasu::vec3& vec);
//   void from_json(const json& j, amaterasu::vec4& vec);

//   void from_json(const json& j, amaterasu::vec2u& vec);
//   void from_json(const json& j, amaterasu::vec3u& vec);
//   void from_json(const json& j, amaterasu::vec4u& vec);

//   void from_json(const json& j, amaterasu::vec2i& vec);
//   void from_json(const json& j, amaterasu::vec3i& vec);
//   void from_json(const json& j, amaterasu::vec4i& vec);

//   void from_json(const json& j, amaterasu::mat2& mat);
//   void from_json(const json& j, amaterasu::mat3& mat);
//   void from_json(const json& j, amaterasu::mat4& mat);
// } // namespace nlohmann
// #endif // defined(AMATERASU_MATH_ENABLE_NLOHMANN_JSON_CONVERT)

#endif // ODEER_MATH_HPP
