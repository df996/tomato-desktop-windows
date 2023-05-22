//
// Created by Administrator on 2023/5/15.
//

#include "td_screen.h"

/**
 * 获取屏幕RECT
 * @return td_screen_rect
 */
td_screen_rect *td_screen_get_rect() {
    // 获取屏幕句柄
    HDC screen_hdc = GetDC(NULL);

    // 设置屏幕矩形结构
    td_screen_rect *rect = (td_screen_rect *) malloc(sizeof(td_screen_rect));
    rect->top = 0;
    rect->left = 0;
    rect->width = GetDeviceCaps(screen_hdc, HORZRES);
    rect->height= GetDeviceCaps(screen_hdc, VERTRES);

    // 释放
    ReleaseDC(NULL, screen_hdc);
    return rect;
}

HBITMAP td_screen_get_hbitmap() {
    // 屏幕矩形区域
    td_screen_rect *screen_rect = td_screen_get_rect();
    // 获取屏幕句柄
    HDC screen_hdc = GetDC(NULL);
    // 创建内存DC
    HDC mem_hdc = CreateCompatibleDC(screen_hdc);
    // 创建屏幕位图
    HBITMAP screen_hbitmap = CreateCompatibleBitmap(screen_hdc, screen_rect->width, screen_rect->height);
    // 将创建的位图关联到内存DC上
    SelectObject(mem_hdc, screen_hbitmap);
    // 复制屏幕DC像素到内存DC中
    BitBlt(mem_hdc, 0, 0, screen_rect->width, screen_rect->height, screen_hdc,0, 0, SRCCOPY);

    return screen_hbitmap;
}

/**
 * 截屏
 * @param bitmap 接收截屏的位图指针
 */
void td_screen_capture(BITMAP *bitmap) {
    // 屏幕矩形区域
    td_screen_rect *screen_rect = td_screen_get_rect();
    // 获取屏幕句柄
    HDC screen_hdc = GetDC(NULL);
    // 创建内存DC
    HDC mem_hdc = CreateCompatibleDC(screen_hdc);
    // 创建屏幕位图
    HBITMAP screen_hbitmap = CreateCompatibleBitmap(screen_hdc, screen_rect->width, screen_rect->height);
    // 将创建的位图关联到内存DC上
    SelectObject(mem_hdc, screen_hbitmap);
    // 复制屏幕DC像素到内存DC中
    BitBlt(mem_hdc, 0, 0, screen_rect->width, screen_rect->height, screen_hdc,0, 0, SRCCOPY);

    // 获取位图信息
    GetObject(screen_hbitmap, sizeof(BITMAP), bitmap);

    // 位图信息头
    BITMAPINFOHEADER bitmap_info_header = { 0 };
    bitmap_info_header.biBitCount = bitmap->bmBitsPixel;
    bitmap_info_header.biCompression = BI_RGB;
    bitmap_info_header.biHeight = bitmap->bmHeight;
    bitmap_info_header.biWidth = bitmap->bmWidth;
    bitmap_info_header.biPlanes = 1;
    bitmap_info_header.biSize = sizeof(BITMAPINFOHEADER);
    bitmap_info_header.biSizeImage = bitmap->bmWidthBytes * bitmap->bmHeight;

    // 复制位图到缓冲区
    bitmap->bmBits = (char *) malloc(bitmap_info_header.biSizeImage);
    GetDIBits(mem_hdc, (HBITMAP) screen_hbitmap, 0, (UINT) bitmap->bmHeight,
              bitmap->bmBits, (LPBITMAPINFO) &bitmap_info_header, DIB_RGB_COLORS);

    // 释放
    DeleteObject(screen_hbitmap);
    DeleteDC(mem_hdc);
    ReleaseDC(NULL, screen_hdc);
    free(screen_rect);
}

void td_screen_capture_to_file() {
    // 屏幕矩形区域
    td_screen_rect *screen_rect = td_screen_get_rect();
    // 获取屏幕句柄
    HDC screen_hdc = GetDC(NULL);
    // 创建内存DC
    HDC mem_hdc = CreateCompatibleDC(screen_hdc);
    // 创建屏幕位图
    HBITMAP screen_h_bmp = CreateCompatibleBitmap(screen_hdc, screen_rect->width, screen_rect->height);
    // 将创建的位图关联到内存DC上
    SelectObject(mem_hdc, screen_h_bmp);
    // 复制屏幕DC像素到内存DC中
    BitBlt(mem_hdc, 0, 0, screen_rect->width, screen_rect->height, screen_hdc,0, 0, SRCCOPY);

    // 获取位图信息
    BITMAP bitmap;
    GetObject(screen_h_bmp, sizeof(BITMAP), &bitmap);

    // 位图文件头
    BITMAPFILEHEADER bitmap_file_header = { 0 };
    bitmap_file_header.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    bitmap_file_header.bfSize = bitmap_file_header.bfOffBits + bitmap.bmWidthBytes * bitmap.bmHeight;
    bitmap_file_header.bfType = (WORD)BITMAP_HEADER_FLAG;

    // 位图信息头
    BITMAPINFOHEADER bitmap_info_header = { 0 };
    bitmap_info_header.biBitCount = bitmap.bmBitsPixel;
    bitmap_info_header.biCompression = BI_RGB;
    bitmap_info_header.biHeight = bitmap.bmHeight;
    bitmap_info_header.biWidth = bitmap.bmWidth;
    bitmap_info_header.biPlanes = 1;
    bitmap_info_header.biSize = sizeof(BITMAPINFOHEADER);
    bitmap_info_header.biSizeImage = bitmap.bmWidthBytes * bitmap.bmHeight;

    // 复制位图到缓冲区
    char *buff_bitmap = (char *) malloc(bitmap_info_header.biSizeImage);
    GetDIBits(mem_hdc, (HBITMAP) screen_h_bmp, 0, (UINT) bitmap.bmHeight,
              buff_bitmap, (LPBITMAPINFO) &bitmap_info_header, DIB_RGB_COLORS);

    // 保存文件
    FILE *fp = NULL;
    fopen_s(&fp, "capture.bmp", "w+b");
    fwrite(&bitmap_file_header, sizeof(BITMAPFILEHEADER), 1, fp);
    fwrite(&bitmap_info_header, sizeof(BITMAPINFOHEADER), 1, fp);
    fwrite(buff_bitmap, bitmap_info_header.biSizeImage, 1, fp);
    fclose(fp);
    fp = NULL;

    // 释放
    free(buff_bitmap);
    DeleteObject(screen_h_bmp);
    DeleteDC(mem_hdc);
    ReleaseDC(NULL, screen_hdc);
    free(screen_rect);
}