//
// Created by cheng on 2023/5/18.
//

// #undef UNICODE
#define WIN32_LEAN_AND_MEAN
#include <stdio.h>
#include <Windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "9999"

#pragma comment(linker,"/subsystem:windows")

static HBITMAP hbitmap;
static int cx_client, cy_client, cx_source, cy_source;
static HANDLE server_thread;
BITMAP bitmap;

void OnPaint(HWND hwnd);
DWORD WINAPI onStartServer(LPVOID lpParam);
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
        case WM_CREATE:
            onStartServer(NULL);
            hbitmap = (HBITMAP) LoadImage(
                    NULL,
                    L"C:\\Users\\cheng\\workspaces\\tomato-desktop-windows\\cmake-build-debug-visual-studio\\capture.bmp",
                    IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
            GetObject(hbitmap, sizeof(BITMAP), &bitmap);
            cx_source = bitmap.bmWidth;
            cy_source = bitmap.bmHeight;
            return 0;
        case WM_DESTROY:
            if (server_thread != NULL) {
                CloseHandle(server_thread);
            }
            PostQuitMessage(0);
            return 0;
        case WM_PAINT: {
            OnPaint(hwnd);
            return 0;
        }
        case WM_SIZE:
            cx_client = LOWORD(lparam);
            cy_client = HIWORD(lparam);
        default:
            return DefWindowProc(hwnd, uMsg, wparam, lparam);
    }
}

void OnPaint(HWND hwnd) {
    printf("OnPaint\n");
    int x, y;
    PAINTSTRUCT ps;

    HDC hdc = BeginPaint(hwnd, &ps);
    HDC mem_dc = CreateCompatibleDC(hdc);
    SelectObject(mem_dc, hbitmap);

    for (y = 0; y < cy_client; y += cy_source) {
        for (x = 0; x < cx_client; x += cx_source) {
            BitBlt(hdc, x, y, cx_source, cy_source, mem_dc, 0, 0, SRCCOPY);
        }
    }

    DeleteDC(mem_dc);
    DeleteDC(hdc);
    EndPaint(hwnd, &ps);
}

DWORD WINAPI onStartServer(LPVOID lpParam) {
    if (NULL == server_thread) {
        server_thread = CreateThread(NULL, 0, onStartServer, lpParam, 0, NULL);
        return 0;
    }

    WSADATA wsaData;
    int iResult;

    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;

    struct addrinfo *result = NULL;
    struct addrinfo hints;

    int iSendResult;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Create a SOCKET for the server to listen for client connections.
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Setup the TCP listening socket
    iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    // Accept a client socket
    ClientSocket = accept(ListenSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET) {
        printf("accept failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    // No longer need server socket
    closesocket(ListenSocket);

    // Receive until the peer shuts down the connection
    do {

        iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0) {
            printf("Bytes received: %d\n", iResult);

            // Echo the buffer back to the sender
            iSendResult = send( ClientSocket, recvbuf, iResult, 0 );
            if (iSendResult == SOCKET_ERROR) {
                printf("send failed with error: %d\n", WSAGetLastError());
                closesocket(ClientSocket);
                WSACleanup();
                return 1;
            }
            printf("Bytes sent: %d\n", iSendResult);
        }
        else if (iResult == 0)
            printf("Connection closing...\n");
        else  {
            printf("recv failed with error: %d\n", WSAGetLastError());
            closesocket(ClientSocket);
            WSACleanup();
            return 1;
        }

    } while (iResult > 0);

    // shutdown the connection since we're done
    iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ClientSocket);
        WSACleanup();
        return 1;
    }

    // cleanup
    closesocket(ClientSocket);
    WSACleanup();

    return 0;
}