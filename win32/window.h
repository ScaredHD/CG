
#include <windows.h>

class Window {
  public:
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

  public:
    Window(int width, int height, HINSTANCE hInstance,int nCmdShow);

    void show();
    void pollEvents();

  private:
    HWND hwnd;
    HINSTANCE hInstance;
    int nCmdShow;

    int w;
    int h;
};