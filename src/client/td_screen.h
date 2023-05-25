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

// 屏幕矩形结构体
typedef struct td_screen_rect {
    long top;
    long left;
    long width;
    long height;
} td_screen_rect;

// 屏幕bitmap wrapper结构体相关
typedef struct td_screen_bitmap_wrapper td_screen_bitmap_wrapper;
typedef void (*func_free_td_screen_bitmap_wrapper)(td_screen_bitmap_wrapper *bitmap_wrapper);
typedef char *(*func_get_td_screen_bitmap_bytes)(td_screen_bitmap_wrapper *bitmapWrapper);

struct td_screen_bitmap_wrapper {
    BITMAPINFOHEADER bitmap_info_header;
    BITMAPFILEHEADER bitmap_file_header;
    BITMAP bitmap;
    HBITMAP hbitmap;
    HDC memory_hdc;
    func_free_td_screen_bitmap_wrapper free;
    func_get_td_screen_bitmap_bytes get_bytes;
};

/**
 * 获取屏幕RECT
 * @return td_screen_rect
 */
td_screen_rect * td_screen_get_rect();

/**
 * 释放td_screen_bitmap_wrapper结构体
 * @param bitmap_wrapper
 */
void td_screen_free_bitmap_wrapper(td_screen_bitmap_wrapper *bitmap_wrapper);

/**
 * 获取bitmap字节数据
 * @param bitmap_wrapper
 * @return
 */
char *td_screen_get_bitmap_bytes(td_screen_bitmap_wrapper *bitmap_wrapper);

/**
 * 获取当前屏幕bitmap wrapper结构
 * @return
 */
td_screen_bitmap_wrapper *td_screen_get_bitmap_wrapper();
#endif //TOMATO_DESKTOP_WINDOWS_TD_SCREEN_H