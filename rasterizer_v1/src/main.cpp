#include <iostream>

#include "buffer.h"
#include "window.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    Window w(800, 600, hInstance, nCmdShow);

    RgbaImage img(800, 600, {255, 0, 0, 0});
    
    w.updateFrameBufferFromImage(img);

    w.show();
    w.pollEvents();

    return 0;
}
