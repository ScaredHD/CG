#include <iostream>

#include "vec.h"
#include "buffer.h"
#include "window.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    Window w(800, 600, hInstance, nCmdShow);

    auto buf = std::make_shared<Buffer>(800 * 600 * 4);
    buf->fill(255, 0, 4);
    w.updateFrameBuffer(buf);

    w.show();
    w.pollEvents();
}
