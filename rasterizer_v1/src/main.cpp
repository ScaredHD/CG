
#include <iostream>
#include <chrono>

#include "rasterizer.h"
#include "testGeometry.h"
#include "transformation.h"

using std::make_shared;

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
    // w.show();

    FpsCamera cam({{0, 0, 0}}, {{0, 0, -1}});
    cam.aspectRatio = double(canvasWidth) / canvasHeight;

    Rasterizer r;
    r.camera = make_shared<FpsCamera>(cam);
    r.window = &w;
    r.window->show();
    r.vShader = make_shared<VertexShader>();
    r.fShader = make_shared<FragmentShader>(canvasWidth, canvasHeight);

    TestGeometry testGeo;

    double deltaTime = 0;
    while (w.isRunning) {
        TimePoint frameBegin = getTime();

        printCameraPose(*r.camera);

        r.render(testGeo.triangle1);
        r.drawFrameBufferToWindow();

        TimePoint frameEnd = getTime();
        deltaTime = timeInterval(frameBegin, frameEnd);

        r.update(deltaTime);
    }

    return 0;
}
