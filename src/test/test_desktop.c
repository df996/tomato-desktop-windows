//
// Created by cheng on 2023/5/22.
//

#include <Windows.h>
#include <stdio.h>
#include "../client/td_screen.h"

#pragma comment(linker,"/subsystem:windows")

#define ID_TIMER = 1

static int cx_client, cy_client;
static HANDLE ui_thread_id;

void OnPaint(HWND hwnd);
DWORD WINAPI ui_render(LPVOID lpvoid);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wparam, LPARAM lparam);
void CALLBACK play(HWND hwnd, UINT uMsg, UINT idTimer, DWORD dwTime);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // 设置控制台输出编码
    SetConsoleOutputCP(CP_UTF8);
    // 设置当前进程DPI
    SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_SYSTEM_AWARE);
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
        case WM_CREATE:
            SetTimer(hwnd, 1, 1000 / 60, (TIMERPROC) play);
            //CreateThread(NULL, 0, ui_render, hwnd, 0, NULL);
            return 0;
        case WM_PAINT:
            OnPaint(hwnd);
            return 0;
        case WM_SIZE:
            cx_client = LOWORD(lparam);
            cy_client = HIWORD(lparam);
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        default:
            return DefWindowProc(hwnd, uMsg, wparam, lparam);
    }
}

DWORD WINAPI ui_render(LPVOID lpvoid) {
    HWND hwnd = (HWND) lpvoid;
    for(;;) {
        Sleep(1000 / 60);
        printf("hwnd: %d, 线程循环\n", hwnd);
        OnPaint(hwnd);
    }
    return 0;
}

void CALLBACK play(HWND hwnd, UINT uMsg, UINT idTimer, DWORD dwTime) {
    // MessageBox(hwnd, (LPCWSTR) "TEST", (LPCWSTR) "TEST", MB_OK);
    RECT rect;
    rect.left = 0;
    rect.top = 0;
    rect.right = cx_client;
    rect.bottom = cy_client;
    InvalidateRect(hwnd, &rect, FALSE);
    UpdateWindow(hwnd);
}

void OnPaint(HWND hwnd) {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);
    td_screen_bitmap_wrapper *bitmap_wrapper = td_screen_get_bitmap_wrapper();
    StretchBlt(hdc, 0, 0, cx_client, cy_client,
               bitmap_wrapper->memory_hdc, 0, 0, bitmap_wrapper->bitmap.bmWidth, bitmap_wrapper->bitmap.bmHeight,
               SRCCOPY);
    bitmap_wrapper->free(bitmap_wrapper);
    free(bitmap_wrapper);
    DeleteDC(hdc);
    EndPaint(hwnd, &ps);
}
