
#include <iostream>
#include <chrono>
#include <iomanip>

#include "rasterizer.h"
#include "testGeometry.h"
#include "transformation.h"

using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;

TimePoint getTime() {
    return std::chrono::high_resolution_clock::now();
}

double timeInterval(const TimePoint& start, const TimePoint& end) {
    return std::chrono::duration<double, std::milli>(end - start).count();
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    int canvasWidth = 800;
    int canvasHeight = 600;
    Window w(canvasWidth, canvasHeight, hInstance, nCmdShow);
    w.show();
    
    FpsCamera cam({{0, 0, 0}}, {{0, 0, -1}});
    cam.aspectRatio = double(canvasWidth) / canvasHeight;
    w.bindCamera(&cam);

    Rasterizer r;
    r.camera = &cam;
    r.window = &w;
    r.vShader = new VertexShader;
    r.fShader = new FragmentShader(canvasWidth, canvasHeight);

    TestGeometry testGeo;

    double deltaTime = 0;
    while (w.isRunning) {
        TimePoint frameBegin = getTime();

        std::cout << std::setprecision(3) << std::fixed;
        std::cout << cam.location << "\t(p:" << cam.pitch() << "\ty:" << cam.yaw() << ")\n";

        r.render(testGeo.triangle1);
        w.drawFrameBuffer();

        TimePoint frameEnd = getTime();
        deltaTime = timeInterval(frameBegin, frameEnd);

        w.pollEvents(deltaTime);
    }
    
    return 0;
}
