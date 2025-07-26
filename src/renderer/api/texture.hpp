#ifndef ODEER_TEXTURE_HPP
#define ODEER_TEXTURE_HPP

#include "utils/macros.hpp"

#include <memory>
#include <cstdint>

namespace odeer 
{
  class ODEER_API texture 
  {
  public:
    enum class internal_format
    {
      none, 
      rgba8, rgb8, rgba, rgb, rgb32ui,
      depth24stencil8,
      uint32, int32
    };

    enum class format 
    {
      none = 0,
      rgb, rgba, rgb_int,
      depth_stencil,
      red_int
    };

    enum class filter 
    {
      linear, nearest
    };

    enum class wrap 
    {
      clamp_to_edge, clamp_to_border, repeat
    };

    struct specs 
    {
      specs() = default;
      specs(filter min, filter mag, wrap s, wrap r, wrap t, format fmt, internal_format ifmt, bool gen_mip_map, bool auto_fmt)
        : min(min), mag(mag), s(s), r(r), t(t), fmt(fmt), ifmt(ifmt), 
          gen_mip_map(gen_mip_map), auto_fmt(auto_fmt)
      {
      }

      specs(format fmt, internal_format ifmt)
        : fmt(fmt), ifmt(ifmt)
      {
      }

      filter min = filter::nearest, 
             mag = filter::nearest;

      wrap s = wrap::clamp_to_edge,
           r = wrap::clamp_to_edge,
           t = wrap::clamp_to_edge;

      format fmt = format::rgba;
      internal_format ifmt = internal_format::rgba;
      bool gen_mip_map = false;
      bool auto_fmt = true;
    };
  public:
    virtual ~texture() = default;

    virtual uint32_t get_width() const = 0;
    virtual uint32_t get_height() const = 0;

    virtual uint32_t get_renderer_id() const = 0;

    virtual void set_data(size_t data_size, const void* data, uint8_t slot = 0) = 0;

    virtual void bind(uint8_t slot = 0) const = 0;
  };

  class ODEER_API texture2d : public texture 
  {
  public:
    texture2d(uint32_t width, uint32_t height, size_t data_size = 0, const void* data = nullptr);
    texture2d(uint8_t* data, int w, int h, int color_channels, const specs& spec = {});
    virtual ~texture2d();
    
    void set_data(size_t data_size, const void* data, uint8_t slot = 0) override;
    void bind(uint8_t slot = 0) const override;
    uint32_t get_renderer_id() const override;
    uint32_t get_width() const override;
    uint32_t get_height() const override;
  private:
    struct data;
    data* m_data;
  };
}

#endif //ODEER_TEXTURE_HPP
