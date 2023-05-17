#include <iostream>

#include "buffer.h"
#include "window.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    Window w(800, 600, hInstance, nCmdShow);

    Image<4> img(800, 600, {255, 0, 0, 0});
    for (int i = 0; i < 200; ++i) {
        for (int j = 0; j < 100; ++j) {
            img.setPixelValue(i, j, {0, 255, 0, 0});
        }
    }

    w.updateFrameBufferFromImage(img);

    w.show();
    w.pollEvents();

    return 0;
}
