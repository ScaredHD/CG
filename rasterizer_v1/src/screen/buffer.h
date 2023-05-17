#pragma once

#include <vector>
#include <algorithm>

using Uchar = unsigned char;

class Buffer {
  public:
  public:
    Buffer(size_t len, Uchar initValue = 0) : buf(len, initValue) {}

    Uchar& operator[](size_t i) { return buf[i]; }
    const Uchar& operator[](size_t i) const { return buf[i]; }

    void fill(Uchar value, size_t start, size_t step = 1) {
        for (auto i = start; i < buf.size(); i += step) {
            buf[i] = value;
        }
    }

    Uchar* data() { return buf.data(); }

  private:
    std::vector<Uchar> buf;
};