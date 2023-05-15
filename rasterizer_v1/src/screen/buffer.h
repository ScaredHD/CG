#pragma once

#include <vector>
#include <algorithm>

class Buffer {
  public:
    using uchar = unsigned char;

  public:
    Buffer(size_t len, uchar initValue = 0) : buf(len, initValue) {}

    uchar& operator[](size_t i) { return buf[i]; }
    const uchar& operator[](size_t i) const { return buf[i]; }

    void fill(uchar value, size_t start, size_t step = 1) {
        for (auto i = start; i < buf.size(); i += step) {
            buf[i] = value;
        }
    }

    uchar* data() { return buf.data(); }

  private:
    std::vector<uchar> buf;
};


class Image {
public:
    Image(int width, int height);
    int width;
    int heigth;

private:
    Buffer buf;
};