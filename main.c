#include <windows.h>
#include <GL/GL.h>

#include <stdio.h>

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
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    //Flags
		PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
		32,                   // Colordepth of the framebuffer.
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		24,                   // Number of bits for the depthbuffer
		8,                    // Number of bits for the stencilbuffer
		0,                    // Number of Aux buffers in the framebuffer.
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
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
        DrawText(hdc, (char*)glGetString(GL_VERSION), -1, &rc, DT_CENTER|DT_SINGLELINE|DT_VCENTER);

        EndPaint(hwnd, &ps);
		break;
    default:
        return DefWindowProc(hwnd, Msg, wParam, lParam);
    }
    return 0;
}