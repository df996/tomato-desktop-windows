//
// Created by Administrator on 2023/5/15.
//

#ifndef TOMATO_DESKTOP_WINDOWS_TD_SCREEN_H
#define TOMATO_DESKTOP_WINDOWS_TD_SCREEN_H

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
void td_screen_capture();
#endif //TOMATO_DESKTOP_WINDOWS_TD_SCREEN_H
