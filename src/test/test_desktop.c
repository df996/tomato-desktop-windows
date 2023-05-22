//
// Created by cheng on 2023/5/22.
//

#include <Windows.h>
#include <stdio.h>
#include "../client/td_screen.h"

#pragma comment(linker,"/subsystem:windows")

static int cx_client, cy_client;

void OnPaint(HWND hwnd);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wparam, LPARAM lparam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    SetConsoleOutputCP(CP_UTF8);
    const wchar_t class_name[] = L"Window";

    // 配置窗口类
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = (LPCWSTR) class_name;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    // 注册窗口类
    RegisterClass(&wc);

    // 创建窗口
    HWND hwnd = CreateWindowEx(
            0,
            class_name,
            (LPCWSTR) L"测试",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
            NULL,
            NULL,
            hInstance,
            NULL
    );

    if (NULL == hwnd) {
        return 0;
    }

    // 显示&更新窗口
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // 消息循环
    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wparam, LPARAM lparam) {
    switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_PAINT: {
            OnPaint(hwnd);
            return 0;
        }
        case WM_SIZE:
            cx_client = LOWORD(lparam);
            cy_client = HIWORD(lparam);
            return 0;
        default:
            return DefWindowProc(hwnd, uMsg, wparam, lparam);
    }
}

void OnPaint(HWND hwnd) {
    PAINTSTRUCT ps;
    BITMAP a;
    td_screen_capture(&a);
    //HBITMAP hbitmap = CreateBitmapIndirect(&bitmap);
    HBITMAP hbitmap = td_screen_get_hbitmap();
    BITMAP bitmap;
    GetObject(hbitmap, sizeof(BITMAP), &bitmap);
    HDC hdc = BeginPaint(hwnd, &ps);
    HDC mem_dc = CreateCompatibleDC(hdc);

    SelectObject(mem_dc, hbitmap);

    int x, y;
    for (y = 0; y < cy_client; y += bitmap.bmHeight) {
        for (x = 0; x < cx_client; x += bitmap.bmWidth) {
            BitBlt(hdc, x, y, bitmap.bmWidth, bitmap.bmHeight, mem_dc, 0, 0, SRCCOPY);
        }
    }

    DeleteObject(hbitmap);
    DeleteDC(mem_dc);
    DeleteDC(hdc);
    EndPaint(hwnd, &ps);
}
