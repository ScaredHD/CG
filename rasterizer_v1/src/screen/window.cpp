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

    windowRect = {0, 0, width, height};
    AdjustWindowRect(&windowRect, style, 0);

    hwnd = CreateWindowEx(0,              // Optional window styles.
                          CLASS_NAME,     // Window class
                          L"Rasterizer",  // Window text
                          style,          // Window style

                          // Position and size
                          CW_USEDEFAULT, CW_USEDEFAULT, windowRect.right - windowRect.left,
                          windowRect.bottom - windowRect.top,

                          NULL,       // Parent window
                          NULL,       // Menu
                          hInstance,  // Instance handle
                          this        // Additional application data

    );
    if (hwnd == nullptr) {
        std::cout << "hwnd is nullptr\n";
    }

    GetWindowRect(hwnd, &windowRect);
    GetClientRect(hwnd, &clientRect);

    isRunning = true;

    centerCursor();
    POINT cursor;
    GetCursorPos(&cursor);
    cursorX = cursor.x;
    cursorY = cursor.y;
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

void Window::pollEvents(double deltaTime) {
    this->deltaTime = deltaTime;

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

LRESULT Window::handleMessages(UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CLOSE:
            quit();
            return 0;
        case WM_MOUSEMOVE:
            handleMouseEvents(uMsg, wParam, lParam);
            return 0;
        case WM_KEYDOWN:
            if (!GetKeyboardState(keyState)) {
                std::cout << "failed to get keyboard state\n";
            }
            handleKeyEvents(uMsg, wParam, lParam);
            return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


bool Window::keyPressed(BYTE key) {
    return keyState[key] & 0x80;
}

void Window::handleKeyEvents(UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (keyPressed(VK_ESCAPE)) quit();
    if (keyPressed('W')) fpsCamera->moveForward(fpsCamera->moveSpeed * deltaTime);
    if (keyPressed('S')) fpsCamera->moveForward(-fpsCamera->moveSpeed * deltaTime);
    if (keyPressed('A')) fpsCamera->moveRight(-fpsCamera->moveSpeed * deltaTime);
    if (keyPressed('D')) fpsCamera->moveRight(fpsCamera->moveSpeed * deltaTime);
}

void Window::handleMouseEvents(UINT uMsg, WPARAM wParam, LPARAM lParam) {
    POINT cursor;
    GetCursorPos(&cursor);
    cursorDeltaX = cursor.x - cursorX;
    cursorDeltaY = cursor.y - cursorY;
    centerCursor();
    fpsCamera->lookUp(-cursorDeltaY * fpsCamera->mouseSensitivity);
    fpsCamera->lookRight(cursorDeltaX * fpsCamera->mouseSensitivity);
}

void Window::quit() {
    isRunning = false;
    PostQuitMessage(0);
}

void Window::centerCursor() {
    POINT p = {width / 2, height / 2};
    ClientToScreen(hwnd, &p);
    SetCursorPos(p.x, p.y);
}

void Window::bindCamera(Camera *camera) {
    if (camera->type == "fps") fpsCamera = reinterpret_cast<FpsCamera *>(camera);
    if (camera->type == "orbit") orbitCamera = reinterpret_cast<OrbitCamera *>(camera);
}

Mat4 Window::viewportTransformation() const {
    double w = width;
    double h = height;
    // clang-format off
    return {{{
        {w / 2, 0, 0, w / 2},
        {0, h / 2, 0, h / 2},
        {0, 0, 1,     0},
        {0, 0, 0,     1}
    }}};
    // clang-fromat on
}
