#include "window.h"

#include <iostream>

static int eventCount = 0;

Window::Window(int width, int height, HINSTANCE hInstance, int nCmdShow)
    : width(width), height(height), hInstance(hInstance), nCmdShow(nCmdShow) {
    createWindow(width, height, hInstance);
    prepareDC();
}

Window::~Window() {
    ReleaseDC(hwnd, windowDC);
    ReleaseDC(hwnd, memoryDC);
    DestroyWindow(hwnd);
}

void Window::createWindow(int &width, int &height, HINSTANCE &hInstance) {
    const wchar_t CLASS_NAME[] = L"Window Class";
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.style = CS_HREDRAW | CS_VREDRAW;

    RegisterClass(&wc);

    hwnd = CreateWindowEx(0,                            // Optional window styles.
                          CLASS_NAME,                   // Window class
                          L"Learn to Program Windows",  // Window text
                          WS_OVERLAPPEDWINDOW,          // Window style

                          // Position and size
                          CW_USEDEFAULT, CW_USEDEFAULT, width, height,

                          NULL,       // Parent window
                          NULL,       // Menu
                          hInstance,  // Instance handle
                          this        // Additional application data

    );

    if (hwnd == nullptr) {
        std::cout << "hwnd is nullptr\n";
    }

    isRunning = true;

    GetWindowRect(hwnd, &windowRect);
}

void Window::prepareDC() {
    windowDC = GetDC(hwnd);
    memoryDC = CreateCompatibleDC(windowDC);
    surface = nullptr;

    BITMAPINFO bmi{};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = -height;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    HBITMAP bitmap = CreateDIBSection(memoryDC, &bmi, DIB_RGB_COLORS, &surface, 0, 0);
    if (!bitmap || !surface) {
        std::cout << "create dib section failed\n";
    }

    HGDIOBJ old = SelectObject(memoryDC, bitmap);
    if (!old) {
        std::cout << "failed to select bitmap\n";
    }
    DeleteObject(old);
}

void Window::show() {
    ShowWindow(hwnd, nCmdShow);
}

void Window::pollEvents() {
    MSG msg{};
    while (PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void Window::updateFrameBuffer(std::shared_ptr<Buffer> frameBuffer) {
    this->frameBuffer = frameBuffer;
}

void Window::updateFrameBufferFromImage(const RgbaImage &image) {
    updateFrameBuffer(image.buffer());
}

void Window::drawFrameBuffer() {
    memcpy(surface, frameBuffer->data(), width * height * 4);
    BitBlt(windowDC, 0, 0, width, height, memoryDC, 0, 0, SRCCOPY);
}

LRESULT CALLBACK Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    Window *pThis = nullptr;
    if (uMsg == WM_NCCREATE) {
        auto pCreate = (CREATESTRUCT *)lParam;
        pThis = (Window *)pCreate->lpCreateParams;
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);
        pThis->hwnd = hwnd;
    } else {
        pThis = (Window *)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    }

    if (pThis) {
        return pThis->handleMessages(uMsg, wParam, lParam);
    } else {
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

static int val = 2;

LRESULT Window::handleMessages(UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CLOSE:
            quit();
            return 0;
        case WM_MOUSEMOVE:
            handleMouseEvents(uMsg, wParam, lParam);
            return 0;
        case WM_KEYDOWN:
            handleKeyEvents(uMsg, wParam, lParam);
            return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void Window::handleKeyEvents(UINT uMsg, WPARAM wParam, LPARAM lParam) {
    auto keyCode = LOWORD(wParam);
    switch (keyCode) {
        case VK_ESCAPE:
            quit();
    }
}

void Window::handleMouseEvents(UINT uMsg, WPARAM wParam, LPARAM lParam) {
    int xPos = GET_X_LPARAM(lParam);
    int yPos = GET_Y_LPARAM(lParam);
    std::cout << xPos << ", " << yPos << "\n";
    centerCursor();
}

void Window::quit() {
    isRunning = false;
    PostQuitMessage(0);
}

void Window::centerCursor() {
    SetCursorPos((windowRect.left + windowRect.right) / 2,
                 (windowRect.top + windowRect.bottom) / 2);
}
