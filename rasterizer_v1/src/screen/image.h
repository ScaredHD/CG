#pragma once

#include <memory>
#include <array>

#include "buffer.h"

template <size_t ChannelCount>
class Image {
  public:
    using Pixel = std::array<Uchar, ChannelCount>;

  public:
    Image(int width, int height, const Pixel& color = Pixel{});

    void setPixelValue(int x, int y, Pixel color);
    Pixel getPixelValue(int x, int y);
    void fill(const Pixel& color);

    std::shared_ptr<Buffer> buffer() const;

    int width;
    int height;
    int channelCount;

  private:
    size_t startIdx(int x, int y) { return (x * width + y) * ChannelCount; }

    Buffer buf;
};

template <size_t ChannelCount>
Image<ChannelCount>::Image(int width, int height, const Pixel& color)
    : width(width), height(height), channelCount(ChannelCount), buf(width * height * channelCount) {
    fill(color);
}

template <size_t ChannelCount>
void Image<ChannelCount>::setPixelValue(int x, int y, Pixel color) {
    auto s = startIdx(x, y);
    for (int i = 0; i < ChannelCount; ++i) {
        buf[s + i] = color[i];
    }
}

template <size_t ChannelCount>
typename Image<ChannelCount>::Pixel Image<ChannelCount>::getPixelValue(int x, int y) {
    Pixel ret;
    auto s = startIdx(x, y);
    for (int i = 0; i < ChannelCount; ++i) {
        ret[i] = buf[s + i];
    }
    return ret;
}

template <size_t ChannelCount>
void Image<ChannelCount>::fill(const Pixel& color) {
    for (int i = 0; i < ChannelCount; ++i) {
        buf.fill(color[i], i, ChannelCount);
    }
}

template <size_t ChannelCount>
std::shared_ptr<Buffer> Image<ChannelCount>::buffer() const {
    return std::make_shared<Buffer>(buf);
}

struct GrayscaleImage : public Image<1> {
    GrayscaleImage(int width, int height, Uchar color) : Image<1>(width, height, {color}) {}
};

using RgbImage = Image<3>;
using RgbaImage = Image<4>;

