//
// Created by Administrator on 2023/5/15.
//

#include "td_screen.h"

/**
 * 获取屏幕RECT
 * @return td_screen_rect
 */
td_screen_rect *td_screen_get_rect() {
    td_screen_rect *rect = (td_screen_rect *) malloc(sizeof(td_screen_rect));
    rect->top = 0;
    rect->left = 0;
}

void td_screen_capture() {
    // Windows 98，WindowsNT 5.0或以后版本：如果该参数为Null，GetDC检索首要显示器的设备上下文环境，
    // 要得到其他显示器的设备上下文环境，可使用EnumDisplayMonitors和CreateDc函数。
    HDC hdc = GetDC(NULL);
    ReleaseDC(NULL, hdc);
}