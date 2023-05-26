#pragma once

#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <windowsx.h>

#include "buffer.h"
#include "image.h"
#include "camera.h"

class Window {
  public:
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

  public:
    Window(int width, int height, HINSTANCE hInstance, int nCmdShow);
    ~Window();

    HWND handle() const { return hwnd; }
    void createWindow(int &width, int &height, HINSTANCE &hInstance);
    void bindCamera(Camera* camera);

    void show();
    void pollEvents(double deltaTime);

    void updateFrameBufferFromImage(const RgbaImage &image);
    void updateFrameBuffer(std::shared_ptr<Buffer> frameBuffer);
    void drawFrameBuffer();

    Mat4 viewportTransformation() const;

    int width;
    int height;

    bool isRunning;

  private:
    void prepareDC();
    void quit();

    LRESULT handleMessages(UINT uMsg, WPARAM wParam, LPARAM lParam);
    void handleKeyEvents(UINT uMsg, WPARAM wParam, LPARAM lParam);
    void handleMouseEvents(UINT uMsg, WPARAM wParam, LPARAM lParam);
    bool keyPressed(BYTE key);

    void centerCursor();

    HWND hwnd;
    HINSTANCE hInstance;
    int nCmdShow;
    DWORD style = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

    std::shared_ptr<Buffer> frameBuffer;

    void *surface;
    HDC windowDC;
    HDC memoryDC;

    RECT windowRect;
    RECT clientRect;

    BYTE keyState[256];

    int cursorX;
    int cursorY;
    int cursorDeltaX;
    int cursorDeltaY;

    // camera with which user input will interact
    FpsCamera* fpsCamera = nullptr;
    OrbitCamera* orbitCamera = nullptr;

    double deltaTime;
};