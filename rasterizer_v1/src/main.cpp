
#include <iostream>
#include <chrono>
#include <iomanip>

#include "buffer.h"
#include "window.h"

using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;
TimePoint getTime() {
    return std::chrono::high_resolution_clock::now();
}

double timeInterval(const TimePoint& start, const TimePoint& end) {
    return std::chrono::duration<double, std::milli>(end - start).count();
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    Window w(800, 600, hInstance, nCmdShow);
    w.show();
    
    FpsCamera cam({{0, 0, 0}}, {{0, 0, -1}});
    w.bindCamera(&cam);

    Image<4> img(800, 600, {255, 0, 0, 0});

    double deltaTime = 0;
    while (w.isRunning) {
        TimePoint frameBegin = getTime();

        std::cout << std::setprecision(3) << std::fixed;
        std::cout << cam.location << "\t(p:" << cam.pitch() << "  y:" << cam.yaw() << ")\n";

        w.updateFrameBufferFromImage(img);
        w.drawFrameBuffer();

        TimePoint frameEnd = getTime();
        deltaTime = timeInterval(frameBegin, frameEnd);

        w.pollEvents(deltaTime);
    }
    
    return 0;
}
