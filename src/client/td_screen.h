//
// Created by Administrator on 2023/5/15.
//

#ifndef TOMATO_DESKTOP_WINDOWS_TD_SCREEN_H
#define TOMATO_DESKTOP_WINDOWS_TD_SCREEN_H

#ifndef BITMAP_HEADER_FLAG
#define BITMAP_HEADER_FLAG 0x4D42
#endif // BITMAP_HEADER_FLAG

#include <Windows.h>
#include <stdio.h>

typedef struct td_screen_rect {
    long top;
    long left;
    long width;
    long height;
} td_screen_rect;

/**
 * 获取屏幕RECT
 * @return td_screen_rect
 */
td_screen_rect * td_screen_get_rect();

/**
 * 获取屏幕位图句柄
 * @return 当前屏幕位图句柄
 */
HBITMAP td_screen_get_hbitmap();

/**
 * 截屏
 * @param bitmap 接收截屏的位图指针
 */
void td_screen_capture(BITMAP *bitmap);

void td_screen_capture_to_file();
#endif //TOMATO_DESKTOP_WINDOWS_TD_SCREEN_H