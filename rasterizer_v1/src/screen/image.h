#pragma once

#include <memory>
#include <tuple>

#include "buffer.h"

class RgbaImage {
  public:
    using RgbaColor = std::tuple<Uchar, Uchar, Uchar, Uchar>;

  public:
    RgbaImage(int width, int height, RgbaColor color = {0, 0, 0, 0});

    void setPixelValue(int x, int y, RgbaColor color);
    RgbaColor getPixelValue(int x, int y);
    void fill(RgbaColor color);

    std::shared_ptr<Buffer> buffer() const;

    int width;
    int height;

  private:
    Buffer buf;

    size_t startIdx(int x, int y) { return (x * width + y) * 4; }
};