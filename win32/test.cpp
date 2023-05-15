#ifndef UNICODE
#define UNICODE
#endif

#include "window.h"


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    Window w(800, 600, hInstance, nCmdShow);

    while (true) {
        w.show();
    }
    
    return 0;
}
