#include <iostream>
#include <windows.h>
#include <time.h>
#include <math.h>

#define WIDTH 600
#define HIEGH 600
#define M_PI 3.14159265358979323846264

typedef unsigned int ui;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

POINT start_pos;
const COLORREF color_pen = 0x0000FF00;
HPEN pen = CreatePen(PS_SOLID, 2, color_pen);
ui num_lines = 150;
double radius = 250;
double alfa = 0;
int x_1 = 0, x_2 = 0, y_1 = 0, y_2 = 0;
ui i = 0;

double Radius(double a)
{
    return 200 + abs(sin(a * 0.004) - 0.5) * 10;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	MSG msg = { 0 };
    WNDCLASSW wc;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.lpszClassName = L"Cardioida";
    wc.hInstance = hInstance;
    wc.hbrBackground = (HBRUSH)GetStockObject(DKGRAY_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpfnWndProc = WndProc;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    RegisterClassW(&wc);
    HWND hwnd = CreateWindowW(wc.lpszClassName, L"Carioida",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        100, 100, WIDTH, HIEGH, NULL, NULL, hInstance, NULL);
    ShowWindow(hwnd, SW_SHOW);
    SetTimer(hwnd, 1, 30, NULL);
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg,
    WPARAM wParam, LPARAM lParam) {

    static RECT rect{ 0, 0, WIDTH, HIEGH };
    switch (msg) {

        case WM_DESTROY:

            PostQuitMessage(0);
            break;
        case WM_TIMER:
            HDC hdc = GetDC(hwnd);

            // Буферизация
            HDC memDC = CreateCompatibleDC(hdc);
            HBITMAP memBitmap = CreateCompatibleBitmap(hdc, WIDTH, HIEGH);
            HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, memBitmap);

            FillRect(memDC, &rect, (HBRUSH)GetStockObject(DKGRAY_BRUSH));
            SelectObject(memDC, pen);

            radius = Radius((double)GetTickCount());

            for (ui i = 0; i < num_lines; i++)
            {
                alfa = 2 * M_PI / num_lines * i;
                x_1 = WIDTH / 2 + (int)(radius * cos(alfa));
                y_1 = HIEGH / 2 + (int)(radius * sin(alfa));

                x_2 = WIDTH / 2 + (int)(radius * cos(alfa * 2));
                y_2 = HIEGH / 2 + (int)(radius * sin(alfa * 2));

                MoveToEx(memDC, x_1, y_1, &start_pos);
                LineTo(memDC, x_2, y_2);
            }

            // Переносим буфер на экран
            BitBlt(hdc, 0, 0, WIDTH, HIEGH, memDC, 0, 0, SRCCOPY);

            // Освобождаем ресурсы
            SelectObject(memDC, oldBitmap);
            DeleteObject(memBitmap);
            DeleteDC(memDC);
            ReleaseDC(hwnd, hdc);

            break;
    }
    return DefWindowProcW(hwnd, msg, wParam, lParam);
}