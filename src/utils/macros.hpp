#ifndef ODEER_MACROS_HPP
#define ODEER_MACROS_HPP

#define ODEER_STRINGIFY(x) #x

#define ODEER_BIND_MEMBER_EVENT_FUN(x) [this](auto&&... args) -> auto { return x(std::forward<decltype(args)>(args)...);}

//prefare __PRETTY_FUNCTION__ over __func__ even if both are supported
#if defined(__GNUC__) || defined(__ghs__)
	#define __ODEER_FUN__ __PRETTY_FUNCTION__
#elif defined(_MSC_VER)
  #define __ODEER_FUN__ __FUNCSIG__
#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
	#define __ODEER_FUN__ __FUNCTION__
#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
	#define __ODEER_FUN__ __FUNC__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
	#define __ODEER_FUN__ __func__
#elif defined(__cplusplus) && (__cplusplus >= 201103)
	#define __ODEER_FUN__ __func__
#else
	#define __ODEER_FUN__  \
  #pragma warning unknown function signature macro
#endif //defined(__GNUC__) || defined(__ghs__)

#if defined(__linux)
  #if defined(__ANDROID__) 
    #define ODEER_ANDROID
    #error Android platform is not supported
  #else 
    #define ODEER_LINUX
  #endif //defined(__ANDROID__)
#elif defined(_WIN32)
  #define ODEER_WINDOWS
  #if defined(_WIN64) 
    #define ODEER_WINDOWS_64
  #endif //defined(_WIN64)
#endif //defined(__linux)

#if defined(ODEER_WINDOWS) || defined(AMATERASU_LINUX) 
  #define ODEER_DESKTOP
#elif defined(ODEER_ANDROID)
  #define ODEER_MOBILE
#endif //defined(ODEER_WINDOWS) || defined(AMATERASU_LINUX) 

#ifdef ODEER_SHARED
#if defined(_MSC_VER)
  #ifdef ODEER_BUILD_SHARED
    #define ODEER_API __declspec(dllexport)
  #else
    #define ODEER_API __declspec(dllimport)
  #endif //ODEER_BUILD_SHARED
#elif defined(__GNUC__) || defined(__ghs__) || defined(__clang__)
  #ifdef ODEER_BUILD_SHARED
    #define ODEER_API __attribute__((visibility("default")))
  #else
    #define ODEER_API 
  #endif //ODEER_BUILD_SHARED
#else
  #define ODEER_API
  #pragma warning unknown dynamic link import/export semantics
#endif //defined(_MSC_VER)
#else
  #define ODEER_API
#endif //ODEER_SHARED

//TODO: assertion/verification needs better logging
#if defined(ODEER_ENABLE_ASSERT)
  #define ODEER_ASSERT(condition, ...) \
          do\
          { \
            if(!(condition)) \
            { \
              std::cerr << "Assertion Failed: " << ODEER_STRINGIFY(condition) << "\n"; \
              if(sizeof(#__VA_ARGS__) > 1) \
              { \
                  std::cerr << "Error: " << __VA_ARGS__ << "\n"; \
              } \
              std::exit(EXIT_FAILURE); \
            } \
          } while(false)
#else 
  #define ODEER_ASSERT(condition, ...) 
#endif //defined(ODEER_ENABLE_ASSERT)

#if defined(ODEER_ENABLE_VERIFY)
  #define ODEER_VERIFY(condition, ...) \
          do\
          { \
            if(!(condition)) \
            { \
              std::cerr << "Verification Failed: " << ODEER_STRINGIFY(condition) << "\n"; \
              if(sizeof(#__VA_ARGS__) > 1) \
              { \
                  std::cerr << "Error: " << __VA_ARGS__ << "\n"; \
              } \
            } \
          } while(false)
#else 
  #define ODEER_VERIFY(condition, ...) 
#endif //defined(ODEER_ENABLE_VERIFY)

#endif //ODEER_MACROS_HPP
