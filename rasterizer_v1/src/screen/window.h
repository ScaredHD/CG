#pragma once

#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <windowsx.h>

#include "buffer.h"
#include "image.h"

class Window {
  public:
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

  public:
    Window(int width, int height, HINSTANCE hInstance, int nCmdShow);
    ~Window();

    HWND handle() const { return hwnd; }
    void createWindow(int &width, int &height, HINSTANCE &hInstance);

    void show();
    void pollEvents();

    void updateFrameBufferFromImage(const RgbaImage &image);
    void updateFrameBuffer(std::shared_ptr<Buffer> frameBuffer);
    void drawFrameBuffer();

    int width;
    int height;

    bool isRunning;

  private:
    void prepareDC();
    void quit();

    LRESULT handleMessages(UINT uMsg, WPARAM wParam, LPARAM lParam);
    void handleKeyEvents(UINT uMsg, WPARAM wParam, LPARAM lParam);
    void handleMouseEvents(UINT uMsg, WPARAM wParam, LPARAM lParam);

    void centerCursor();

    std::shared_ptr<Buffer> frameBuffer;

    HWND hwnd;
    HINSTANCE hInstance;
    int nCmdShow;

    void *surface;
    HDC windowDC;
    HDC memoryDC;

    RECT windowRect;
};