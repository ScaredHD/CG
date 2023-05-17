#include "image.h"

RgbaImage::RgbaImage(int width, int height, RgbaColor color)
    : width(width), height(height), buf(width * height * 4) {
    fill(color);
}

void RgbaImage::setPixelValue(int x, int y, RgbaColor color) {
    auto start = startIdx(x, y);
    const auto& [r, g, b, a] = color;
    buf[start] = r;
    buf[start + 1] = g;
    buf[start + 2] = b;
    buf[start + 3] = a;
}

RgbaImage::RgbaColor RgbaImage::getPixelValue(int x, int y) {
    auto start = startIdx(x, y);
    return std::make_tuple(buf[start], buf[start + 1], buf[start + 2], buf[start + 3]);
}

void RgbaImage::fill(RgbaColor color) {
    const auto& [r, g, b, a] = color;
    buf.fill(r, 0, 4);
    buf.fill(g, 1, 4);
    buf.fill(b, 2, 4);
    buf.fill(a, 3, 4);
}

std::shared_ptr<Buffer> RgbaImage::buffer() const {
    return std::make_shared<Buffer>(buf);
}
