#include <windows.h>
#include <GL/GL.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam);

HGLRC _context;

int WinMain(__in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in_opt LPSTR lpCmdLine, __in int nShowCmd)
{
    (void)hPrevInstance;
    (void)lpCmdLine;
    (void)nShowCmd;

    MSG Msg;
    WNDCLASS  wc = {0};

    wc.style = CS_OWNDC;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
    wc.lpszClassName = "testOpenGL";

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW,
                          wc.lpszClassName,
                          "Window Title",
                          WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                          100,
                          120,
                          600,
                          100,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    UpdateWindow(hwnd);

    while(GetMessage(&Msg, NULL, 0, 0))
    {
        DispatchMessage(&Msg);
    }
	return 0;
}

void initContext(HWND hwnd)
{
    PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		32, // Colordepth of the framebuffer
		0, 0, 0, 0, 0, 0, 0, 0, // Bitplane & shift (RGBA)
		0, 0, 0, 0, 0, // Bitplane in acc buffer (TT then RGBA)
		24, // Nb of bits for the depthbuffer
		8, // Nb of bits for the stencilbuffer
		0, // Nb of Aux buffers in the framebuffer
		0,
		0, // Nb of overlay and underlay planes
		0,
        0, // Transparent color of underlay plane
        0
	};

    HDC hdc = GetDC(hwnd);
    int  pxlFormat = ChoosePixelFormat(hdc, &pfd); 
	SetPixelFormat(hdc, pxlFormat, &pfd);
    _context = wglCreateContext(hdc);
    wglMakeCurrent(hdc, _context);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    switch(Msg)
    {
    case WM_DESTROY:
        wglDeleteContext(_context);
        PostQuitMessage(WM_QUIT);
        break;

	case WM_CREATE:
        initContext(hwnd);
		break;

	case WM_PAINT:
        PAINTSTRUCT ps;
        RECT rc;
        HDC hdc = BeginPaint(hwnd, &ps);

        GetClientRect(hwnd, &rc);
        SetTextColor(hdc, 0);
        SetBkMode(hdc, TRANSPARENT);

        char* title = "OpenGL Version: ";
        char* version = (char*)glGetString(GL_VERSION);
        char* display = malloc(strlen(title) + strlen(version) + 1);
        strcat_s(display, strlen(title) + strlen(version) + 1, title);
        strcat_s(display, strlen(title) + strlen(version) + 1, version);

        DrawText(hdc, display, -1, &rc, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

        EndPaint(hwnd, &ps);
		break;
    default:
        return DefWindowProc(hwnd, Msg, wParam, lParam);
    }
    return 0;
}