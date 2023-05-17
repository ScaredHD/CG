#include <iostream>

#include <chrono>

#include "buffer.h"
#include "window.h"

using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;
TimePoint getTime() {
    return std::chrono::high_resolution_clock::now();
}

auto timeIntervalInMs(const TimePoint& start, const TimePoint& end) {
    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    Window w(800, 600, hInstance, nCmdShow);
    w.show();

    Image<4> img(800, 600, {255, 0, 0, 0});
    for (int i = 0; i < 200; ++i) {
        for (int j = 0; j < 100; ++j) {
            img.setPixelValue(i, j, {0, 255, 0, 0});
        }
    }

    while (w.isRunning) {
        w.updateFrameBufferFromImage(img);
        w.drawFrameBuffer();
        w.pollEvents();
    }

    return 0;
}
