#include "window.h"

#include <iostream>

Window::Window(int width, int height, HINSTANCE hInstance, int nCmdShow)
    : w(width), h(height), hInstance(hInstance), nCmdShow(nCmdShow) {
    const wchar_t CLASS_NAME[] = L"Window Class";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    hwnd = CreateWindowEx(0,                            // Optional window styles.
                          CLASS_NAME,                   // Window class
                          L"Learn to Program Windows",  // Window text
                          WS_OVERLAPPEDWINDOW,          // Window style

                          // Position and size
                          CW_USEDEFAULT, CW_USEDEFAULT, w, h,

                          NULL,       // Parent window
                          NULL,       // Menu
                          hInstance,  // Instance handle
                          NULL        // Additional application data

    );

    if (hwnd == nullptr) {
        std::cout << "hwnd is nullptr\n";
    }
}

void Window::show() {
    ShowWindow(hwnd, nCmdShow);
}

void Window::pollEvents() {
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

LRESULT CALLBACK Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}