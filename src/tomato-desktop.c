//
// Created by Administrator on 2023/5/14.
//

#include <stdio.h>
#include "client/td_screen.h"
#include "core/td_rdp.h"

int main() {
    // SetConsoleOutputCP(CP_UTF8);
    printf("测试");
    // 设置当前进程DPI
    SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_SYSTEM_AWARE);

    td_screen_capture();
    return 0;
}
